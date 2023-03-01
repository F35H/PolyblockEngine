#include <PRVTPB.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

pb::Config::Render* rendConf;

struct GLFW {
  GLFWmonitor* mon;
  GLFWwindow* win;
  const GLFWvidmode* vidMode;
  INT winXPos;
  INT winYPos;
  INT monWidth;
  INT monHeight;



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

struct Input {
  int backtick : 1;
  int one : 1;
  int two : 1;
  int three : 1;
  int four : 1;
  int five : 1;
  int six : 1;
  int seven : 1;
  int eight : 1;
  int nine : 1;
  int zero : 1;
  int minus : 1;
  int plus : 1;
  int tab : 1;
  int q : 1;
  int w : 1;
  int e : 1;
  int r : 1;
  int t : 1;
  int y : 1;
  int i : 1;
  int o : 1;
  int p : 1;
  int a : 1;
  int s : 1;
  int d : 1;
  int f : 1;
  int g : 1;
  int h : 1;
  int j : 1;
  int k : 1;
  int l : 1;
  int u : 1;
  int colon : 1;
  int apostophe : 1;
  int shift : 1;
  int z : 1;
  int x : 1;
  int c : 1;
  int v : 1;
  int b : 1;
  int n : 1;
  int m : 1;
  int comma : 1;
  int period : 1;
  int slash : 1;
  int alt : 1;
  int backslash : 1;
  int bracketLeft : 1;
  int bracketRight : 1;
  int cntrl : 1;
  int space : 1;
};

Input* input;
GLFW* glfw;

struct WorldSpace {
  std::chrono::steady_clock::time_point delta;
  //std::vector<Camera*>* camVec;

  WorldSpace() {};

  void TriggerDelta() {
    delta = std::chrono::high_resolution_clock::now();
  };

  float GetDelta() {
    auto cT = std::chrono::high_resolution_clock::now();

    auto d = std::chrono::duration<float,
      std::chrono::seconds::period>(cT -
        delta).count();

    std::cout << d << "\n";

    return d;

  };

};

WorldSpace* world;

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
  }
};


struct Block {
  Block() {};

  glm::mat4 DeltaUpWorldPos(float x, float y, float z) {
    auto d = world->GetDelta();

    worldPos[3][0] += -x * d;
    worldPos[3][1] += -y * d;
    worldPos[3][2] += -z * d;
    return worldPos;
  };

  int blockCount = 300;

  glm::mat4 worldPos;

  const std::vector<Vertex> vertices = {

    //Bottom
    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

    //Back Plane
    {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

    //Front Plane
    {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

    //Right Plane
    {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

    //Left Plane
    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

    //Top
    {{0.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
  };

  const std::vector<UINT16> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
  };


};

struct Camera {
  Camera(glm::vec3 initPos) {
    worldPos = glm::mat4(1.0f);

    worldPos[3][0] = initPos[0];
    worldPos[3][1] = initPos[1];
    worldPos[3][2] = initPos[2];



    nearDis = 5.0f;
    farDis = 15.0f;
  };

  glm::mat4 SetPerspectiveProj(float width, float height) {
    projMat = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 10.0f);
    projMat[1][1] *= -1;
    return projMat;
  };

  glm::mat4 SetViewMatrix(float x, float y, float z) {
    viewMat = glm::lookAt(
      glm::vec3(2.0f, 2.0f, 2.0f), //Eye
      glm::vec3(
        -x + worldPos[3][0], //Look At X
        -y + worldPos[3][1], //Look At Y
        -z + worldPos[3][2]), //Look At Z
      glm::vec3(0.0f, 1.0f, 0.0f)); //Height

    viewMat *= worldPos;
    return viewMat;
  };

  glm::mat4 SetWorldPos(float x, float y, float z) {
    worldPos[3][0] = -x;
    worldPos[3][1] = -y;
    worldPos[3][2] = -z;
    return worldPos;
  };

  glm::mat4 UpWorldPos(float x, float y, float z) {
    worldPos[3][0] += -x;
    worldPos[3][1] += -y;
    worldPos[3][2] += -z;
    return worldPos;
  };

  glm::mat4 DeltaSetWorldPos(float x, float y, float z) {
    auto d = world->GetDelta();

    worldPos[3][0] = -x * d;
    worldPos[3][1] = -y * d;
    worldPos[3][2] = -z * d;
    return worldPos;
  };

  glm::mat4 DeltaUpWorldPos(float x, float y, float z) {
    auto d = world->GetDelta();

    worldPos[3][0] += -x * d;
    worldPos[3][1] += -y * d;
    worldPos[3][2] += -z * d;
    return worldPos;
  };

  glm::mat4 GetPerspectiveProj() {
    return projMat;
  };

  glm::mat4 GetViewMatrix() {
    return viewMat *= worldPos;
  };

  glm::mat4 GetWorldPos() {
    return worldPos;
  };

private:
  float farDis;
  float nearDis;

  glm::mat4 worldPos;
  glm::mat4 viewMat;
  glm::vec3 eyePos;

  glm::mat4 projMat;
};

Camera* cam;
Block* block;

struct VulkanInterface {
  struct SoftInterface {
    VkDevice device;
  };

  struct HardInterface {
    VkPhysicalDevice device;
    VkPhysicalDeviceProperties deviceProperties;
  
    HardInterface() {
      vkGetPhysicalDeviceProperties(device, &deviceProperties);
    };
  };

  SoftInterface* softInterface;
  HardInterface* hardInterface;

  VulkanInterface() = delete;
  VulkanInterface() :
    softInterface(new SoftInterface()),
    hardInterface(new HardInterface()) 
  {};

  ~VulkanInterface() {
    delete softInterface;
    delete hardInterface;
  };

};


namespace Vulkan13 {
  VkDevice device;
  VkCommandPool commandPool;
  VkPhysicalDevice physicalDevice;
  VkQueue graphicsQueue;

  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
      if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
        return i;
      }
    }

    throw std::runtime_error("failed to find suitable memory type!");
  } //FindMemoryType

  struct CmndBuffer {
    VkBuffer stageBuff;
    VkBuffer destBuff;
    VkDeviceSize buffSize;
    VkDeviceMemory stageData;
    VkDeviceMemory destData;


    CmndBuffer(VkDeviceSize bufferSize) {
        buffSize = bufferSize;
    }; //Buffer

    void InitStageBuff(VkBufferUsageFlags use, VkMemoryPropertyFlags memProp) {
      VkBufferCreateInfo buffInfo{};
      buffInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      buffInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      buffInfo.size = buffSize;
      buffInfo.usage = use;

      if (vkCreateBuffer(device, &buffInfo, nullptr, &stageBuff) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
      }

      VkMemoryRequirements memReq; //Memory Requirements
      vkGetBufferMemoryRequirements(device, stageBuff, &memReq);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memReq.size;
      allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, memProp);

      if (vkAllocateMemory(device, &allocInfo, nullptr, &stageData) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
      }

      vkBindBufferMemory(device, stageBuff, stageData, 0);
    }

    void InitDestBuff(VkBufferUsageFlags use, VkMemoryPropertyFlags memProp) {
      VkBufferCreateInfo buffInfo{};
      buffInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      buffInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      buffInfo.size = buffSize;
      buffInfo.usage = use;

      if (vkCreateBuffer(device, &buffInfo, nullptr, &destBuff) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
      }

      VkMemoryRequirements memReq; //Memory Requirements
      vkGetBufferMemoryRequirements(device, destBuff, &memReq);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memReq.size;
      allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, memProp);

      if (vkAllocateMemory(device, &allocInfo, nullptr, &destData) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
      }

      vkBindBufferMemory(device, destBuff, destData, 0);
    }

    //Templates are for losers
    void InjectData(const void* input) {
      void* data;
      vkMapMemory(device, stageData, 0, buffSize, 0, &data);
      memcpy(data, input, (size_t)buffSize);
      vkUnmapMemory(device, stageData);
    }; //InjectData


    void SendCmndBuffer() {
      //Build Buffer
      VkCommandBuffer cmndBuff;

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = commandPool;
      allocInfo.commandBufferCount = 1;

      vkAllocateCommandBuffers(device, &allocInfo, &cmndBuff);

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      
      //Record
      vkBeginCommandBuffer(cmndBuff, &beginInfo);

      VkBufferCopy copyRegion{};
      copyRegion.size = buffSize;
      vkCmdCopyBuffer(cmndBuff, stageBuff, destBuff, 1, &copyRegion);

      
      //Recorded
      vkEndCommandBuffer(cmndBuff);

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &cmndBuff;

      vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
      vkQueueWaitIdle(graphicsQueue);

      vkFreeCommandBuffers(device, commandPool, 1, &cmndBuff);
    }; //CopyBuffers


    ~CmndBuffer() {
      vkDestroyBuffer(device, stageBuff, nullptr);
      vkFreeMemory(device, stageData, nullptr);
    }; //Destructor
  };


  //Sec. Init
  VkDebugUtilsMessengerEXT debugMessenger;
  VkInstance instance;
  VkSurfaceKHR surface;
  VkQueue presentQueue;


  //Sec. Pipe
  VkImage textureImage;
  VkDeviceMemory textureImageMemory;
  VkImageView textureImageView;
  VkSampler textureSampler;
  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;
  std::vector<VkImageView> swapChainImageViews;
  VkDescriptorSetLayout descriptorSetLayout;
  VkRenderPass renderPass;
  VkExtent2D swapChainExtent;


  //Sec. Cmnd
  std::vector<VkDescriptorSet> descriptorSets;
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;
  std::vector<void*> uniformBuffersMapped;
  VkDescriptorPool descriptorPool;
  UINT32 currentFrame = 0;
  const int MAX_FRAMES_IN_FLIGHT = 2;
  char mesoLog[64];
  const char* macroLog = "Integrating Commands";
  VkImage depthImage;
  VkDeviceMemory depthImageMemory;
  VkImageView depthImageView;

  //Sec. Loop
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  bool framebufferResized = false;

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


  //Sec. Pipe
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
      glfwGetFramebufferSize(glfw->win, &width, &height);

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
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
      throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
  }

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

  //Sec. Cmnd

  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
  } //createBuffer

  void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
      throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(device, image, imageMemory, 0);
  } //CreateImage

  void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
  }//endSingleTimeCommands

  VkCommandBuffer beginSingleTimeCommands() {
    //Copy COmmand Buffer Info
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
  } //beginSingleTimeCommands

  void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else {
      throw std::invalid_argument("unsupported layout transition!");
    }

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;  
        if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT) {
        barrier.subresourceRange.aspectMask |=
          VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    vkCmdPipelineBarrier(
      commandBuffer,
      sourceStage, destinationStage,
      0,
      0, nullptr,
      0, nullptr,
      1, &barrier
    ); //vkCmdPipelineBarrier

    endSingleTimeCommands(commandBuffer);
  } //TransitionImageLayout

  void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    }; //VkBufferImageCopy

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(commandBuffer);
  } //CopyBufferToImage

  void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    
    
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    //Generic Copy
    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    //Flush Equivalent
    endSingleTimeCommands(commandBuffer);
  } //CopyBuffer

  struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
  }; //UniformBufferObjects

  //Sec. Loop
  void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(instance, debugMessenger, pAllocator);
    }
  } // DestroDebugMessengerEXT

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

  inline void CreateInstance() {
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

  inline void CreateSurface() {
    InternalLog("Initializing Vulkan ", " Creating Window Surface ", " Creating and Confirming");

    if (glfwCreateWindowSurface(instance, glfw->win, nullptr, &surface) != VK_SUCCESS) {
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


      VkPhysicalDeviceFeatures supportedFeatures;
      vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

      if (indices.isComplete() && requiredExtensions.empty() && swapChainAdequate && supportedFeatures.samplerAnisotropy) {
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


  //Sec. Pipe
  VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
      VkFormatProperties props;
      vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

      if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
        return format;
      }
      else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
        return format;
      }
    }

    throw std::runtime_error("failed to find supported format!");
  }

  VkFormat FindDepthFormat() {
    return FindSupportedFormat(
      { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
      VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
  } //VkFormat

  VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
      throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
  } //CreateImageView

  inline void CreateSwapChain() {
    InternalLog("Creating Vulkan Pipeline ", " Creating Image Chain ", " Checking for GPU Support");

    SwapChainSupportDetails swapChainSupport =
      querySwapChainSupport(physicalDevice);

    InternalLog("Creating Vulkan Pipeline ", " Creating Image Chain ", " Setting GPU Options");

    VkSurfaceFormatKHR surfaceFormat =
      chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode =
      chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent =
      chooseSwapExtent(swapChainSupport.capabilities);

    InternalLog("Creating Vulkan Pipeline ", " Creating Image Chain ", " Setting Chain Length");

    UINT32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
      imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    InternalLog("Creating Vulkan Pipeline ", " Creating Image Chain ", " Informing Chain");

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
    UINT32 queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

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

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
      InternalReport("Creating Vulkan Pipeline ", " Creating Image Chain ", " Chain Creation Failed");
    }

    InternalLog("Creating Vulkan Pipeline ", " Creating Image Chain ", " Preparing Final Chain");

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
  }; //CreateSwapChain

  void CreateImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
      swapChainImageViews[i] = CreateImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
  } //CreateImageView

  inline void CreateRenderPass() {
    InternalLog("Creating Vulkan Pipeline ", " Creating Image Processors ", " Assigning Image Processors");

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    InternalLog("Creating Vulkan Pipeline ", " Creating Image Processors ", " Confirming Image Processors");

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
      InternalReport("Creating Vulkan Pipeline ", " Creating Image Processors ", " Image Processor Creation Failed");
    }
  }; //CreateRenderPass

  void CreateDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor set layout!");
    }
  }

  inline void CreateGraphicsPipeline() {
    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Reading SPIR-V");

    auto vertShaderCode = readFile("shaders/shadBin/vert.spv");
    auto fragShaderCode = readFile("shaders/shadBin/frag.spv");

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Creating Shaders Interface");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Setting Shader Uses");

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

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Creating Shader Stages");

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Creating Shader Interfaces");

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

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

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

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Confirming Interfaces");

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
      InternalReport("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Interface Confirmation Failed");
    }

    InternalLog(
      "Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Assigning Shader Stages");

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Confirming Graphic Pipeline");

    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
      InternalReport("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Graphics Pipeline Confirmation Failed");
    } //If

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Clearing Unecessary Assets");

    vkDestroyShaderModule(
      device, fragShaderModule, nullptr);
    vkDestroyShaderModule(
      device, vertShaderModule, nullptr);
  }; //CreateGraphicsPipeline

  void CreateFramebuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
      std::array<VkImageView, 2> attachments = {
          swapChainImageViews[i],
          depthImageView
      };

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = renderPass;
      framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
      framebufferInfo.pAttachments = attachments.data();
      framebufferInfo.width = swapChainExtent.width;
      framebufferInfo.height = swapChainExtent.height;
      framebufferInfo.layers = 1;

      if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
      }
    }
  } //CreateFrameBuffers

  //Sec. Cmnd
  inline void CreateCommandPool() {
    memset(mesoLog, 0, sizeof(mesoLog));
    strcpy(mesoLog, "Creating Threads");

    InternalLog(macroLog, mesoLog, " Creating Threadpool");

    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    InternalLog(macroLog, mesoLog, " Checking Theadpool");

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
      InternalReport(macroLog, mesoLog, " Threapool Creation Failed");
    }
  }; //CreateCommandPool

  void CreateDepthResources() {
    VkFormat depthFormat = FindDepthFormat();

    createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    depthImageView = CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    TransitionImageLayout(depthImage, depthFormat,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


  } //CreateDepthResources

  inline void CreateTextureImage() {
    memset(mesoLog, 0, sizeof(mesoLog));
    strcpy(mesoLog, "Creating Texture Interface");

    InternalLog(macroLog, mesoLog, "Creating Texture");

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load("textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
      throw std::runtime_error("failed to load texture image!");
    }

    InternalLog(macroLog, mesoLog, "Building Interface");

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, imageSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

    InternalLog(macroLog, mesoLog, "Assigning Interface Constants");

    TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    InternalLog(macroLog, mesoLog, "Assigning Interface to Hardware");

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
  }; //CreateTextureImage

  void CreateTextureImageView() {
    textureImageView = CreateImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
  } //CreateTextureImageView

  void CreateTextureSampler() {
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
      throw std::runtime_error("failed to create texture sampler!");
    }
  } //CreateTextureSampler

  inline void CreateVertexBuffer() {
    VkDeviceSize bufferSize = 
      sizeof(block->vertices[0]) 
      * block->vertices.size();
    
    auto vertBuff = new CmndBuffer(bufferSize);

    vertBuff->InitStageBuff(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vertBuff->InitDestBuff(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    vertBuff->InjectData(block->vertices.data());
    vertBuff->SendCmndBuffer();

    //instanceBuffer.descriptor.range = instanceBuffer.size;
    //instanceBuffer.descriptor.buffer = instanceBuffer.buffer;
    //instanceBuffer.descriptor.offset = 0;

    delete vertBuff;
  }; //CreateVertexBuffer

  inline void CreateIndexBuffer() {
    VkDeviceSize bufferSize =
      sizeof(block->indices[0])
      * block->indices.size();

    auto indiceBuff = new CmndBuffer(bufferSize);

    indiceBuff->InitStageBuff(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    indiceBuff->InitDestBuff(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    indiceBuff->InjectData(block->indices.data());
    indiceBuff->SendCmndBuffer();

    delete indiceBuff;
  }; //CreateIndexBuffer

  inline void CreateUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

      vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
  }; //CreateUniformBuffer

  inline void CreateDescriptorPool() {
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor pool!");
    }
  } //CreateDescriptorPool


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

      VkDescriptorImageInfo imageInfo{};
      imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageInfo.imageView = textureImageView;
      imageInfo.sampler = textureSampler;

      std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

      descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[0].dstSet = descriptorSets[i];
      descriptorWrites[0].dstBinding = 0;
      descriptorWrites[0].dstArrayElement = 0;
      descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrites[0].descriptorCount = 1;
      descriptorWrites[0].pBufferInfo = &bufferInfo;

      descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[1].dstSet = descriptorSets[i];
      descriptorWrites[1].dstBinding = 1;
      descriptorWrites[1].dstArrayElement = 0;
      descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      descriptorWrites[1].descriptorCount = 1;
      descriptorWrites[1].pImageInfo = &imageInfo;

      vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
  } //CreateDescriptorSets

  void CreateCommandBuffers() {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate command buffers!");
    }
  } //Create Command Buffers


  inline void CreateSyncObjects() {
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
  }


  //Sec. Loop
  void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    //Record Vertice Buffer
    VkDeviceSize bufferSize =
      sizeof(block->vertices[0])
      * block->vertices.size();

    auto vertBuff = new CmndBuffer(bufferSize);

    vertBuff->InitStageBuff(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vertBuff->InitDestBuff(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    vertBuff->InjectData(block->vertices.data());
    vertBuff->SendCmndBuffer();


    //delete vertBuff;
    

    //Record Indiec Buffer
    bufferSize =
      sizeof(block->indices[0])
      * block->indices.size();

    auto indiceBuff = new CmndBuffer(bufferSize);

    indiceBuff->InitStageBuff(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    indiceBuff->InitDestBuff(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    indiceBuff->InjectData(block->indices.data());
    indiceBuff->SendCmndBuffer();

    //delete indiceBuff;


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

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

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

    VkBuffer vertexBuffers[] = { vertBuff->destBuff };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indiceBuff->destBuff, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(block->indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }
  }

  void UpdateUniformBuffer(uint32_t currentImage) {
    UniformBufferObject ubo{};

    size_t minUboAlignment = VkPhysicalDeviceLimits::minUniformBufferOffsetAlignment;
    dynamicAlignment = sizeof(glm::mat4);
    if (minUboAlignment > 0) {
      dynamicAlignment = (dynamicAlignment + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }

    ubo.model = glm::mat4(1.0f);
    ubo.proj = cam->SetPerspectiveProj(swapChainExtent.width, swapChainExtent.width);
    ubo.view = cam->GetViewMatrix();

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    world->TriggerDelta();
  }

  void CleanupSwapChain() {
    vkDestroyImageView(device, depthImageView, nullptr);
    vkDestroyImage(device, depthImage, nullptr);
    vkFreeMemory(device, depthImageMemory, nullptr);

    for (auto framebuffer : swapChainFramebuffers) {
      vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    for (auto imageView : swapChainImageViews) {
      vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
  } //CleanupSwapChain



  void RecreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(glfw->win, &width, &height);
    while (width == 0 || height == 0) {
      glfwGetFramebufferSize(glfw->win, &width, &height);
      glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);

    CleanupSwapChain();

    CreateSwapChain();
    CreateImageViews();
    CreateDepthResources();
    CreateFramebuffers();
  } //RecreateSwapChain

  inline void RenderFrame() {
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    UINT32 imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      RecreateSwapChain();
      return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image!");
    }

    UpdateUniformBuffer(currentFrame);

    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    RecordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
      throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
      framebufferResized = false;
      RecreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }; //Render Frame


  void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    switch (key) {
    case GLFW_KEY_W:
      if (action == GLFW_PRESS) {
        input->w = 1;
      }
      else if (action == GLFW_RELEASE) {
        input->w = 0;
      }
      return;

    case GLFW_KEY_A:
      if (action == GLFW_PRESS) {
        input->a = 1;
      }
      else if (action == GLFW_RELEASE) {
        input->a = 0;
      }
      return;

    case GLFW_KEY_S:
      if (action == GLFW_PRESS) {
        input->s = 1;
      }
      else if (action == GLFW_RELEASE) {
        input->s = 0;
      }
      return;

    case GLFW_KEY_D:
      if (action == GLFW_PRESS) {
        input->d = 1;
      }
      else if (action == GLFW_RELEASE) {
        input->d = 0;
      }
      return;

    case GLFW_KEY_SPACE:
      if (action == GLFW_PRESS) {
        input->space = 1;
      }
      else if (action == GLFW_RELEASE) {
        input->space = 0;
      }
      return;

    case GLFW_KEY_E:
      if (action == GLFW_PRESS) {
        input->e = 1;
      }
      else if (action == GLFW_RELEASE) {
        input->e = 0;
      }
      return;

    case GLFW_KEY_Q:
      if (action == GLFW_PRESS) {
        input->q = 1;
      }
      else if (action == GLFW_RELEASE) {
        input->q = 0;
      }
      return;
    }
  }


  void ActuateEvents() {
    if (input->w) { cam->DeltaUpWorldPos(0, 1, 0); };
    if (input->a) { cam->DeltaUpWorldPos(-1, 0, 0); };
    if (input->d) { cam->DeltaUpWorldPos(1, 0, 0); };
    if (input->s) { cam->DeltaUpWorldPos(0, -1, 0); };
    if (input->e) { cam->DeltaUpWorldPos(0, 0, 1); };
    if (input->q) { cam->DeltaUpWorldPos(0, 0, -1); };
  }; //ActuateEvents

  void Loop() {
    glfwSetKeyCallback(glfw->win, keyCallback);

    cam->SetWorldPos(0,0,0);

    while (!glfwWindowShouldClose(glfw->win)) {
      cam->SetViewMatrix(0, 0, 0);

      glfwPollEvents();
      ActuateEvents();
      RenderFrame();
    }

    vkDeviceWaitIdle(device);
  } //Loop

  inline void VulkanInit() {
    
    //Sec. Init
    CheckDebug();
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface(); //Move to GLFW
    PickPhysicalDevice(); //Move to Class
    CreateLogicalDevice(); //Move to Class

    //Sec. Pipe
    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateDescriptorSetLayout();
    CreateGraphicsPipeline();

    //Sec. Cmnd
    CreateCommandPool();
    CreateDepthResources();
    CreateFramebuffers();
    CreateTextureImage();
    CreateTextureImageView();
    CreateTextureSampler();
    //CreateVertexBuffer(); //Fix this to allow multiple classes
    //CreateIndexBuffer(); //Fix this to allow multiple classes
    CreateUniformBuffers();
    CreateDescriptorPool();
    CreateDescriptorSets();
    CreateCommandBuffers();
    CreateSyncObjects();

    //Sec. Loop
    Loop();
  }; //VulkanInit


  inline void VulkanDelete() {
    CleanupSwapChain();

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      vkDestroyBuffer(device, uniformBuffers[i], nullptr);
      vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorPool(device, descriptorPool, nullptr);

    vkDestroySampler(device, textureSampler, nullptr);
    vkDestroyImageView(device, textureImageView, nullptr);

    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);

    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    //vkDestroyBuffer(device, indexBuffer, nullptr);
    //vkFreeMemory(device, indexBufferMemory, nullptr);

    //vkDestroyBuffer(device, vertexBuffer, nullptr);
    //vkFreeMemory(device, vertexBufferMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
      vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
      vkDestroyFence(device, inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroyDevice(device, nullptr);

    if (DEBUG) {
      DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(glfw->win);

    glfwTerminate();
  }; //VulkanDelete
}; //Vulkan13


  PRIVATEPB::Vulkan::Vulkan(pb::Config::Render* R) {
    glfw = new GLFW();
    cam = new Camera(glm::vec3(1.0, 1.0, 1.0));
    world = new WorldSpace();
    input = new Input();
    block = new Block();
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

        default:
          return;
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

  PRIVATEPB::Vulkan::~Vulkan() {
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
    } //Catch exc

    catch (...) {
      auto rend = rendConf->GetRenderEngine();
      std::string mesoSphere = "Deleting Engine: ";
      mesoSphere += rend;
      InternalLog("Deleting Vulkan ", mesoSphere.c_str(), "Unknown Exception");
    }; //Catch exc

    delete glfw;
    delete cam;
  }; //~Vulkan