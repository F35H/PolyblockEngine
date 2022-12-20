#include <pbGFX.h>

#define GLFW_INCLUDE_VULKAN


namespace pbVulkan {

  //PbInit
  bool pbInit::QFamIndices::isComplete() {
    return gfxFam.has_value() && presFam.has_value();
  } //QFamIndices

  pbInit::pbInit() {
    const std::vector<const char*> validLayers = {
    "VK_LAYER_KHRONOS_validation" };



    if (DEBUG) {
      WritetoTimedLog(
        "Vulkan Initialization | GPU Validation | Attempting Layer Check");

      UINT32 vIndice;
      vkEnumerateInstanceLayerProperties(&vIndice, nullptr);

      std::vector<VkLayerProperties> availableLayers(vIndice);
      vkEnumerateInstanceLayerProperties(&vIndice, availableLayers.data());

      UFINT8 i, j = 0;
      for (;;) {
        if (validLayers[j] == availableLayers[i].layerName) {

          if (j >= validLayers.size()) {
            WritetoTimedLog(
              "Vulkan Initialization | GPU Validation | Layer Check Failed");
            return;
          };

          ++j;
        }
        else {
          if (i >= validLayers.size()) {
            break;
          };

          ++i;

        } //Else
      } //For

    } //IF




    WritetoTimedLog(
      "Vulkan Initialization | Collecting Application Info | Attempting Collection");

    //Initializing AppInfo
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Polyblock Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_0;



    WritetoTimedLog(
      "Vulkan Initialization | Assigning Application Info | Attempting Assignment");

    VkInstance instance;
    VkInstanceCreateInfo createInstanceInfo{ };
    createInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInstanceInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&createInstanceInfo, nullptr, &instance) != VK_SUCCESS) {
      throw std::runtime_error("Vulkan Initialization | Assigning Application Failed | Trying Next Renderer");
    } //IF



    WritetoTimedLog(
      "Vulkan Initialization | Creating Vulkan Logger | Attempting Creation");

    VkDebugUtilsMessengerEXT debugMessenger;

    if (DEBUG) {
      VkDebugUtilsMessengerCreateInfoEXT createInfo;
      populateDebugMessenger(createInfo);

      if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
      }
    }



    WritetoTimedLog(
      "Vulkan Initialization | Building Window Surface | Attempting Creation");

    if (glfwCreateWindowSurface(instance, winVec[0].win, nullptr, &surface) != VK_SUCCESS) {
      throw std::runtime_error("Vulkan Initialization | Creating Window Surface Failed | Trying Next Renderer");
    } //IF



    WritetoTimedLog(
      "Vulkan Initialization | Enumerating Device Data | Attempting Enumeration");

    UFINT32 deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
      throw std::runtime_error(
        "Vulkan Initialization | Collecting Device Data | Attempt Failed, Invalid GPU");
    } //IF



    WritetoTimedLog(
      "Vulkan Initialization | Confirming Device Compatability | Attempting Comparison");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
      QFamIndices indices = findQFams(device);

      UINT32 extensionCount;
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

      std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

      for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
      } //For

      bool extensionsSup = requiredExtensions.empty();
      bool posSwapChain = false;

      if (extensionsSup) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        posSwapChain = !swapChainSupport.formats.empty() &&
          !swapChainSupport.presentModes.empty();

      } //IF

      VkPhysicalDeviceFeatures supportedFeatures;
      vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

      if (indices.isComplete() &&
        extensionsSup &&
        posSwapChain &&
        supportedFeatures.samplerAnisotropy) {

        physicalDevice = device;
        break;
      } //IF
    } //FOR

    if (physicalDevice == VK_NULL_HANDLE) {
      throw std::runtime_error("Vulkan Initialization | Confirming Device Compatability | Comparison Failed");
    } //IF



    WritetoTimedLog(
      "Vulkan Initialization | Creating Device Interface | Attempting Interface Creation");

    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    QFamIndices indices = findQFams(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> qCreateInfo;
    std::set<UINT32> uniqueQFams = {
        indices.gfxFam.value(), indices.presFam.value() };

    float qPriority = 1.0f;
    for (UINT32 qFam : uniqueQFams) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = qFam;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &qPriority;
      qCreateInfo.emplace_back(queueCreateInfo);

    } //FOR

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createDeviceInfo{};
    createDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createDeviceInfo.queueCreateInfoCount = static_cast<UINT32>(qCreateInfo.size());
    createDeviceInfo.pEnabledFeatures = &deviceFeatures;
    createDeviceInfo.enabledExtensionCount = static_cast<UINT32>(deviceExtensions.size());
    createDeviceInfo.ppEnabledLayerNames = validLayers.data();


    if (vkCreateDevice(physicalDevice, &createDeviceInfo, nullptr, &device) != VK_SUCCESS) {
      WritetoTimedLog(
        "Vulkan Initialization | Creating Device Interface | Interface Creation Attempt Failed");
    } //IF


    vkGetDeviceQueue(device, indices.gfxFam.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.gfxFam.value(), 0, &presentQueue);

  }//Constructor

  pbInit::SwapChainSupportDetails pbInit::querySwapChainSupport(VkPhysicalDevice d) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(d, surface, &details.capabilities);

    UINT32 fmatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(d, surface, &fmatCount, nullptr);

    if (fmatCount != 0) {
      details.formats.resize(fmatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(d, surface, &fmatCount, details.formats.data());
    }//IF

    UINT32 presModeCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(d, surface, &presModeCount, nullptr);

    if (presModeCount != 0) {
      details.presentModes.resize(presModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(d, surface, &presModeCount, details.presentModes.data());
    } //IF

    return details;

  } //SwapChainSupport

  pbInit::QFamIndices pbInit::findQFams(VkPhysicalDevice d) {
    QFamIndices indices;

    UINT32 qFamCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(d, &qFamCount, nullptr);

    std::vector<VkQueueFamilyProperties> qFams(qFamCount);
    vkGetPhysicalDeviceQueueFamilyProperties(d, &qFamCount, qFams.data());

    size_t i = 0;
    for (const auto& qFam : qFams) {
      if (qFam.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        indices.gfxFam = i;
      } //IF

      VkBool32 presSup = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(d, i, surface, &presSup);

      if (presSup) {
        indices.presFam = i;
      } //IF

      if (indices.isComplete()) {
        break;
      } //IF

      ++i;
    } //FOR

    return indices;
  } //QFamIndices

  void pbInit::populateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
  } //PopulateDebugMessenger

  VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
      const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
      return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
  }

  VKAPI_ATTR VkBool32 VKAPI_CALL pbInit::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::string str = "Vulkan Initialization | GPU Validation | ";
    std::string id = pCallbackData->pMessageIdName;
    std::string msg = pCallbackData->pMessage;
    std::string rtrnStr = str + id + " | " + msg;

    WritetoTimedLog(rtrnStr);

    return VK_FALSE;
  }


  //PbGFX

  //Commands

}; //pbVulkan Namespace