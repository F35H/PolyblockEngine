#include <pbMacros.h>
#include <PRVTPB.h>

#define PRVTPB //PIMPLE


  //CONFIG
  struct PRIVATEPB::Config {
    bool Confirmed : 1;
    bool WrotetoUtil : 1;
    bool WrotetoRender : 1;

    std::shared_ptr<pb::Config::Utils> U; //Utils
    std::shared_ptr<pb::Config::Render> R; //Render

    Config() :
      Confirmed(0b00),
      WrotetoUtil(0b00),
      WrotetoRender(0b00)
    {}; //Config

  }; //Config


  //CLIENT
  struct PRIVATEPB::Client {

    //Make this its own class
    std::vector<
      std::shared_ptr<Client>
    > ClientVector;

    std::shared_ptr<Config> Conf = std::make_shared<Config>();



    //CLIENT
    Client() {
      ClientVector = std::vector
        <std::shared_ptr<Client>>();
    }; //CLIENT

  }; //Client


class Control {
}; //Control


  class Features {
  }; //Features


    //Config Functions
    void PRIVATEPB::Client::PrepareVector() {
      if (PB->Client_ptr->ClientVector.empty()) {

        PB->Client_ptr->ClientVector.emplace_back(
          std::make_shared<PRIVATEPB::Client>());
      
      }; //IF

    }; //CheckConfigs


    void pb::Config::AddConfig(pb::Config::Utils* U) {
      CheckConfigs();

      if (!PB->Client_ptr->ClientVector[
        PB->Client_ptr->ClientVector.size() - 1]->
        Conf->WrotetoUtil) {

        PB->Client_ptr->ClientVector[
          PB->Client_ptr->ClientVector.size() - 1]->
          Conf->U = std::move(std::shared_ptr
            <pb::Config::Utils>(U));

          PB->Client_ptr->ClientVector[
            PB->Client_ptr->ClientVector.size() - 1]->
            Conf->WrotetoUtil = true;
      } //IF
      else {
        //WritetoErrorLog
        terminate();
      }; //ELSE

    }; //AddConfig


    void pb::Config::AddConfig(pb::Config::Render* R) {
      CheckConfigs();

      if (!PB->Client_ptr->ClientVector[
        PB->Client_ptr->ClientVector.size() - 1]->
        Conf->WrotetoRender) {

        PB->Client_ptr->Client::ClientVector[
          PB->Client_ptr->ClientVector.size() - 1]->
          Conf->R = std::move(std::shared_ptr
            <pb::Config::Render>(R));

          PB->Client_ptr->ClientVector[
            PB->Client_ptr->ClientVector.size() - 1]->
          Conf->WrotetoRender = true;
      } //IF
      else {
        //WritetoErrorLog
        terminate();
      }; //ELSE
    }; //Add Config
    

    void pb::Config::AddConfig(std::shared_ptr<pb::Config::Utils> U) {
      CheckConfigs();

      if (!PB->Client_ptr->ClientVector[
        PB->Client_ptr->ClientVector.size() - 1]->
        Conf->WrotetoUtil) {

        PB->Client_ptr->ClientVector[
          PB->Client_ptr->ClientVector.size() - 1]->
          Conf->U = U;

          PB->Client_ptr->ClientVector[
            PB->Client_ptr->ClientVector.size() - 1]->
          Conf->WrotetoUtil = true;
      } //IF
      else {
        //WritetoErrorLog

      }; //ELSE
    }; //AddConfig
    

    void pb::Config::AddConfig(std::shared_ptr<pb::Config::Render> R) {
      CheckConfigs();

      if (!PB->Client_ptr->ClientVector[
        PB->Client_ptr->ClientVector.size() - 1]->
        Conf->WrotetoRender) {

        PB->Client_ptr->ClientVector[
          PB->Client_ptr->ClientVector.size() - 1]->
          Conf->R = R;

          PB->Client_ptr->ClientVector[
            PB->Client_ptr->ClientVector.size() - 1]->
          Conf->WrotetoRender = true;

      } //IF
      else {
        //WritetoErrorLog

      }; //ELSE
    }; //AddConfig


    void pb::Config::ConfirmConfigs() {
      
      PB->Client_ptr->ClientVector[
        PB->Client_ptr->ClientVector.size() - 1]->
        Conf->Confirmed = true;


        //ConstructOutput();
        //Flush Log
    };


namespace pb {


  //Client
  //inline void Client::Confirm() {
  //  try {

  //    Config = std::make_shared<pbConfig>();
  //    Render = std::make_shared<pbRender>();
  //    Utils = std::make_shared<pbUtils>();
  //  }
  //  catch (const std::exception& exc) {
  //    Utils->Output->WritetoTimedLog(exc.what());
  //  }
  //  catch (...) {
  //    Utils->Output->WritetoTimedLog("Unknown Exception Triggered!");
  //  }; //Catch
  //}; //Confirm

  //inline void Client::Start() {
  //  GFX::Render* r = new GFX::Render();
  //  delete r;
  //};


  //GFX
  //GFX::Render::Render() {
  //  auto Utils = GetUtils();

  //  auto Config = GetConfig();
  //  auto rend = Config->Render->RenderEngine;

  //  while (rend) {
  //    try {
  //      switch (rend)
  //      {
  //      case VULKAN13:
  //        API->Vulkan = new pbGFX::Vulkan();
  //        break;

  //      default:
  //        throw std::runtime_error("Initializing Render Engine | Selecting Engine | All Possible Renders Failed!");
  //      } //Switch
  //    } //Try

  //    catch (const std::exception& exc) {
  //      Utils->Output->WritetoTimedLog(exc.what());
  //    } //Catch

  //    catch (...) {
  //      Utils->Output->WritetoTimedLog("Unknown Exception Triggered!");
  //      rend -= 1;

  //    } //Catch
  //  } //While
  //}; //RunRender


  //GFX::Vulkan
  //GFX::Vulkan::Vulkan() {
  //};


};


namespace pb {
 // //inline std::shared_ptr<pbConfig> CreateConfig() {
 //   //Context::Config = std::make_shared<pbConfig>();
 //   //return Context::Config;
 // //}; //CreateContext

 // //inline std::shared_ptr<pbConfig> GetConfig() {
 //   //auto Cntx = Context::Config;
 //   //return Cntx;
 // //}; //CreateContext

 // //inline std::shared_ptr<pbUtils> CreateUtils() {
 // //  Context::Utils = std::make_shared<pbUtils>();
 // //  return Context::Utils;
 // //}; //CreateContext

 // //inline std::shared_ptr<pbUtils> GetUtils() {
 // //  auto Utils = Context::Utils;
 // //  return Utils;
 // //}; //CreateContext  
 // //
 // //inline std::shared_ptr<pbRender> CreateRender() {
 // //  Context::Render = std::make_shared<pbRender>();
 // //  return Context::Render;
 // //}; //CreateContext

 // //inline std::shared_ptr<pbRender> GetRender() {
 // //  auto Cntx = Context::Render;
 // //  return Cntx;
 // //}; //CreateContext


 ////PbGFX::Vulkan
 // pbGFX::Vulkan::Vulkan() {
 //   auto Config = GetConfig();
 //   auto Utils = GetUtils();

 //   auto RendConf = Config->Render;
 //   
 //   Utils->Output->WritetoTimedLog(
 //     "Creating Window | Selecting Window | Collecting Configurations");

 //   if (RendConf->WindowHeight && RendConf->WindowWidth) {
 //     createWindow(RendConf->WindowWidth,
 //       RendConf->WindowHeight, RendConf->WindowName);
 //   }
 //   else {
 //     createWindow(RendConf->WindowName);
 //   } //Else

 //   pbInit::pbInit(primWin->win);

 //   //Render = new pbRender();
 //   //Control = new pbControl();
 //   //Main = new pbMain();
 // } //Vulkan

 // bool pbGFX::Vulkan::pbInit::QFamIndices::isComplete() {
 //   return gfxFam.has_value() && presFam.has_value();
 // };

 // pbGFX::Vulkan::pbInit::pbInit(GLFWwindow* primWin) {
 //   auto Config = GetConfig();
 //   auto Utils = GetUtils();

 //   auto RendConf = Config->Render;

 //   if (DEBUG) {
 //     Utils->Output->WritetoTimedLog(
 //       "Vulkan Initialization | GPU Validation | Attempting Layer Check");

 //     UINT32 vIndice;
 //     vkEnumerateInstanceLayerProperties(&vIndice, nullptr);

 //     std::vector<VkLayerProperties> availableLayers(vIndice);
 //     vkEnumerateInstanceLayerProperties(&vIndice, availableLayers.data());

 //     size_t i = 0, j = 0;
 //     for (;;) {
 //       if (validLayers[j] == availableLayers[i].layerName) {

 //         if (j >= validLayers.size()) {
 //           Utils->Output->WritetoTimedLog(
 //             "Vulkan Initialization | GPU Validation | Layer Check Failed");
 //           return;
 //         };

 //         ++j;
 //       }
 //       else {
 //         if (i >= validLayers.size()) {
 //           break;
 //         };

 //         ++i;

 //       } //Else
 //     } //For

 //   } //IF

 //   Utils->Output->WritetoTimedLog(
 //     "Vulkan Initialization | Collecting Application Info | Attempting Collection");

 //   //Initializing AppInfo
 //   VkApplicationInfo appInfo{};
 //   appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
 //   appInfo.pApplicationName = RendConf->WindowName.c_str();
 //   appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
 //   appInfo.pEngineName = "Polyblock Engine";
 //   appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
 //   appInfo.apiVersion = VK_API_VERSION_1_0;



 //   Utils->Output->WritetoTimedLog(
 //     "Vulkan Initialization | Assigning Application Info | Attempting Assignment");

 //   VkInstance instance;
 //   VkInstanceCreateInfo createInstanceInfo{ };
 //   createInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
 //   createInstanceInfo.pApplicationInfo = &appInfo;

 //   uint32_t glfwExtensionCount = 0;
 //   const char** glfwExtensions;
 //   glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

 //   std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

 //   if (DEBUG) {
 //     extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
 //   }

 //   createInstanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
 //   createInstanceInfo.ppEnabledExtensionNames = extensions.data();



 //   Utils->Output->WritetoTimedLog(
 //     "Vulkan Initialization | Creating Vulkan Logger | Attempting Creation");

 //   VkDebugUtilsMessengerEXT debugMessenger;

 //   if (DEBUG) {
 //     createInstanceInfo.enabledExtensionCount = static_cast<UINT32>(validLayers.size());
 //     createInstanceInfo.ppEnabledLayerNames = validLayers.data();

 //     VkDebugUtilsMessengerCreateInfoEXT createDebugInfo;
 //     populateDebugMessenger(createDebugInfo);

 //     createInstanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&createDebugInfo;
 //   }
 //   else {
 //     createInstanceInfo.enabledLayerCount = 0;
 //     createInstanceInfo.pNext = nullptr;
 //   } //ELSE

 //   if (vkCreateInstance(&createInstanceInfo, nullptr, &instance) != VK_SUCCESS) {
 //     throw std::runtime_error("Vulkan Initialization | Vulkan Logger Creation Failed | Attempting Other Renderer");
 //   }



 //   Utils->Output->WritetoTimedLog(
 //     "Vulkan Initialization | Creating the Debugger | Attempting Creation");

 //   if (DEBUG) {
 //     VkDebugUtilsMessengerCreateInfoEXT createDebugInfo;
 //     populateDebugMessenger(createDebugInfo);

 //     if (CreateDebugUtilsMessengerEXT(instance, &createDebugInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
 //       throw std::runtime_error("Vulkan Initialization | Debug Creation Failed | Attempting Other Renderer");
 //     } //IF
 //   } //IF



 //   Utils->Output->WritetoTimedLog(
 //     "Vulkan Initialization | Building Window Surface | Attempting Creation");

 //   if (glfwCreateWindowSurface(instance, primWin, nullptr, &surface) != VK_SUCCESS) {
 //     throw std::runtime_error("Vulkan Initialization | Creating Window Surface Failed | Attempting Other Renderer");
 //   } //IF



 //   Utils->Output->WritetoTimedLog(
 //     "Vulkan Initialization | Enumerating Device Data | Attempting Enumeration");

 //   UINT32 deviceCount = 0;
 //   vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

 //   if (deviceCount == 0) {
 //     throw std::runtime_error(
 //       "Vulkan Initialization | Device Data Collection Failed | Attempting Other Renderer");
 //   } //IF



 //   Utils->Output->WritetoTimedLog(
 //     "Vulkan Initialization | Confirming Device Compatability | Attempting Comparison");

 //   std::vector<VkPhysicalDevice> devices(deviceCount);
 //   vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

 //   for (const auto& device : devices) {
 //     QFamIndices indices = findQFams(device);

 //     UINT32 extensionCount;
 //     vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

 //     std::vector<VkExtensionProperties> availableExtensions(extensionCount);
 //     vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

 //     std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

 //     for (const auto& extension : availableExtensions) {
 //       requiredExtensions.erase(extension.extensionName);
 //     } //For

 //     bool extensionsSup = requiredExtensions.empty();
 //     bool posSwapChain = false;

 //     if (extensionsSup) {
 //       SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
 //       posSwapChain = !swapChainSupport.formats.empty() &&
 //         !swapChainSupport.presentModes.empty();

 //     } //IF

 //     VkPhysicalDeviceFeatures supportedFeatures;
 //     vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

 //     if (indices.isComplete() &&
 //       extensionsSup &&
 //       posSwapChain &&
 //       supportedFeatures.samplerAnisotropy) {

 //       physicalDevice = device;
 //       break;
 //     } //IF
 //   } //FOR

 //   if (physicalDevice == VK_NULL_HANDLE) {
 //     throw std::runtime_error("Vulkan Initialization | Confirming Device Compatability | Comparison Failed");
 //   } //IF



 //   Utils->Output->WritetoTimedLog(
 //     "Vulkan Initialization | Creating Device Interface | Attempting Interface Creation");

 //   VkDevice device;
 //   VkQueue graphicsQueue;
 //   VkQueue presentQueue;

 //   QFamIndices indices = findQFams(physicalDevice);

 //   std::vector<VkDeviceQueueCreateInfo> qCreateInfo;
 //   std::set<UINT32> uniqueQFams = {
 //       indices.gfxFam.value(), indices.presFam.value() };

 //   float qPriority = 1.0f;
 //   for (UINT32 qFam : uniqueQFams) {
 //     VkDeviceQueueCreateInfo queueCreateInfo{};
 //     queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
 //     queueCreateInfo.queueFamilyIndex = qFam;
 //     queueCreateInfo.queueCount = 1;
 //     queueCreateInfo.pQueuePriorities = &qPriority;
 //     qCreateInfo.emplace_back(queueCreateInfo);

 //   } //FOR

 //   VkPhysicalDeviceFeatures deviceFeatures{};
 //   deviceFeatures.samplerAnisotropy = VK_TRUE;

 //   VkDeviceCreateInfo createDeviceInfo{};
 //   createDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
 //   createDeviceInfo.queueCreateInfoCount = static_cast<UINT32>(qCreateInfo.size());
 //   createDeviceInfo.pEnabledFeatures = &deviceFeatures;
 //   createDeviceInfo.enabledExtensionCount = static_cast<UINT32>(deviceExtensions.size());
 //   createDeviceInfo.ppEnabledLayerNames = validLayers.data();


 //   if (vkCreateDevice(physicalDevice, &createDeviceInfo, nullptr, &device) != VK_SUCCESS) {
 //     Utils->Output->WritetoTimedLog(
 //       "Vulkan Initialization | Creating Device Interface | Interface Creation Attempt Failed");
 //   } //IF


 //   vkGetDeviceQueue(device, indices.gfxFam.value(), 0, &graphicsQueue);
 //   vkGetDeviceQueue(device, indices.gfxFam.value(), 0, &presentQueue);
 // }; //pbInitConstrct

 // pbGFX::Vulkan::pbInit::SwapChainSupportDetails pbGFX::Vulkan::Vulkan::pbInit::querySwapChainSupport(VkPhysicalDevice d) {
 //   SwapChainSupportDetails details;

 //   vkGetPhysicalDeviceSurfaceCapabilitiesKHR(d, surface, &details.capabilities);

 //   UINT32 fmatCount;
 //   vkGetPhysicalDeviceSurfaceFormatsKHR(d, surface, &fmatCount, nullptr);

 //   if (fmatCount != 0) {
 //     details.formats.resize(fmatCount);
 //     vkGetPhysicalDeviceSurfaceFormatsKHR(d, surface, &fmatCount, details.formats.data());
 //   }//IF

 //   UINT32 presModeCount;
 //   vkGetPhysicalDeviceSurfaceFormatsKHR(d, surface, &presModeCount, nullptr);

 //   if (presModeCount != 0) {
 //     details.presentModes.resize(presModeCount);
 //     vkGetPhysicalDeviceSurfacePresentModesKHR(d, surface, &presModeCount, details.presentModes.data());
 //   } //IF

 //   return details;
 // }

 // pbGFX::Vulkan::pbInit::QFamIndices pbGFX::Vulkan::Vulkan::pbInit::findQFams(VkPhysicalDevice d) {
 //   QFamIndices indices;

 //   UINT32 qFamCount = 0;
 //   vkGetPhysicalDeviceQueueFamilyProperties(d, &qFamCount, nullptr);

 //   std::vector<VkQueueFamilyProperties> qFams(qFamCount);
 //   vkGetPhysicalDeviceQueueFamilyProperties(d, &qFamCount, qFams.data());

 //   UINT32 i = 0;
 //   for (const auto& qFam : qFams) {
 //     if (qFam.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
 //       indices.gfxFam = i;
 //     } //IF

 //     VkBool32 presSup = false;
 //     vkGetPhysicalDeviceSurfaceSupportKHR(d, i, surface, &presSup);

 //     if (presSup) {
 //       indices.presFam = i;
 //     } //IF

 //     if (indices.isComplete()) {
 //       break;
 //     } //IF

 //     ++i;
 //   } //FOR

 //   return indices;
 // } //QFamIndices

 // void pbGFX::Vulkan::pbInit::populateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
 //   createInfo = {};
 //   createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
 //   createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
 //   createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
 //   createInfo.pfnUserCallback = debugCallback;
 // } //PopulateDebugMessenger

 // VkResult pbGFX::Vulkan::pbInit::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
 //   const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
 //   auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
 //   if (func != nullptr) {
 //     return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
 //   }
 //   else {
 //     return VK_ERROR_EXTENSION_NOT_PRESENT;
 //   }
 // }

 // VKAPI_ATTR VkBool32 VKAPI_CALL pbGFX::Vulkan::pbInit::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
 //   VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
 //   auto Utils = GetUtils();


 //   std::string str = "Vulkan Initialization | GPU Validation | ";
 //   std::string id = pCallbackData->pMessageIdName;
 //   std::string msg = pCallbackData->pMessage;
 //   std::string rtrnStr = str + id + " | " + msg;

 //   Utils->Output->WritetoTimedLog(rtrnStr);

 //   return VK_FALSE;
 // } //debugCallback



 // //pbGFX::GLFW
 // pbGFX::GLFW::Window::Window(Monitor* monitor, std::string winname) {
 //   winName = winname;
 //   winWidth = monitor->workArea->height;
 //   winHeight = monitor->workArea->height;

 //   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

 //   auto Utils = GetUtils();
 //   Utils->Output->WritetoTimedLog(
 //     "Creating Window | Creating Window | Creating Window");

 //   win = glfwCreateWindow(winWidth, winHeight, winName.c_str(), nullptr, nullptr);
 // }; //Window

 // pbGFX::GLFW::Window::Window(Monitor* monitor, const char* winname) {
 //   winName = winname;
 //   winWidth = monitor->workArea->height;
 //   winHeight = monitor->workArea->height;
 //   mon = monitor;

 //   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

 //   auto Utils = GetUtils();
 //   Utils->Output->WritetoTimedLog(
 //     "Creating Window | Creating Window | Creating Window");

 //   win = glfwCreateWindow(winWidth, winHeight, winName.c_str(), mon->mon, nullptr);
 // }; //Window

 // pbGFX::GLFW::GLFW::Window::Window(Monitor* monitor, const char* winname, int winwidth, int winheight) {
 //   winName = winname;
 //   winWidth = winwidth;
 //   winHeight = winheight;
 //   mon = monitor;

 //   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

 //   auto Utils = GetUtils();
 //   Utils->Output->WritetoTimedLog(
 //     "Creating Window | Creating Window | Creating Window");

 //   win = glfwCreateWindow(winWidth, winHeight, winName.c_str(), mon->mon, nullptr);
 // };

 // pbGFX::GLFW::Window::Window(Monitor* monitor, std::string winname, int winwidth, int winheight) {
 //   winName = winname;
 //   winWidth = winwidth;
 //   winHeight = winheight;
 //   mon = monitor;

 //   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

 //   auto Utils = GetUtils();
 //   Utils->Output->WritetoTimedLog(
 //     "Creating Window | Creating Window | Creating Window");

 //   win = glfwCreateWindow(winWidth, winHeight, winName.c_str(), mon->mon, nullptr);
 // }; //Window

 // pbGFX::GLFW::WorkArea::WorkArea(GLFWmonitor* monitor) {
 //   glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &width, &height);
 // }; //workArea

 // pbGFX::GLFW::Monitor::Monitor(GLFWmonitor* monitor) {
 //   workArea = new WorkArea(monitor);
 //   vidMode = glfwGetVideoMode(monitor);
 //   mon = monitor;
 // }; //monitor

 // void pbGFX::GLFW::createWindow(int winWidth, int winHeight, std::string winName) {
 //   primWin = new Window(primMon, winName, winWidth, winHeight);
 // }; //createWindow

 // void pbGFX::GLFW::createWindow(int winWidth, int winHeight, const char* winName) {
 //   primWin = new Window(primMon, winName, winWidth, winHeight);
 // }; //createWindow

 // void pbGFX::GLFW::createWindow(std::string winName) {
 //   primWin = new Window(primMon, winName, primMon->workArea->width, primMon->workArea->width);
 // }; //createWindow

 // void pbGFX::GLFW::createWindow(const char* winName) {
 //   primWin = new Window(primMon, winName, primMon->workArea->width, primMon->workArea->width);
 // }; //createWindow

 // pbGFX::GLFW::GLFW() {
 //   auto mon = glfwGetPrimaryMonitor();
 //   primMon = new Monitor(mon);
 // }; //GLFW



 // //UTILS::Output
 // pbUtils::pbuOutput::pbuOutput() {
 //   auto Config = GetConfig();
 //   
 //   int i = 0;
 //   std::string uuid = sole::uuid1().str();

 //   auto cout = Config->Utils->LogBuffer;
 //   auto logfile = Config->Utils->LogFile;

 //   std::string fileName = "logs";
 //   fileName += "/log-";
 //   fileName += uuid;
 //   fileName += ".txt";

 //   const char* errMsg;

 //   do { //Create Log File
 //     switch (i) {
 //     default:
 //       errMsg = "Attempting Log File Creation | File Creation Imminent | Attempting Once";
 //       cout->write(errMsg, sizeof(errMsg));
 //       continue;

 //     case 0b01:
 //       errMsg = "Log File Creation Failed | File Creation Primary Attempt | Attempting Twice";
 //       cout->write(errMsg, sizeof(errMsg));
 //       continue;

 //     case 0b10:
 //       errMsg = "Log File Creation Failed | File Creation Secondary Attempt | Attempting Trice";
 //       cout->write(errMsg, sizeof(errMsg));
 //       continue;

 //     case 0b11:
 //       errMsg = "Log File Creation Failed | File Creation Tertiary Attempt | Termination Imminent";
 //       cout->write(errMsg, sizeof(errMsg));
 //       abort();

 //     }//Switch

 //     ++i;
 //   } while (!logfile->is_open());
 // } //pbOutput 




 // //CONFIG
 // void pbConfig::pbcUtils::SetBenchmarkLevel(UINT level) { BenchmarkLevel = level; };
 // void pbConfig::pbcUtils::SetLogBuffer(std::ostream* logBuffer) { LogBuffer = logBuffer; };
 // void pbConfig::pbcUtils::SetLogFile(std::ofstream* logFile) { LogFile = logFile; };
 // pbConfig::pbcUtils::~pbcUtils() {
 //   delete LogFile;
 //   delete LogBuffer;
 // }; //~pbUtils

 // void pbConfig::pbcGFX::SetRenderEngine(UINT renderEngine) { RenderEngine = renderEngine; };
 // void pbConfig::pbcGFX::SetWindowWidth(UINT windowWidth) { WindowWidth = windowWidth; };
 // void pbConfig::pbcGFX::SetWindowHeight(UINT windowHeight) { WindowHeight = windowHeight; };
 // void pbConfig::pbcGFX::SetWindowName(UINT windowName) { WindowName = windowName; };

}; //PB

int main() {
  auto Utils = new pb::Config::Utils();
  auto Render = new pb::Config::Render();

  pb::Config::AddConfig(Utils);
  pb::Config::AddConfig(Render);
  pb::Config::ConfirmConfigs();

  return 0;
}