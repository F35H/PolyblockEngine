#include <PRVTPB.h>

#include <windows.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DXHelpers/d3dx12.h>
#include <dxgi.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#include <string>
#include <wrl.h>
#include <shellapi.h>

using namespace Microsoft::WRL;

HRESULT DXError = NULL;
ComPtr<ID3D12CommandAllocator> cmdAllocator;
ComPtr<ID3D12GraphicsCommandList> cmdList;

UINT m_rtvDescriptorSize;
UINT m_frameIndex;
constexpr INT MAX_FRAMES_IN_FLIGHT = 2;
ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
ComPtr<ID3D12Resource> m_renderTargets[MAX_FRAMES_IN_FLIGHT];
//ComPtr<ID3D12CommandAllocator> cmdAllocator[MAX_FRAMES_IN_FLIGHT];
ComPtr<IDXGISwapChain3> m_swapChain;
ComPtr<ID3D12PipelineState> m_pipelineState;

UINT64 fenceValue;
ComPtr<ID3D12Fence> fence;
HANDLE fenceEvent;

inline void DXCheckError(const char* macro, const char* meso, const char* micro) {
  if (FAILED(DXError)) {
    InternalLog(macro, meso, micro);
  }; //If Failed
}; //DXCheckError

struct GPUInterface {
  struct SoftInterface {
    ComPtr<IDXGIAdapter> softInterface;

    SoftInterface(ComPtr<IDXGIFactory4> &factory) {
      ComPtr<IDXGIAdapter> warpAdapter;

      DXError =
        factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));

      InternalLog(
        "Creating Virtual Device",
        "Gathering Software Adaptor",
        "Failed Gathering Software Adaptor"
      );

      DXCheckError(
        "Creating Virtual Device",
        "Gathering Software Adaptor",
        "Failed Gathering Software Adaptor"
      ); //DXCheckError
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
  }; //HardInterface

  SoftInterface* softInterface;
  HardInterface* hardInterface;
  ComPtr<ID3D12Device> dxDevice;
  ComPtr<ID3D12CommandQueue> cmdQueue;

  GPUInterface(ComPtr<IDXGIFactory4> &factory, bool UseWarpDevice) {
    UINT dxgiFactoryFlags = 0;

    if (DEBUG) {
      Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
      if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();

        // Enable additional debug layers.
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
      }
    };
    
    CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
    
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
        IID_PPV_ARGS(&dxDevice)
      ); //CreateDevice

      DXCheckError(
        "Creating Virtual Device",
        "Integrating Software Adaptor",
        "Software Adaptor Failed"
      ); //DXCheckError



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
        IID_PPV_ARGS(&dxDevice)
      ); //CreateDevice

      DXCheckError(
        "Creating Virtual Device",
        "Integrating Hardware Adaptor",
        "Hardaware Adaptor Failed"
      ); //DXCheckError
    } //Else WarpDrive

    //Create CmdQueue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    DXError = dxDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));

    DXCheckError(
      "Creating Command Queue",
      "Informing Queue",
      "Failed Informing Queue"
    ); //DXCheckError
  }; //GPUInterface

  ~GPUInterface() {
    delete softInterface;
    delete hardInterface;
  }; //GPUInterface
}; //GPUInterface

struct DXInterface {
  ComPtr<IDXGIFactory4> factory;
  GPUInterface* gpu;

  DXInterface() {
    DirectXInterface();

    gpu = new GPUInterface(factory, true);
  }; //DXInterface

  void DirectXInterface() {
  };

  ComPtr<ID3D12Device> GetDxDevice() {
    return gpu->dxDevice;
  };

  ComPtr<ID3D12CommandQueue> GetCmdQueue() {
    return gpu->cmdQueue;
  };

  ~DXInterface() {
    delete gpu;
  };
};

void PopulateCommandList() {
  DXError = cmdAllocator->Reset();

  DXCheckError(
    "Populating Command List",
    "Clearing Command Allocator",
    "Failed Clearing"
  ); //DXCheckError

  DXError = cmdList->Reset(cmdAllocator.Get(), m_pipelineState.Get());

  DXCheckError(
    "Populating Command List",
    "Clearing Command Allocator",
    "Failed Clearing"
  ); //DXCheckError

  const auto rsrcOne = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

  // Indicate that the back buffer will be used as a render target.
  cmdList->ResourceBarrier(1, &rsrcOne);

  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);

  // Record commands.
  const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
  cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

  const auto rsrcTwo = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

  // Indicate that the back buffer will now be used to present.
  cmdList->ResourceBarrier(1, &rsrcTwo);

  DXError = cmdList->Close();

  DXCheckError(
    "Populating Command List",
    "Closing Command List",
    "Failed Closing"
  ); //DXCheckError
} //Populating Command List

void WaitForPreviousFrame(DXInterface* DX) {
  auto cmdQueue = DX->GetCmdQueue();

  // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
  // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
  // sample illustrates how to use fences for efficient resource usage and to
  // maximize GPU utilization.

  // Signal and increment the fence value.
  const UINT64 fValue = fenceValue;
  DXError = cmdQueue->Signal(fence.Get(), fValue);

  //DXCheckError(
  //
  //); //DXCheckError
  fenceValue++;

  // Wait until the previous frame is finished.
  if (fence->GetCompletedValue() < fValue)
  {
    DXError = fence->SetEventOnCompletion(fValue, fenceEvent);
    WaitForSingleObject(fenceEvent, INFINITE);
  }

  m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

// Render the scene.
void OnRender(DXInterface* DX) {
  auto cmdQueue = DX->GetCmdQueue();

  // Record all the commands we need to render the scene into the command list.
  PopulateCommandList();

  // Execute the command list.
  ID3D12CommandList* ppCommandLists[] = { cmdList.Get() };
  cmdQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

  // Present the frame.

  if (FAILED(m_swapChain->Present(1, 0))) {

  }; 

  WaitForPreviousFrame(DX);
} //OnRender

void OnUpdate()
{
  //const float rotationSpeed = 0.015f;

  //// Update the rotation constant
  //m_curRotationAngleRad += rotationSpeed;
  //if (m_curRotationAngleRad >= XM_2PI)
  //{
  //  m_curRotationAngleRad -= XM_2PI;
  //}

  //// Rotate the cube around the Y-axis
  //m_worldMatrix = XMMatrixRotationY(m_curRotationAngleRad);
}

// Main message handler for the sample.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  DXInterface* DX = reinterpret_cast<DXInterface*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

  switch (message)
  {
  case WM_CREATE: {
    LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
  } //WM_CREATE
                return 0;

  case WM_KEYDOWN:
    //OnKeyDown(static_cast<UINT8>(wParam));
    return 0;

  case WM_KEYUP:
    // OnKeyUp(static_cast<UINT8>(wParam));
    return 0;

  case WM_PAINT:
    //if (DX) {
    OnUpdate();
    OnRender(DX);
    //}; //IF DX 
    return 0;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, message, wParam, lParam);
} //WIndowProc

struct Window {
  Window(DXInterface* DX, pb::Config::Render* R) {
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = R->GetDirectXInstanceID();
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"DXInterface";
    RegisterClassEx(&windowClass);

    winRect = {};
    winRect.top = 0;
    winRect.left = 0;
    winRect.right = R->GetWindowWidth();
    winRect.bottom = R->GetWindowHeight();

    if (winRect.bottom | winRect.right) {
      RectToWorkArea(R);
    }; //If Dimentions aren't set

    winInstance = CreateWindow(
      windowClass.lpszClassName,
      (LPCWSTR)R->GetWindowName(),
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      winRect.right - winRect.left,
      winRect.bottom - winRect.top,
      nullptr,
      nullptr,
      R->GetDirectXInstanceID(),
      DX
    ); //CreateWindow
  }; //Window

  inline HWND GetWindowInstance() {
    return winInstance;
  }; // GetWindowInstance

  inline UINT GetWindowHeight() {
    GetWindowRect(winInstance, &winRect);

    return winRect.bottom;
  }; //GetWindowWidth

  inline UINT GetWindowWidth() {
    GetWindowRect(winInstance, &winRect);

    return winRect.right;
  }; //GetWindowWidth


private: 
  inline void RectToWorkArea(pb::Config::Render* R) {
    bool testVal = SystemParametersInfoA(SPI_GETWORKAREA, 0, &winRect, 0);

    R->SetWindowWidth(winRect.right);
    R->SetWindowHeight(winRect.bottom);

    if (testVal) {
    //  InternalReport(, , );
    }; //Failed
  }; //SetWinDimentions

  inline void SetRendConfVars(pb::Config::Render* R) {
    GetWindowRect(winInstance, &winRect);

    R->SetWindowWidth(winRect.right);
    R->SetWindowHeight(winRect.bottom);
  }; //GetWindowWidth

  RECT winRect;
  HWND winInstance;
}; //Window

PRIVATEPB::DirectX12::DirectX12(pb::Config::Render* R, Features* F, Control* C) {
  auto DX = new DXInterface();
  auto dxDevice = DX->GetDxDevice();
  auto cmdQueue = DX->GetCmdQueue();

  auto window = new Window(DX, R);
  auto winInstance = window->GetWindowInstance();

  //Create Descriptors
  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = MAX_FRAMES_IN_FLIGHT;
  swapChainDesc.Width = window->GetWindowWidth();
  swapChainDesc.Height = window->GetWindowHeight();
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  //Create SwapChain
  ComPtr<IDXGISwapChain1> swapChain;

  DXError = DX->factory->CreateSwapChainForHwnd(
    cmdQueue.Get(),
    winInstance, 
    &swapChainDesc,
    nullptr,
    nullptr,
    &swapChain
  );//CreateSwapChain 

  DXCheckError(
  "Creating Pipeline",
  "Creating Swapchain",
  "Swapchain Creating Failed"
  ); //DXCheckError

  DXError = 
    DX->factory->MakeWindowAssociation(winInstance, DXGI_MWA_NO_ALT_ENTER);
  
  DXCheckError(
    "Creating Pipeline",
    "Creating Swapchain",
    "Assigning to Window Failed"
  ); //DXCheckError

  DXError = swapChain.As(&m_swapChain);

  DXCheckError(
    "Creating Pipeline",
    "Creating Swapchain",
    "Assigning Swapchain Data"
  ); //DXCheckError

  m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

  // Create descriptor heaps.
    // Describe and create a render target view (RTV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = MAX_FRAMES_IN_FLIGHT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    DXError = dxDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

    DXCheckError(
      "Creating Pipeline",
      "Creating Descriptor Heaps",
      "Creation Failed"
    ); //DXCheckError

    // Describe and create a depth stencil view (DSV) descriptor heap.
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    DXError = dxDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));

    DXCheckError(
      "Creating Pipeline",
      "Creating Descriptor Heaps",
      "Creation Failed"
    ); //DXCheckError

    m_rtvDescriptorSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

  // Create frame resources.
  {
      CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    // Create a RTV and a command allocator for each frame.
    for (UINT n = 0; n < MAX_FRAMES_IN_FLIGHT; n++)
    {
      DXError = m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
      dxDevice->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
      rtvHandle.Offset(1, m_rtvDescriptorSize);

      //Change this with more knowledge
      //DXCheckError(
      //  "Creating Pipeline",
      //  "Creating Descriptor Heaps",
      //  "Creation Failed"
      //); //DXCheckError
    } //FOr Loop

    DXError = dxDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
  }

  // Create the command list.
  DXError = dxDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));

  DXCheckError(
    "Creating Pipeline",
    "Creating CmdList",
    "Creation Failed"
  ); //DXCheckError

  // Command lists are created in the recording state, but there is nothing
  // to record yet. The main loop expects it to be closed, so close it now.
  DXError = cmdList->Close();

  DXCheckError(
    "Creating Pipeline",
    "Closing",
    "Creation Failed"
  ); //DXCheckError

  //Sync Objects

    DXError = dxDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    UINT8 fenceCount = 1;

    // Create an event handle to use for frame synchronization.
    auto fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (fenceEvent == nullptr)
    {
      if (FAILED((HRESULT_FROM_WIN32(GetLastError())))) {};
    }

  ShowWindow(winInstance, R->GetDirectXCmdShow());

  // Main sample loop.
  MSG msg = {};
  while (msg.message != WM_QUIT) {
    // Process any messages in the queue.
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } //IF
  } //While

  // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
// sample illustrates how to use fences for efficient resource usage and to
// maximize GPU utilization.

  auto f = fenceValue;
// Signal and increment the fence value.
  DXError = cmdQueue->Signal(fence.Get(), f);
  
  DXCheckError(
    "Creating Pipeline",
    "Updating Fence",
    "Fence Update Failed"
  ); //DXCheckError
  
  fenceValue++;

  // Wait until the previous frame is finished.
  if (fence->GetCompletedValue() < f) {
    
    DXError = fence->SetEventOnCompletion(f, fenceEvent);

    DXCheckError(
      "Creating Pipeline",
      "Completing Fence",
      "Fence Completion Failed Failed"
    ); //DXCheckError

    WaitForSingleObject(fenceEvent, INFINITE);
  } //If Failed

  m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

  CloseHandle(fenceEvent);
}; //DIRECTX

PRIVATEPB::DirectX12::~DirectX12() {

};