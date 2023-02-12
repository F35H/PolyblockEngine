#include <PRVTPB.h>

pb::Config::Render* rendConf;

struct GLFW {
  GLFWmonitor* mon;
  GLFWwindow* win;
  const GLFWvidmode* vidMode;
  INT winXPos;
  INT winYPos;
  INT monWidth;
  INT monHeight;
  pb::Config::Render* rendConf;

  GLFW() {
    InternalLog("Creating Window", "Initializing GLFW", "Initializing");

    if (!glfwInit()) {
      InternalReport("Creating Window", "Initializing GLFW", "Initialization Failed");
    }

    InternalLog("Creating Window", "Creating Monitor", "Collecting Stats");

    mon = glfwGetPrimaryMonitor();
    vidMode = glfwGetVideoMode(mon);

    glfwGetMonitorWorkarea(
      mon, &winXPos, &winYPos,
      &monWidth, &monHeight);
  }; //GLFW

  void createWindow(FINT16 winWidth, FINT16 winHeight, const char* winName) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    InternalLog("Creating Window", "Creating Window", "Creating Window");

    if (rendConf->GetFullscreenBool()) {
      win = glfwCreateWindow(winWidth, winHeight, winName, mon, nullptr);
    }
    else {
      win = glfwCreateWindow(winWidth, winHeight, winName, nullptr, nullptr);
    } //ifFullscreen

    if (!win) {
      InternalReport("Creating Window", "Creating Window", "Window Creation Failed");
    }
  }; //createWindow

  void createWindow(const char* winName) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    InternalLog("Creating Window ", " Creating Window ", " Creating Window");

    if (rendConf->GetFullscreenBool()) {
      win = glfwCreateWindow(monWidth, monHeight, winName, mon, nullptr);
    }
    else {
      win = glfwCreateWindow(monWidth, monHeight, winName, nullptr, nullptr);
    } //ifFullscreen

    if (!win) {
      InternalReport("Creating Window ", " Creating Window ", " Window Creation Failed");
    }

    rendConf->SetWindowHeight(monHeight);
    rendConf->SetWindowWidth(monWidth);
  }; //createWindow

  ~GLFW() {};

}; //GLFW


GLFW* glfw;

namespace Vulkan13 {

  //Sec. Init
  VkDebugUtilsMessengerEXT debugMessenger;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkInstance instance;
  VkSurfaceKHR surface;
  VkQueue presentQueue;
  VkQueue graphicsQueue;

  //Sec. Init
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

  const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation"
  }; //ValidationLayers

  const std::vector<const char*> deviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
  }; //deviceExtensions

  inline QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) {
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
  } //QueueFamilyIndices

  inline SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
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

  inline static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
  } //CallBack

  inline void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
  } //DebugMessenger


  //Sec. Init
  inline void CheckDebug() {
    if (DEBUG) {
      InternalLog("Initializing Vulkan", "Confirming Validation Layers", "Enumerating and Confirming Layers");

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
          InternalReport("Initializing Vulkan ", " Confirming Validation Layers ", " Failure");
        }
      }
    }; // if (DEBUG)
  }; //checkDebug

  inline void CreateInstance(pb::Config::Render* rendConf) {
    InternalLog("Initializing Vulkan ", " Creating Program Instance ", " Writing Struct");

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


    InternalLog("Initializing Vulkan ", " Creating Program Instance ", " Collecting Extensions");

    UINT32 glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (DEBUG) {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();


    InternalLog("Initializing Vulkan ", " Creating Program Instance ", " Collecting Vulkan Debug Data");

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


    InternalLog("Creating Program Instance ", " Creating Virtual Instance ", " Appoving Instance");

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
      InternalReport("Creating Program Instance ", " Creating Virtual Instance ", " Instance Creation Failed");
    }
  }; //createInstance

  inline void SetupDebugMessenger() {
    if (DEBUG) {
      VkDebugUtilsMessengerCreateInfoEXT createInfo;
      populateDebugMessengerCreateInfo(createInfo);

      InternalLog("Initializing Vulkan ", " Creating Debug Messenger ", " Creating Debugger");

      auto b = VK_ERROR_EXTENSION_NOT_PRESENT;

      auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
      if (func != nullptr) {
        b = func(instance, &createInfo, nullptr, &debugMessenger);
      }

      InternalLog("Initializing Vulkan ", " Confirming Debugger ", " Confirming Debugger");

      if (b != VK_SUCCESS) {
        InternalReport("Initializing Vulkan ", " Confirming Debugger ", " Confirmation Failed");

      } // If CeateMessenger
    }; //If Debug
  }; //setupDebugMessenger

  inline void CreateSurface(GLFWwindow* win) {
    InternalLog("Initializing Vulkan ", " Creating Window Surface ", " Creating and Confirming");

    if (glfwCreateWindowSurface(instance, win, nullptr, &surface) != VK_SUCCESS) {
      InternalReport("Initializing Vulkan ", " Creating Window Surface ", " Creation Failed");
    } //If Surface
  }; //Create Surface

  inline void PickPhysicalDevice() {
    InternalLog("Initializing Vulkan ", " Picking Physical Device ", " Enumerating GPUs");

    UINT32 deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
      InternalReport("Initializing Vulkan ", " Picking Physical Device ", " No GPU Support Found");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    InternalLog("Initializing Vulkan ", " Picking Physical Device ", " Testing Pipelines");

    for (const auto& device : devices) {
      QueueFamilyIndices indices = FindQueueFamilies(device);

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

      if (indices.isComplete() && requiredExtensions.empty() && swapChainAdequate) {
        physicalDevice = device;
        break;
      }
    }

    InternalLog("Initializing Vulkan ", " Picking Physical Device ", " Confirming Pipeline Support");

    if (physicalDevice == VK_NULL_HANDLE) {
      InternalReport("Initializing Vulkan ", " Picking Physical Device ", " No Pipeline Support Found");
    }
  }; //pickPhysicalDevice

  inline void CreateLogicalDevice() {
    InternalLog("Initializing Vulkan ", " Creating Device Interface ", " Queueing Devices");

    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    InternalLog("Initializing Vulkan ", " Creating Device Interface ", " Staging Info");

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

    InternalLog("Initializing Vulkan ", " Creating Device Interface ", " Confirming Virtual Interface");

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
      throw std::runtime_error("failed to create logical device!");
    }

    InternalLog("Initializing Vulkan ", " Creating Device Interface ", " Applying Interface to Device");

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
  }; //createLogicalDevice

  inline void VulkanInit() {
    CheckDebug();
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
  }; //VulkanInit

  inline void VulkanDelete() {
  
  }; //VulkanDelete
};

struct PRIVATEPB::Vulkan {
  Vulkan(pb::Config::Render* R) {
    rendConf = R;

    InternalLog(
      "Creating Window ", " Selecting Window ", " Collecting Configurations");

    FINT16 winHeight = rendConf->GetWindowHeight();
    FINT16 winWidth = rendConf->GetWindowWidth();
    const char* winName = rendConf->GetWindowName();

    if (winHeight && winWidth) {
      glfw->createWindow(winHeight, winWidth, winName);
    }
    else {
      glfw->createWindow(winName);
    } //Else


    // Select Vulkan Version
    try {
      while (1) {
        switch (rendConf->GetRenderEngine()) {
        case VULKAN13:
          Vulkan13::VulkanInit();
          break;
        }; //Switch
      }; //While
    } //Try

    catch (const std::exception& exc) {
      auto rend = rendConf->GetRenderEngine();

      std::string mesoSphere = "Selecting Engine: ";
      mesoSphere += rend;
      InternalLog("Initializing Vulkan ", mesoSphere.c_str(), exc.what());

      rend -= 1;

      rendConf
        ->SetRenderEngine(rend);
    } //Catch exc

    catch (...) {
      auto rend = rendConf->GetRenderEngine();

      std::string mesoSphere = "Selecting Engine: ";
      mesoSphere += rend;
      InternalLog("Initializing Vulkan ", mesoSphere.c_str(), "Unknown Exception Occuered");

      rend -= 1;

      rendConf
        ->SetRenderEngine(rend);
    }; //Catch exc
  }; //Vulkan

  ~Vulkan() {
    // Select Vulkan Version
    try {
      while (1) {
        switch (rendConf->GetRenderEngine()) {
        case VULKAN13:
          Vulkan13::VulkanDelete();
          break;
        }; //Switch
      }; //While
    } //Try

    catch (const std::exception& exc) {
      auto rend = rendConf->GetRenderEngine();
      std::string mesoSphere = "Deleting Engine: ";
      mesoSphere += rend;
      InternalLog("Deleting Vulkan ", mesoSphere.c_str(), exc.what());
      terminate();
    } //Catch exc

    catch (...) {
      auto rend = rendConf->GetRenderEngine();
      std::string mesoSphere = "Deleting Engine: ";
      mesoSphere += rend;
      InternalLog("Deleting Vulkan ", mesoSphere.c_str(), "Unknown Exception");
      terminate();
    }; //Catch exc
  }; //~Vulkan

}; //Vulkan Class