#include <PRVTPB.h>

#include <windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#include <string>
#include <wrl.h>
#include <shellapi.h>

using namespace Microsoft::WRL;

HRESULT DXError = NULL;

struct GPUInterface{
  struct SoftInterface {
    ComPtr<IDXGIAdapter> softInterface;

    SoftInterface(ComPtr<IDXGIFactory4> factory) {
      ComPtr<IDXGIAdapter> warpAdapter;

      DXError =
        factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));

      InternalLog(
        "Creating Virtual Device",
        "Gathering Software Adaptor",
        "Failed Gathering Software Adaptor"
      );

      if (FAILED(DXError)) {
        InternalReport(
          "Creating Virtual Device",
          "Gathering Software Adaptor",
          "Failed Gathering Software Adaptor"
        );
      };
    };

  };
  
  struct HardInterface {
    ComPtr<IDXGIAdapter1> hardInterface;
    
    HardInterface(
      IDXGIFactory1* pFactory,
      bool requestHighPerformanceAdapter) {
      
      hardInterface = nullptr;

      ComPtr<IDXGIAdapter1> adapter;

      ComPtr<IDXGIFactory6> factory6;
      if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
      {
        for (
          UINT adapterIndex = 0;
          SUCCEEDED(factory6->EnumAdapterByGpuPreference(
            adapterIndex,
            requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
            IID_PPV_ARGS(&adapter)));
          ++adapterIndex)
        {
          DXGI_ADAPTER_DESC1 desc;
          adapter->GetDesc1(&desc);

          if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
          }

          if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
            break;
          }
        }
      }

      if (adapter.Get() == nullptr)
      {
        for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
        {
          DXGI_ADAPTER_DESC1 desc;
          adapter->GetDesc1(&desc);

          if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
          {
            // Don't select the Basic Render Driver adapter.
            // If you want a software adapter, pass in "/warp" on the command line.
            continue;
          }

          if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
          {
            break;
          }
        }
      }

      hardInterface = adapter.Detach();
    };
  };

  SoftInterface* softInterface;
  HardInterface* hardInterface;
  ComPtr<ID3D12Device> DXDevice;
  ComPtr<ID3D12CommandQueue> CmdQueue;

  GPUInterface(ComPtr<IDXGIFactory4> factory, bool UseWarpDevice) {
    if (UseWarpDevice) {
      InternalLog(
        "Creating Virtual Device",
        "Integrating Software Adaptor",
        "Beginning Adaptor Creation"
      ); //InternalLog

      softInterface = new SoftInterface(factory);
      
      InternalLog(
        "Creating Virtual Device",
        "Integrating Software Adaptor",
        "Beginning Software Integration"
      ); //InternalLog

      DXError = D3D12CreateDevice(
        softInterface->softInterface.Get(),
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&DXDevice)
      ); //CreateDevice

      if (FAILED(DXError)) {
        InternalReport(
          "Creating Virtual Device",
          "Integrating Software Adaptor",
          "SoftwareAdaptorFailed"
        ); //InternalReport
      }; //If Failed

    } 
    else {
      InternalLog(
        "Creating Virtual Device",
        "Integrating Hardware Adaptor",
        "Beginning Adaptor Creation"
      ); //InternalLog

      hardInterface = new HardInterface(factory.Get(), true);

      InternalLog(
        "Creating Virtual Device",
        "Integrating Hardware Adaptor",
        "Beginning Hardware Integration"
      ); //InternalLog

      D3D12CreateDevice(
        hardInterface->hardInterface.Get(),
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&DXDevice)
      ); //CreateDevice

      if (FAILED(DXError)) {
        InternalReport(
          "Creating Virtual Device",
          "Integrating Hardware Adaptor",
          "HardwareAdaptorFailed"
        ); //InternalReport
      }; //If Failed
    }
  
    //Create CmdQueue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    DXError = DXDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&CmdQueue));

    if (FAILED(DXError)) {
      InternalReport("", "", "");
    }; // If Failed
  };

  ~GPUInterface() {
    delete softInterface;
    delete hardInterface;
  };
};

struct DXInterface{
  ComPtr<IDXGIFactory4> factory;
  GPUInterface* gpu;

  DXInterface() {
    DirectXInterface();

    gpu = new GPUInterface(factory, true);
  }; //DXInterface

  void DirectXInterface() {
    UINT dxgiFactoryFlags = 0;

    if (DEBUG) {
      Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
      if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();

        // Enable additional debug layers.
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
      }
    };
  };

  ComPtr<ID3D12Device> GetDxDevice() {
    return gpu->DXDevice;
  };

  ComPtr<ID3D12CommandQueue> GetCmdQueue() {
    return gpu->CmdQueue;
  };

  ~DXInterface() {
    delete gpu;
  };
};

PRIVATEPB::DirectX12::DirectX12(pb::Config::Render* R) {
  
  WNDCLASSEX windowClass = { 0 };
  windowClass.cbSize = sizeof(WNDCLASSEX);
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = WindowProc;
  windowClass.hInstance = R->GetDirectXInstanceID();
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.lpszClassName = L"DXSampleClass";
  RegisterClassEx(&windowClass);

  RECT windowRect = { 0, 0, static_cast<LONG>(R->GetWindowWidth()), static_cast<LONG>(R->GetWindowWidth()) };
  AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

  auto winInstance = CreateWindow(
    windowClass.lpszClassName,
    R->GetWindowName(),
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    windowRect.right - windowRect.left,
    windowRect.bottom - windowRect.top,
    nullptr,
    nullptr,
    R->GetDirectXInstanceID(),
    /* Replace With Certain Replicated Class*/);


  auto DX = new DXInterface();
  auto DXDevice = DX->GetDxDevice();
  auto CmdQueue = DX->GetCmdQueue();

  constexpr INT MAX_FRAMES_IN_FLIGHT = 2;

  //Create Descriptors
  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = MAX_FRAMES_IN_FLIGHT;
  swapChainDesc.Width = R->GetWindowWidth();
  swapChainDesc.Height = R->GetWindowHeight();
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  //Create SwapChain
  ComPtr<IDXGISwapChain1> swapChain;
  ThrowIfFailed(DX->factory->CreateSwapChainForHwnd(
    CmdQueue.Get(),
    winInstance, //Window Handle
    &swapChainDesc,
    nullptr,
    nullptr,
    &swapChain
  ));

  ThrowIfFailed(DX->factory->MakeWindowAssociation(winInstance, DXGI_MWA_NO_ALT_ENTER));
  
  UINT m_rtvDescriptorSize;
  UINT m_frameIndex;
  ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
  ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
  ComPtr<ID3D12Resource> m_renderTargets[MAX_FRAMES_IN_FLIGHT];
  ComPtr<ID3D12CommandAllocator> m_commandAllocators[MAX_FRAMES_IN_FLIGHT];
  ComPtr<IDXGISwapChain3> m_swapChain;
  ThrowIfFailed(swapChain.As(&m_swapChain));
  m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

  // Create descriptor heaps.
    // Describe and create a render target view (RTV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = MAX_FRAMES_IN_FLIGHT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(DXDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

    // Describe and create a depth stencil view (DSV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(DXDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap)));

    m_rtvDescriptorSize = DXDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

  // Create frame resources.
  {
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    // Create a RTV and a command allocator for each frame.
    for (UINT n = 0; n < MAX_FRAMES_IN_FLIGHT; n++)
    {
      ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
      DXDevice->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
      rtvHandle.Offset(1, m_rtvDescriptorSize);

      ThrowIfFailed(DXDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n])));
    }
  }

  // Create the depth stencil view.
  {
    D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
    depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

    ThrowIfFailed(DXDevice->CreateCommittedResource(
      &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
      D3D12_HEAP_FLAG_NONE,
      &CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, m_width, m_height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE), // Performance tip: Deny shader resource access to resources that don't need shader resource views.
      D3D12_RESOURCE_STATE_DEPTH_WRITE,
      &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0), // Performance tip: Tell the runtime at resource creation the desired clear value.
      IID_PPV_ARGS(&m_depthStencil)
    ));

    DXDevice->CreateDepthStencilView(m_depthStencil.Get(), &depthStencilDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
  }


};

PRIVATEPB::DirectX12::~DirectX12() {

};