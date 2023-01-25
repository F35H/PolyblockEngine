#include <PRVTPB.h>

//CONFIG
struct PRIVATEPB::Config {
  Config() :
    Confirmed(0b00),
    WrotetoUtil(0b00),
    WrotetoRender(0b00),
    errBuffer(
      new std::vector < const char*>())
  {}; //Config

  //Getters
  pb::Config::Utils* GetUtils() { return U; };
  pb::Config::Render* GetRender() { return R; };
  bool GetConfirmed() { return Confirmed; };
  bool GetWrotetoUtil() { return WrotetoUtil; };
  bool GetWrotetoRender() { return WrotetoRender; };
  std::vector<const char*> GetErrBuff() { return *errBuffer; };


  //Setters
  void SetUtils(pb::Config::Utils* u) { U = u; };
  void SetRender(pb::Config::Render* r) { R = r; };
  void SetConfirmed(bool b) { Confirmed = b; };
  void SetWrotetoUtil(bool b) { WrotetoUtil = b; };
  void SetWrotetoRender(bool b) { WrotetoRender = b; };

  void ExtndErrBuff(const char* str) {
    errBuffer->emplace_back(str);
  }; //ExtndErrBuffer

  ~Config() {
    delete U;
    delete R;
    delete errBuffer;
  }; //Config

private:
  std::vector<const char*>* errBuffer;
  pb::Config::Utils* U; //Utils
  pb::Config::Render* R; //Render


  bool Confirmed : 1;
  bool WrotetoUtil : 1;
  bool WrotetoRender : 1;

}; //Config


//CLIENT
struct PRIVATEPB::Client {
  PRIVATEPB::Utils* Utils;

  PRIVATEPB::Config* Conf =
    new PRIVATEPB::Config();

  void SetConfirmed(bool b) { Confirmed = b; };
  bool GetConfirmed(bool b) { return Confirmed; };

private:
  bool Confirmed;

  ~Client() {
    delete Utils;
    delete Conf;
  }; //Client

}; //Client

struct PRIVATEPB::Utils {
  std::ofstream* logFile;

  Utils(pb::Config::Utils* U) {
    std::string fileName = "gameLog";
    fileName += ".txt";

    logFile = new std::ofstream(
      fileName, std::ios::ate | std::ios::out);

    int i = 0;
    auto cout = U->GetLogBuffer();
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

  ~Utils() {
    delete logFile;
  }; //Utils

}; //Utils

struct PRIVATEPB::ClientVector {
  ClientVector() {
    vector = new std::vector<PRIVATEPB::Client*>();

    innerIndice -= 1;
    outerIndice -= 1;

    NewClient();
  }; //ClientVector


  //New
  void NewClient() {
    innerIndice += 1;
    outerIndice += 1;

    vector->emplace_back(
      new PRIVATEPB::Client());
  }; //AddClient


  PRIVATEPB::Utils* NewUtils() {
    return vector->operator[](innerIndice)->Utils =
      new PRIVATEPB::Utils(
        GetLatestConfig()
        ->GetUtils());
  }; //NewUtils


  PRIVATEPB::Config* NewConfig() {
    return vector->operator[](innerIndice)->Conf =
      new PRIVATEPB::Config();
  }; //NewConfig



  //Get Items
  PRIVATEPB::Client* GetClient(UINT indice) {
    return vector->operator[](indice);
  }; //GetClient


  PRIVATEPB::Client* GetLatestClient() {
    return vector->operator[](innerIndice);
  }; //GetClient


  PRIVATEPB::Client* GetCurrentClient() {
    return vector->operator[](currentIndice);
  }; //GetClient


  PRIVATEPB::Config* GetCurrentConfig() {
    return vector->operator[](currentIndice)->Conf;
  }; //GetClient

  PRIVATEPB::Config* GetLatestConfig() {
    return vector->operator[](innerIndice)->Conf;
  }; //GetClient


  PRIVATEPB::Utils* GetLatestUtils() {
    return vector->operator[](innerIndice)->Utils;
  }; //GetClient


  std::vector
    <PRIVATEPB::Client*> GetClientVector() {
    return *vector;
  }; //GetClient


  //Set Items
  PRIVATEPB::Utils* SetLatestUtils(
    PRIVATEPB::Utils* U) {

    return vector->operator[](innerIndice)->Utils = U;
  }; //GetClient


  PRIVATEPB::Config* SetLatestConfig(
    PRIVATEPB::Config* C) {

    return vector->operator[](innerIndice)->Conf = C;
  }; //GetClient


  ~ClientVector() {
    delete vector;
  }; //ClientVector


private:
  UINT currentIndice = 0;
  UINT innerIndice = 0;
  UINT outerIndice = 1;

  std::vector<PRIVATEPB::Client*>* vector;

}; //ClientVector



class GLFW {
protected:
  struct GLFWData {
  }; //
  
  GLFWmonitor* mon;
  GLFWwindow* win;
  const GLFWvidmode* vidMode;
  INT winXPos;
  INT winYPos;
  INT monWidth;
  INT monHeight;
  pb::Config::Render* rendConf;
  
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

    glfwGetMonitorWorkarea(
      mon, &winXPos, &winYPos,
      &monWidth, &monHeight);
  }; //GLFW

  void createWindow(FINT16 winWidth, FINT16 winHeight, const char* winName) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    pb::Utils::Output::WritetoTimedLog(
      "Creating Window | Creating Window | Creating Window");

    if (rendConf->GetFullscreenBool()) {
      win = glfwCreateWindow(winWidth, winHeight, winName, mon, nullptr);
    }
    else {
      win = glfwCreateWindow(winWidth, winHeight, winName, mon, nullptr);
    } //ifFullscreen

    if (!win) {
      throw std::runtime_error(
        "Creating Window | Creating Window | Window Creation Failed");
    }
  }; //createWindow

  void createWindow(const char* winName) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    pb::Utils::Output::WritetoTimedLog(
      "Creating Window | Creating Window | Creating Window");

    if (rendConf->GetFullscreenBool()) {
      win = glfwCreateWindow(monWidth, monHeight, winName, mon, nullptr);
    }
    else {
      win = glfwCreateWindow(monWidth, monHeight, winName, mon, nullptr);
    } //ifFullscreen

    if (!win) {
      throw std::runtime_error(
        "Creating Window | Creating Window | Window Creation Failed");
    }

    rendConf->SetWindowHeight(monHeight);
    rendConf->SetWindowWidth(monWidth);
  }; //createWindow

  ~GLFW() {};

}; //GLFW



namespace Vulkan{
  class Comm {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkSurfaceKHR surface;
    VkDevice device;

    struct SwapChainSupportDetails {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
    }; //SwapChainSupportDetails

    struct QueueFamilyIndices {
      std::optional<uint32_t> graphicsFamily;
      std::optional<uint32_t> presentFamily;

      bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
      } //isComplete
    }; //QueueFamilyIndices

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) {
      Comm::QueueFamilyIndices indices;

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
    } //QueueFamilyIndices

    //Functions
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const {
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
    } //SwapChainSupportDetails

    ~Comm() {
      delete physicalDevice;
      delete surface;
    }; //FindQueueFamilies

    friend class Center;
    friend class Init;
    friend class Pipe;
    friend class Cmnd;
  }; //comm

  class Init {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    Comm* Cmn;

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

    const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
    }; //ValidationLayers

    const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
    }; //deviceExtensions

    Init(Comm* C) : Cmn(C) {};

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

    void CreateInstance(pb::Config::Render* rendConf) {
      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Creating Program Instance | Writing Struct");

      VkApplicationInfo appInfo{};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = rendConf->GetWindowName();
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

    void CreateSurface(GLFWwindow* win, VkSurfaceKHR* surface) {
      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Creating Window Surface | Creating and Confirming");

      if (glfwCreateWindowSurface(instance, win, nullptr, surface) != VK_SUCCESS) {
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
        Comm::QueueFamilyIndices indices = Cmn->FindQueueFamilies(device);

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
          Comm::SwapChainSupportDetails swapChainSupport = Cmn->querySwapChainSupport(device);
          swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        if (indices.isComplete() && requiredExtensions.empty() && swapChainAdequate) {
          Cmn->physicalDevice = device;
          break;
        }
      }

      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Picking Physical Device | Confirming Pipeline Support");

      if (Cmn->physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error(
          "Initializing Vulkan | Picking Physical Device | No Pipeline Support Found");
      }
    }; //pickPhysicalDevice

    void CreateLogicalDevice() {
      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Creating Device Interface | Queueing Devices");

      Comm::QueueFamilyIndices indices = Cmn->FindQueueFamilies(Cmn->physicalDevice);

      std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
      std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

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
      }
      else {
        createInfo.enabledLayerCount = 0;
      }

      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Creating Device Interface | Confirming Virtual Interface");

      if (vkCreateDevice(Cmn->physicalDevice, &createInfo, nullptr, &Cmn->device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
      }

      pb::Utils::Output::WritetoTimedLog(
        "Initializing Vulkan | Creating Device Interface | Applying Interface to Device");

      vkGetDeviceQueue(Cmn->device, indices.graphicsFamily.value(), 0, &graphicsQueue);
      vkGetDeviceQueue(Cmn->device, indices.presentFamily.value(), 0, &presentQueue);
    }; //createLogicalDevice

    friend class Center;
  }; //VulkanInit

  class Pipe {
    Comm* Cmn;
    VkSwapchainKHR swapChain;
    VkRenderPass renderPass;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    struct Vertex {
      glm::vec2 pos;
      glm::vec3 color;

      static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
      }

      static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
      } //std::Array
    }; //Vertex

    static std::vector<char> readFile(const std::string& filename) {
      std::ifstream file(filename, std::ios::ate | std::ios::binary);

      if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
      }

      size_t fileSize = (size_t)file.tellg();
      std::vector<char> buffer(fileSize);

      file.seekg(0);
      file.read(buffer.data(), fileSize);

      file.close();

      return buffer;
    }

    Pipe(Comm* C) : Cmn(C) {}

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
      for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
          return availableFormat;
        }
      }

      return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
      for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
          return availablePresentMode;
        }
      }

      return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
      if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
      }
      else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
      }
    }

    VkShaderModule createShaderModule(const std::vector<char>& code) {
      VkShaderModuleCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      createInfo.codeSize = code.size();
      createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

      VkShaderModule shaderModule;
      if (vkCreateShaderModule(Cmn->device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
      }

      return shaderModule;
    }

    void CreateSwapChain() {
      Comm::SwapChainSupportDetails swapChainSupport = Cmn->querySwapChainSupport(Cmn->physicalDevice);

      VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
      VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
      VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

      uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
      if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
      }

      VkSwapchainCreateInfoKHR createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      createInfo.surface = Cmn->surface;

      createInfo.minImageCount = imageCount;
      createInfo.imageFormat = surfaceFormat.format;
      createInfo.imageColorSpace = surfaceFormat.colorSpace;
      createInfo.imageExtent = extent;
      createInfo.imageArrayLayers = 1;
      createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

      Comm::QueueFamilyIndices indices = Cmn->FindQueueFamilies(Cmn->physicalDevice);
      uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

      if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
      }
      else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      }

      createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
      createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      createInfo.presentMode = presentMode;
      createInfo.clipped = VK_TRUE;

      if (vkCreateSwapchainKHR(Cmn->device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
      }

      vkGetSwapchainImagesKHR(Cmn->device, swapChain, &imageCount, nullptr);
      swapChainImages.resize(imageCount);
      vkGetSwapchainImagesKHR(Cmn->device, swapChain, &imageCount, swapChainImages.data());

      swapChainImageFormat = surfaceFormat.format;
      swapChainExtent = extent;
    }; //CreateSwapChain

    void CreateImageViews() {
      swapChainImageViews.resize(swapChainImages.size());

      for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(Cmn->device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
          throw std::runtime_error("failed to create image views!");
        }
      }
    }; //CreateImageView

    void CreateRenderPass() {
      VkAttachmentDescription colorAttachment{};
      colorAttachment.format = swapChainImageFormat;
      colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
      colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

      VkAttachmentReference colorAttachmentRef{};
      colorAttachmentRef.attachment = 0;
      colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      VkSubpassDescription subpass{};
      subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpass.colorAttachmentCount = 1;
      subpass.pColorAttachments = &colorAttachmentRef;

      VkSubpassDependency dependency{};
      dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
      dependency.dstSubpass = 0;
      dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependency.srcAccessMask = 0;
      dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

      VkRenderPassCreateInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      renderPassInfo.attachmentCount = 1;
      renderPassInfo.pAttachments = &colorAttachment;
      renderPassInfo.subpassCount = 1;
      renderPassInfo.pSubpasses = &subpass;
      renderPassInfo.dependencyCount = 1;
      renderPassInfo.pDependencies = &dependency;

      if (vkCreateRenderPass(Cmn->device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
      }
    }; //CreateRenderPass

    void CreateDescriptorSetLayout() {
      VkDescriptorSetLayoutBinding uboLayoutBinding{};
      uboLayoutBinding.binding = 0;
      uboLayoutBinding.descriptorCount = 1;
      uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      uboLayoutBinding.pImmutableSamplers = nullptr;
      uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

      VkDescriptorSetLayoutCreateInfo layoutInfo{};
      layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      layoutInfo.bindingCount = 1;
      layoutInfo.pBindings = &uboLayoutBinding;

      if (vkCreateDescriptorSetLayout(Cmn->device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
      }
    }; //CreateDescriptorLayout

    void CreateGraphicsPipeline() {
      auto vertShaderCode = readFile("shaders/vert.spv");
      auto fragShaderCode = readFile("shaders/frag.spv");

      VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
      VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

      VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
      vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
      vertShaderStageInfo.module = vertShaderModule;
      vertShaderStageInfo.pName = "main";

      VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
      fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      fragShaderStageInfo.module = fragShaderModule;
      fragShaderStageInfo.pName = "main";

      VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

      VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
      vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

      auto bindingDescription = Vertex::getBindingDescription();
      auto attributeDescriptions = Vertex::getAttributeDescriptions();

      vertexInputInfo.vertexBindingDescriptionCount = 1;
      vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
      vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
      vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

      VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
      inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
      inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
      inputAssembly.primitiveRestartEnable = VK_FALSE;

      VkPipelineViewportStateCreateInfo viewportState{};
      viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      viewportState.viewportCount = 1;
      viewportState.scissorCount = 1;

      VkPipelineRasterizationStateCreateInfo rasterizer{};
      rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
      rasterizer.depthClampEnable = VK_FALSE;
      rasterizer.rasterizerDiscardEnable = VK_FALSE;
      rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
      rasterizer.lineWidth = 1.0f;
      rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
      rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
      rasterizer.depthBiasEnable = VK_FALSE;

      VkPipelineMultisampleStateCreateInfo multisampling{};
      multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      multisampling.sampleShadingEnable = VK_FALSE;
      multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

      VkPipelineColorBlendAttachmentState colorBlendAttachment{};
      colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
      colorBlendAttachment.blendEnable = VK_FALSE;

      VkPipelineColorBlendStateCreateInfo colorBlending{};
      colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      colorBlending.logicOpEnable = VK_FALSE;
      colorBlending.logicOp = VK_LOGIC_OP_COPY;
      colorBlending.attachmentCount = 1;
      colorBlending.pAttachments = &colorBlendAttachment;
      colorBlending.blendConstants[0] = 0.0f;
      colorBlending.blendConstants[1] = 0.0f;
      colorBlending.blendConstants[2] = 0.0f;
      colorBlending.blendConstants[3] = 0.0f;

      std::vector<VkDynamicState> dynamicStates = {
          VK_DYNAMIC_STATE_VIEWPORT,
          VK_DYNAMIC_STATE_SCISSOR
      };
      VkPipelineDynamicStateCreateInfo dynamicState{};
      dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
      dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
      dynamicState.pDynamicStates = dynamicStates.data();

      VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = 1;
      pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

      if (vkCreatePipelineLayout(Cmn->device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
      }

      VkGraphicsPipelineCreateInfo pipelineInfo{};
      pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      pipelineInfo.stageCount = 2;
      pipelineInfo.pStages = shaderStages;
      pipelineInfo.pVertexInputState = &vertexInputInfo;
      pipelineInfo.pInputAssemblyState = &inputAssembly;
      pipelineInfo.pViewportState = &viewportState;
      pipelineInfo.pRasterizationState = &rasterizer;
      pipelineInfo.pMultisampleState = &multisampling;
      pipelineInfo.pColorBlendState = &colorBlending;
      pipelineInfo.pDynamicState = &dynamicState;
      pipelineInfo.layout = pipelineLayout;
      pipelineInfo.renderPass = renderPass;
      pipelineInfo.subpass = 0;
      pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

      if (vkCreateGraphicsPipelines(Cmn->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
      }

      vkDestroyShaderModule(Cmn->device, fragShaderModule, nullptr);
      vkDestroyShaderModule(Cmn->device, vertShaderModule, nullptr);
    }; //CreateGraphicsPipeline
    
    friend class Center;
  }; //VulkanPipe

  class Cmnd {
    Comm* Cmn;

    Cmnd(Comm* C) : Cmn(C) {};

    void CreateFramebuffers() {
      swapChainFramebuffers.resize(swapChainImageViews.size());

      for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
          throw std::runtime_error("failed to create framebuffer!");
        }
      }
    }; //CreateFrameBuffers

    void CreateCommandPool() {
      QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

      VkCommandPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
      poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

      if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
      }
    }; //CreateCommandPool

    void CreateTextureImage() {
      int texWidth, texHeight, texChannels;
      stbi_uc* pixels = stbi_load("textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
      VkDeviceSize imageSize = texWidth * texHeight * 4;

      if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
      }

      VkBuffer stagingBuffer;
      VkDeviceMemory stagingBufferMemory;
      createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

      void* data;
      vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
      memcpy(data, pixels, static_cast<size_t>(imageSize));
      vkUnmapMemory(device, stagingBufferMemory);

      stbi_image_free(pixels);

      createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

      transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
      transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

      vkDestroyBuffer(device, stagingBuffer, nullptr);
      vkFreeMemory(device, stagingBufferMemory, nullptr);
    }; //CreateTextureImage

    void CreateVertexBuffer() {
      VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

      VkBuffer stagingBuffer;
      VkDeviceMemory stagingBufferMemory;
      createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

      void* data;
      vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
      memcpy(data, vertices.data(), (size_t)bufferSize);
      vkUnmapMemory(device, stagingBufferMemory);

      createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

      copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

      vkDestroyBuffer(device, stagingBuffer, nullptr);
      vkFreeMemory(device, stagingBufferMemory, nullptr);
    }; //CreateVertexBuffer

    void CreateIndexBuffer() {
      VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

      VkBuffer stagingBuffer;
      VkDeviceMemory stagingBufferMemory;
      createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

      void* data;
      vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
      memcpy(data, indices.data(), (size_t)bufferSize);
      vkUnmapMemory(device, stagingBufferMemory);

      createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

      copyBuffer(stagingBuffer, indexBuffer, bufferSize);

      vkDestroyBuffer(device, stagingBuffer, nullptr);
      vkFreeMemory(device, stagingBufferMemory, nullptr);
    }; //CreateIndexBuffer

    void CreateUniformBuffers() {
      VkDeviceSize bufferSize = sizeof(UniformBufferObject);

      uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
      uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
      uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

      for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
      }
    }; //CreateUniformBuffer

    void CreateDescriptorPool() {
      VkDescriptorPoolSize poolSize{};
      poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

      VkDescriptorPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      poolInfo.poolSizeCount = 1;
      poolInfo.pPoolSizes = &poolSize;
      poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

      if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
      }
    }; //CreateDescriptorPool

    void CreateDescriptorSets() {
      std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
      VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = descriptorPool;
      allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
      allocInfo.pSetLayouts = layouts.data();

      descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
      if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
      }

      for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
      }
    }; //CreateDescriptorSets

    void CreateCommandBuffers() {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
      }

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = renderPass;
      renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
      renderPassInfo.renderArea.offset = { 0, 0 };
      renderPassInfo.renderArea.extent = swapChainExtent;

      VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
      renderPassInfo.clearValueCount = 1;
      renderPassInfo.pClearValues = &clearColor;

      vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

      VkViewport viewport{};
      viewport.x = 0.0f;
      viewport.y = 0.0f;
      viewport.width = (float)swapChainExtent.width;
      viewport.height = (float)swapChainExtent.height;
      viewport.minDepth = 0.0f;
      viewport.maxDepth = 1.0f;
      vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

      VkRect2D scissor{};
      scissor.offset = { 0, 0 };
      scissor.extent = swapChainExtent;
      vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

      VkBuffer vertexBuffers[] = { vertexBuffer };
      VkDeviceSize offsets[] = { 0 };
      vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

      vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

      vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

      vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

      vkCmdEndRenderPass(commandBuffer);

      if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
      }
    }; //CreateCommandBuffers

    void CreateSyncObjects() {
      imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
      renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
      inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

      VkSemaphoreCreateInfo semaphoreInfo{};
      semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      VkFenceCreateInfo fenceInfo{};
      fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
          vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
          vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
          throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
      }
    }; //CreateSyncObjects

    friend class Vulkan;
  }; //VulkanCmnd

  struct Center :
    GLFW {

    Init* VInit;
    Pipe* VPipe;
    Cmnd* VCmnd;
    Comm* Cmn;

    //Update debugCallback

    Center(pb::Config::Render* RendConf) {
      GLFW::rendConf = RendConf;

      pb::Utils::Output::WritetoTimedLog(
        "Creating Window | Selecting Window | Collecting Configurations");

      FINT16 winHeight = RendConf->GetWindowHeight();
      FINT16 winWidth = RendConf->GetWindowWidth();
      const char* winName = RendConf->GetWindowName();

      if (winHeight && winWidth) {
        GLFW::createWindow(winHeight, winWidth, winName);
      }
      else {
        GLFW::createWindow(winName);
      } //Else
    
      Cmn = new Comm();
      VInit = new Init(Cmn);
      VPipe = new Pipe(Cmn);
      VCmnd = new Cmnd(Cmn);
    }; //Vulkan Constructor

    void Vulkan13() {
      VInit->CheckDebug();
      VInit->CreateInstance(GLFW::rendConf);
      VInit->SetupDebugMessenger();
      VInit->CreateSurface(GLFW::win);
      VInit->PickPhysicalDevice();
      VInit->CreateLogicalDevice();

      VPipe->CreateSwapChain();
      VPipe->CreateImageViews();
      VPipe->CreateRenderPass();
      VPipe->CreateDescriptorSetLayout();
      VPipe->CreateGraphicsPipeline();

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
    }; //Vulkan13

    ~Center() {
      delete VInit;
      delete VPipe;
      delete VCmnd;
      delete Cmn;
    }; //Center

  }; //Vulkan
}



class DirectX {

}; //DirectX

class OpenGL {

}; //OpenGL

struct PRIVATEPB::GFX {
  union API {
    Vulkan* V;
    DirectX* D;
    OpenGL* O;

    ~API(){};
  }; //Union

  API* rendClass = new API();

  GFX() {
    auto Client = PRIVATEPB::Client_ptr->GetCurrentClient();
    auto RendConf = Client->Conf->GetRender();
    
    UINT rend = RendConf->GetRenderEngine();
    bool init = 0;


    pb::Utils::Output::WritetoTimedLog(
      "Initializing Render Engine | Selecting Engine | Beginning Render Segments");

    //Rewritting Try-Catch Statement
    try {
      while (rend) {
        if (!init) {
          switch (rend) {
          case VULKAN13:
            rendClass->V = new Vulkan(RendConf);
            rend = true;
            break;
          } //Switch
        } //If init
        
        switch (rend) {
        case VULKAN13:
          rendClass->V->Vulkan13();
          break;

        default:
          pb::Utils::Output::WritetoTimedLog(
            "Initializing Render Engine | Selecting Engine | All Possible Renders Failed!");
          pb::Utils::Output::FlushtoLog();
          terminate();

        } //Switch
      } //While
    } //Try
    catch (const std::exception& exc) {
      pb::Utils::Output::WritetoTimedLog(exc.what());
      
      if (rend == VULKAN13) {
        delete rendClass->V;
        init = false;
      } //if (rend == Vulkan13)

      rend -= 1;

      Client->Conf
        ->GetRender()
        ->SetRenderEngine(rend);
    } //Catch

    catch (...) {
      pb::Utils::Output::WritetoTimedLog(
        "Unknown Exception Triggered");

      if (rend == VULKAN13) {
        delete rendClass->V;
        init = false;
      } //if (rend == Vulkan13)

      Client->Conf
        ->GetRender()
        ->SetRenderEngine(rend);
    } //Catch
  }; //GFX Constrct

  ~GFX() {
    delete rendClass;
  }; //GFX
};

inline void pb::RunRender() {
  PRIVATEPB::GFX();
}; //RunRender


void pb::Config::Render::SetRenderEngine(UINT renderEngine) { RenderEngine = renderEngine; };
void pb::Config::Render::SetWindowWidth(UINT windowWidth) { WindowWidth = windowWidth; };
void pb::Config::Render::SetWindowHeight(UINT windowHeight) { WindowHeight = windowHeight; };
void pb::Config::Render::SetWindowName(const char* windowName) { WindowName = windowName; };
void pb::Config::Render::SetAppVersion(const char* version) { AppVersion = version; };
void pb::Config::Render::SetFullscreenBool(bool fScreen) { FullScreen = fScreen; };

UINT pb::Config::Render::GetRenderEngine() { return RenderEngine; };
UINT pb::Config::Render::GetWindowWidth() { return WindowWidth; };
UINT pb::Config::Render::GetWindowHeight() { return WindowHeight; };
const char* pb::Config::Render::GetWindowName() { return WindowName; };
const char* pb::Config::Render::GetAppVersion() { return AppVersion; };
bool pb::Config::Render::GetFullscreenBool() { return FullScreen; };