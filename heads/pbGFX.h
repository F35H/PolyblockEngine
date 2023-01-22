#pragma once

#define GLFW_INCLUDE_VULKAN

#include <pbCntx.h>
#include <pbUtils.h>
#include <pbMacros.h> 

namespace pbVulkan {
  class pbGFX {

  };
  
  class pbCntrl {

  };
  
  class pbInit :
    private pbUtils::pbOutput,
    private pbCntx::pbCntx {

    struct QFamIndices {


      bool isComplete();

      QFamIndices() = default;
    }; //QFamIndices

    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;

    const std::vector<const char*> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    void populateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
      VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
      const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

  protected:
    pbInit();
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice d);
    QFamIndices findQFams(VkPhysicalDevice d);
  };
  
  class pbVulkan {

  public:
    pbVulkan();

  };


}

namespace pbOpenGL {
  class pbOpenGL {

  };
}

namespace pbGFX {

  class pbGFX :
    
    // Inherit Heads
    private pbCntx::pbCntx,

    //Inherit Namespace
    private pbVulkan::pbVulkan,
    private pbOpenGL::pbOpenGL {

  protected:
    pbGFX();
    void gfxLoop();

  };
}