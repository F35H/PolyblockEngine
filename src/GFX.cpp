#include <PRVTPB.h>

//CONFIG
struct PRIVATEPB::Config {
  Config() :
    Confirmed(0b00),
    WrotetoUtil(0b00),
    WrotetoRender(0b00),
    errBuffer(
      std::make_shared<
      std::vector<std::string>>())
  {}; //Config


  ~Config() {};


  //Getters
  std::shared_ptr<pb::Config::Utils> GetUtils() { return U; };
  std::shared_ptr<pb::Config::Render> GetRender() { return R; };
  bool GetConfirmed() { return Confirmed; };
  bool GetWrotetoUtil() { return WrotetoUtil; };
  bool GetWrotetoRender() { return WrotetoRender; };
  std::vector<std::string> GetErrBuff() { return *errBuffer.get(); };


  //Setters
  void SetUtils(std::shared_ptr<pb::Config::Utils> u) { U = u; };
  void SetRender(std::shared_ptr<pb::Config::Render> r) { R = r; };
  void SetConfirmed(bool b) { Confirmed = b; };
  void SetWrotetoUtil(bool b) { WrotetoUtil = b; };
  void SetWrotetoRender(bool b) { WrotetoRender = b; };

  void ExtndErrBuff(std::string str) {
    errBuffer->emplace_back(str);
  }; //ExtndErrBuffer

private:
  std::shared_ptr<std::vector<std::string>> errBuffer;
  std::shared_ptr<pb::Config::Utils> U; //Utils
  std::shared_ptr<pb::Config::Render> R; //Render


  bool Confirmed : 1;
  bool WrotetoUtil : 1;
  bool WrotetoRender : 1;



}; //Config


//CLIENT
struct PRIVATEPB::Client {
  std::shared_ptr<PRIVATEPB::Utils> Utils;

  std::shared_ptr<PRIVATEPB::Config> Conf =
    std::make_shared<PRIVATEPB::Config>();

  ~Client() {};

  void SetConfirmed(bool b) { Confirmed = b; };
  bool GetConfirmed(bool b) { return Confirmed; };

private:
  bool Confirmed;

}; //Client


struct PRIVATEPB::ClientVector {
  ClientVector() {
    vector = std::make_shared< std::vector
      <std::shared_ptr<PRIVATEPB::Client>>>();

    innerIndice -= 1;
    outerIndice -= 1;

    NewClient();
  }; //ClientVector


  //New
  void NewClient() {
    innerIndice += 1;
    outerIndice += 1;

    vector->emplace_back(
      std::make_shared<PRIVATEPB::Client>());
  }; //AddClient


  std::shared_ptr<PRIVATEPB::Utils> NewUtils() {
    return vector->operator[](innerIndice)->Utils =
      std::make_shared<PRIVATEPB::Utils>();
  }; //NewUtils


  std::shared_ptr<PRIVATEPB::Config> NewConfig() {
    return vector->operator[](innerIndice)->Conf =
      std::make_shared<PRIVATEPB::Config>();
  }; //NewConfig



  //Get Items
  std::shared_ptr<PRIVATEPB::Client> GetClient(UINT indice) {
    return vector->operator[](indice);
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Client> GetLatestClient() {
    return vector->operator[](innerIndice);
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Client> GetCurrentClient() {
    return vector->operator[](currentIndice);
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Config> GetCurrentConfig() {
    return vector->operator[](currentIndice)->Conf;
  }; //GetClient

  std::shared_ptr<PRIVATEPB::Config> GetLatestConfig() {
    return vector->operator[](innerIndice)->Conf;
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Utils> GetLatestUtils() {
    return vector->operator[](innerIndice)->Utils;
  }; //GetClient


  std::vector
    <std::shared_ptr<PRIVATEPB::Client>> GetClientVector() {
    return *vector.get();
  }; //GetClient


  //Set Items
  std::shared_ptr<PRIVATEPB::Utils> SetLatestUtils(
    std::shared_ptr<PRIVATEPB::Utils> U) {

    return vector->operator[](innerIndice)->Utils = U;
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Config> SetLatestConfig(
    std::shared_ptr<PRIVATEPB::Config> C) {

    return vector->operator[](innerIndice)->Conf = C;
  }; //GetClient


  ~ClientVector() {};


private:
  UINT currentIndice = 0;
  UINT innerIndice = 0;
  UINT outerIndice = 1;

  std::shared_ptr <
    std::vector<
    std::shared_ptr<PRIVATEPB::Client>
    >>  vector;

}; //ClientVector


struct PRIVATEPB::Utils {
  std::shared_ptr<std::ofstream> logFile;

  Utils() {
    std::string fileName = "gameLog";
    fileName += ".txt";

    logFile = std::move(
      std::shared_ptr<std::ofstream>(new std::ofstream(
        fileName, std::ios::ate | std::ios::out)));

    int i = 0;
    auto cout = PRIVATEPB::Client_ptr->GetLatestConfig()->GetUtils()->GetLogBuffer();
    const char* errMsg;

    do { //Create Log File
      switch (i) {
      default:
        errMsg = "Attempting Log File Creation | File Creation Imminent | Attempting Once \n";
        cout->write(errMsg, strlen(errMsg));
        logFile->write(errMsg, strlen(errMsg));
        break;

      case 0b01:
        errMsg = "Log File Creation Failed | File Creation Primary Attempt | Attempting Twice \n";
        cout->write(errMsg, strlen(errMsg));
        break;

      case 0b10:
        errMsg = "Log File Creation Failed | File Creation Secondary Attempt | Attempting Trice \n";
        cout->write(errMsg, strlen(errMsg));
        break;

      case 0b11:
        errMsg = "Log File Creation Failed | File Creation Tertiary Attempt | Termination Imminent \n";
        cout->write(errMsg, strlen(errMsg));
        abort();

      }; //Switch

      ++i;
    } while (!logFile->is_open());

  }; //UTILSCONSTRUCTOR

}; //Utils

class GLFW {

protected:
  GLFWmonitor* mon;
  GLFWwindow* win;
  const GLFWvidmode* vidMode;
  INT winXPos;
  INT winYPos;
  INT monWidth;
  INT monHeight;
  std::shared_ptr<pb::Config::Render> rendConf;
  
  GLFW() {
    pb::Utils::Output::WritetoTimedLog(
      "Creating Window | Initializing GLFW | Initializing");

    if (!glfwInit()){
      throw std::runtime_error(
        "Creating Window | Initializing GLFW | Initialization Failed");
    }

    pb::Utils::Output::WritetoTimedLog(
      "Creating Window | Creating Monitor | Collecting Stats");

    mon = glfwGetPrimaryMonitor();
    vidMode = glfwGetVideoMode(mon);

    glfwGetMonitorWorkarea(mon, &winXPos, &winYPos, &monWidth, &monHeight);


  }; //GLFW

  void createWindow(FINT8 winWidth, FINT8 winHeight, const char* winName) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    pb::Utils::Output::WritetoTimedLog(
      "Creating Window | Creating Window | Creating Window");

    win = glfwCreateWindow(winWidth, winHeight, winName, mon, nullptr);

    if (!win) {
      throw std::runtime_error(
        "Creating Window | Creating Window | Window Creation Failed");
    }
  }; //createWindow

  void createWindow(const char* winName) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    pb::Utils::Output::WritetoTimedLog(
      "Creating Window | Creating Window | Creating Window");

    win = glfwCreateWindow(monWidth, monHeight, winName, mon, nullptr);

    if (!win) {
      throw std::runtime_error(
        "Creating Window | Creating Window | Window Creation Failed");
    }

    rendConf->SetWindowHeight(monHeight);
    rendConf->SetWindowWidth(monWidth);
  }; //createWindow

}; //GLFW


struct Vulkan :
  GLFW {

  //Update debugCallback

  class VulkanInit {
    VkInstance instance;
    VkSurfaceKHR surface;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkDevice device;

    struct SwapChainSupportDetails {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices {
      std::optional<uint32_t> graphicsFamily;
      std::optional<uint32_t> presentFamily;

      bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
      }
    };

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
      std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

      return VK_FALSE;
    } //CallBack

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
      createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      createInfo.pfnUserCallback = debugCallback;
    } //DebugMessenger

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
      SwapChainSupportDetails details;

      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

      uint32_t formatCount;
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

      if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
      }

      uint32_t presentModeCount;
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

      if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
      }

      return details;
    }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
      QueueFamilyIndices indices;

      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

      int i = 0;
      for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
          indices.presentFamily = i;
        }

        if (indices.isComplete()) {
          break;
        }

        i++;
      }

      return indices;
    }

    const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    void CheckDebug() {
      if (DEBUG) {
        pb::Utils::Output::WritetoTimedLog(
          "Initializing Vulkan | Confirming Validation Layers | Enumerating and Confirming Layers");

        UINT32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
          bool layerFound = false;

          for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
              layerFound = true;
              break;
            }
          }

          if (!layerFound) {
            throw std::runtime_error("Initializing Vulkan | Confirming Validation Layers | Failure");
          }
        }
      }; // if (DEBUG)
    }; //checkDebug
    
    void CreateInstance(const char* appName) {
      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Creating Program Instance | Writing Struct");

      VkApplicationInfo appInfo{};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = appName;
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName = "PolyBlock Engine";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.apiVersion = VK_API_VERSION_1_0;

      VkInstanceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;


      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Creating Program Instance | Collecting Extensions");

      UINT32 glfwExtensionCount = 0;
      const char** glfwExtensions;
      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

      if (DEBUG) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      }

      createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
      createInfo.ppEnabledExtensionNames = extensions.data();


      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Creating Program Instance | Collecting Vulkan Debug Data");

      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
      if (DEBUG) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
      }
      else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
      }


      pb::Utils::Output::WritetoTimedLog(
        "Creating Program Instance | Creating Virtual Instance | Appoving Instance");

      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Creating Program Instance | Creating Virtual Instance | Instance Creation Failed");
      }
    }; //createInstance
    
    void SetupDebugMessenger() {
      if (DEBUG) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        pb::Utils::Output::WritetoTimedLog(
          "Initializing Vulkan | Creating Debug Messenger | Creating Debugger");

        auto b = VK_ERROR_EXTENSION_NOT_PRESENT;

        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
          b = func(instance, &createInfo, nullptr, &debugMessenger);
        }

        pb::Utils::Output::WritetoTimedLog(
          "Initializing Vulkan | Confirming Debugger | Confirming Debugger");

        if (b != VK_SUCCESS) {
          throw std::runtime_error(
            "Initializing Vulkan | Confirming Debugger | Confirmation Failed");
        
        } // If CeateMessenger
      }; //If Debug
    }; //setupDebugMessenger

    void CreateSurface(GLFWwindow* win) {
      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Creating Window Surface | Creating and Confirming");

      if (glfwCreateWindowSurface(instance, win, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error(
          "Initializing Vulkan | Creating Window Surface | Creation Failed");
      } //If Surface
    }; //Create Surface

    void PickPhysicalDevice() {
      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Picking Physical Device | Enumerating GPUs");

      UINT32 deviceCount = 0;
      vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

      if (deviceCount == 0) {
        throw std::runtime_error(
          "Initializing Vulkan | Picking Physical Device | No GPU Support Found");
      }

      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Picking Physical Device | Testing Pipelines");

      for (const auto& device : devices) {
        QueueFamilyIndices indices = findQueueFamilies(device);

        UINT32 extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
          requiredExtensions.erase(extension.extensionName);
        }

        bool swapChainAdequate = false;
        if (requiredExtensions.empty()) {
          SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
          swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        if (indices.isComplete() && requiredExtensions.empty() && swapChainAdequate){
          physicalDevice = device;
          break;
        }
      }

      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Picking Physical Device | Confirming Pipeline Support");

      if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error(
          "Initializing Vulkan | Picking Physical Device | No Pipeline Support Found");
      }
    }; //pickPhysicalDevice

    void CreateLogicalDevice() {
      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Creating Device Interface | Queueing Devices");

        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        pb::Utils::Output::WritetoTimedLog(
          "Initializing Vulkan | Creating Device Interface | Staging Info");

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (DEBUG) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        pb::Utils::Output::WritetoTimedLog(
          "Initializing Vulkan | Creating Device Interface | Confirming Virtual Interface");

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        pb::Utils::Output::WritetoTimedLog(
          "Initializing Vulkan | Creating Device Interface | Applying Interface to Device");

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    }; //createLogicalDevice
  
    friend class Vulkan;
  }; //VulkanInit


  class VulkanPipe {
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
  }; //VulkanPipe

  class VulkanCmnd {
    void createCommandPool();
    void createDepthResources();
    void createFramebuffers();
    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void createCommandBuffers();
    void createSyncObjects();
  }; //VulkanCmnd

  Vulkan(std::shared_ptr<pb::Config::Render> RendConf) {
    GLFW::rendConf = RendConf;

    pb::Utils::Output::WritetoTimedLog(
      "Creating Window | Selecting Window | Collecting Configurations");

    FINT16 winHeight = rendConf->GetWindowHeight();
    FINT16 winWidth = rendConf->GetWindowWidth();
    const char* winName = rendConf->GetWindowName();

    if (winHeight & winWidth) {
      createWindow(winHeight,winWidth, winName);
    }
    else {
      createWindow(winName);
    } //Else

    Vulkan::VulkanInit* VInit = new VulkanInit();
    //Vulkan::VulkanPipe* VInit = new VulkanPipe();
    //Vulkan::VulkanCmnd* VInit = new VulkanCmnd();


    switch (rendConf->GetRenderEngine()) {
    case VULKAN13:
    default:
      VInit->CheckDebug();
      VInit->CreateInstance(rendConf->GetWindowName());
      VInit->SetupDebugMessenger();
      VInit->CreateSurface(GLFW::win);
      VInit->PickPhysicalDevice();
      VInit->CreateLogicalDevice();

      //VPipe->createSwapChain();
      //VPipe->createImageViews();
      //VPipe->createRenderPass();
      //VPipe->createDescriptorSetLayout();
      //VPipe->createGraphicsPipeline();

      //VCmnd->createCommandPool();
      //VCmnd->createDepthResources();
      //VCmnd->createFramebuffers();
      //VCmnd->createTextureImage();
      //VCmnd->createTextureImageView();
      //VCmnd->createTextureSampler();
      //VCmnd->createVertexBuffer();
      //VCmnd->createIndexBuffer();
      //VCmnd->createUniformBuffers();
      //VCmnd->createDescriptorPool();
      //VCmnd->createDescriptorSets();
      //VCmnd->createCommandBuffers();
      //VCmnd->createSyncObjects();
      break;
    } //Switch

  }; //Vulkan Constructor

}; //Vulkan

class DirectX {

}; //DirectX

class OpenGL {

}; //OpenGL

struct PRIVATEPB::GFX {
  union API {
    Vulkan V;
    DirectX D;
    OpenGL O;

    API() {};
  }; //Union

  GFX() {
    UINT rend = 1;

    auto Client = PRIVATEPB::Client_ptr->GetCurrentClient();
    auto RendConf = Client->Conf->GetRender();

    API* rendClass = new API();

    pb::Utils::Output::WritetoTimedLog(
      "Initializing Render Engine | Selecting Engine | Beginning Render Segments");

    while (rend) {
      try {
        rend = RendConf->GetRenderEngine();

        switch (rend)
        {
        case VULKAN13:
          rendClass->V = Vulkan(RendConf);
          break;

        default:
          pb::Utils::Output::WritetoTimedLog(
            "Initializing Render Engine | Selecting Engine | All Possible Renders Failed!");
          pb::Utils::Output::FlushtoLog();
          terminate();

        } //Switch
      } //Try

      catch (const std::exception& exc) {
        pb::Utils::Output::WritetoTimedLog(exc.what());
        rend -= 1;

        Client->Conf
          ->GetRender()
          ->SetRenderEngine(rend);
      } //Catch

      catch (...) {
        pb::Utils::Output::WritetoTimedLog("Unknown Exception Triggered");
        rend -= 1;

        Client->Conf
          ->GetRender()
          ->SetRenderEngine(rend);
      } //Catch
    } //While
  };
};

inline void pb::RunRender() {
  PRIVATEPB::GFX();
}; //RunRender


void pb::Config::Render::SetRenderEngine(UINT renderEngine) { RenderEngine = renderEngine; };
void pb::Config::Render::SetWindowWidth(UINT windowWidth) { WindowWidth = windowWidth; };
void pb::Config::Render::SetWindowHeight(UINT windowHeight) { WindowHeight = windowHeight; };
void pb::Config::Render::SetWindowName(const char* windowName) { WindowName = windowName; };
void pb::Config::Render::SetAppVersion(const char* version) { AppVersion = version; };

UINT pb::Config::Render::GetRenderEngine() { return RenderEngine; };
UINT pb::Config::Render::GetWindowWidth() { return WindowWidth; };
UINT pb::Config::Render::GetWindowHeight() { return WindowHeight; };
const char* pb::Config::Render::GetWindowName() { return WindowName; };
const char* pb::Config::Render::GetAppVersion() { return AppVersion; };