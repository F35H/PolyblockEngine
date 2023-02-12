#include <PRVTPB.h>


namespace Vulkan{
  class Comm {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    std::vector<VkImageView> swapChainImageViews;
    VkDescriptorSetLayout descriptorSetLayout;
    VkRenderPass renderPass;
    VkExtent2D swapChainExtent;
    VkDevice device;
    VkQueue graphicsQueue;

    ~Comm() {
    }; //destrctor

    friend class Center;
    friend class Init;
    friend class Pipe;
    friend class Cmnd;
    friend class Loop;
  }; //comm

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

  class Pipe {
    Comm* Cmn;
    GLFWwindow* window;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

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

    Pipe(Comm* C, GLFWwindow* win) : Cmn(C), window(win) {};

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
      InternalLog("Creating Vulkan Pipeline " , " Creating Image Chain " , " Checking for GPU Support");

      Comm::SwapChainSupportDetails swapChainSupport = 
        Cmn->querySwapChainSupport(Cmn->physicalDevice);

      InternalLog("Creating Vulkan Pipeline " , " Creating Image Chain " , " Setting GPU Options");

      VkSurfaceFormatKHR surfaceFormat = 
        chooseSwapSurfaceFormat(swapChainSupport.formats);
      VkPresentModeKHR presentMode = 
        chooseSwapPresentMode(swapChainSupport.presentModes);
      VkExtent2D extent = 
        chooseSwapExtent(swapChainSupport.capabilities);

      InternalLog("Creating Vulkan Pipeline " , " Creating Image Chain " , " Setting Chain Length");

      UINT32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
      if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
      }

      InternalLog("Creating Vulkan Pipeline " , " Creating Image Chain " , " Informing Chain");

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

      if (vkCreateSwapchainKHR(Cmn->device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        InternalReport("Creating Vulkan Pipeline " , " Creating Image Chain " , " Chain Creation Failed");
      }

      InternalLog("Creating Vulkan Pipeline " , " Creating Image Chain " , " Preparing Final Chain");

      vkGetSwapchainImagesKHR(Cmn->device, swapChain, &imageCount, nullptr);
      swapChainImages.resize(imageCount);
      vkGetSwapchainImagesKHR(Cmn->device, swapChain, &imageCount, swapChainImages.data());

      swapChainImageFormat = surfaceFormat.format;
      Cmn->swapChainExtent = extent;
    }; //CreateSwapChain

    void CreateImageViews() {
      InternalLog("Creating Vulkan Pipeline " , " Creating Image Interfaces " , " Informing Interfaces");

      Cmn->swapChainImageViews.resize(swapChainImages.size());

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

        if (vkCreateImageView(Cmn->device, &createInfo, nullptr, &Cmn->swapChainImageViews[i]) != VK_SUCCESS) {
          InternalReport("Creating Vulkan Pipeline " , " Creating Image Interfaces " , " Informing Image Interfaces Failed");
        } //If Success
      } //For Every Image
    }; //CreateImageView

    void CreateRenderPass() {
      InternalLog("Creating Vulkan Pipeline " , " Creating Image Processors " , " Assigning Image Processors");

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

      InternalLog("Creating Vulkan Pipeline " , " Creating Image Processors " , " Confirming Image Processors");

      if (vkCreateRenderPass(Cmn->device, &renderPassInfo, nullptr, &Cmn->renderPass) != VK_SUCCESS) {
        InternalReport("Creating Vulkan Pipeline " , " Creating Image Processors " , " Image Processor Creation Failed");
      }
    }; //CreateRenderPass

    void CreateDescriptorSetLayout() {
      InternalLog("Creating Vulkan Pipeline " , " Creating Image SubProcessors " , " Assigning Image SubProcessors");

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

      InternalLog(
        "Creating Vulkan Pipeline " , " Creating Image SubProcessors " , " Confirming Image SubProcessors");

      if (vkCreateDescriptorSetLayout(Cmn->device, &layoutInfo, nullptr, &Cmn->descriptorSetLayout) != VK_SUCCESS) {
        InternalReport("Creating Vulkan Pipeline " , " Creating Image SubProcessors " , " Subprocessor Creation Failed");
      }
    }; //CreateDescriptorLayout

    void CreateGraphicsPipeline() {
      InternalLog("Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Reading SPIR-V");

      auto vertShaderCode = readFile("shaders/vert.spv");
      auto fragShaderCode = readFile("shaders/frag.spv");

      InternalLog("Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Creating Shaders Interface");

      VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
      VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

      InternalLog("Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Setting Shader Uses");

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

      InternalLog("Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Creating Shader Stages");

      VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

      VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
      vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

      InternalLog("Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Creating Shader Interfaces");

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
      pipelineLayoutInfo.pSetLayouts = &Cmn->descriptorSetLayout;

      InternalLog("Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Confirming Interfaces");

      if (vkCreatePipelineLayout(Cmn->device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        InternalReport("Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Interface Confirmation Failed");
      }

      InternalLog(
        "Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Assigning Shader Stages");

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
      pipelineInfo.renderPass = Cmn->renderPass;
      pipelineInfo.subpass = 0;
      pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

      InternalLog("Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Confirming Graphic Pipeline");

      if (vkCreateGraphicsPipelines(Cmn->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        InternalReport("Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Graphics Pipeline Confirmation Failed");
      } //If

      InternalLog("Creating Vulkan Pipeline " , " Creating Graphics Pipeline " , " Clearing Unecessary Assets");

      vkDestroyShaderModule(
        Cmn->device, fragShaderModule, nullptr);
      vkDestroyShaderModule(
        Cmn->device, vertShaderModule, nullptr);
    }; //CreateGraphicsPipeline

    void CreateFramebuffers() {
      InternalLog("Creating Vulkan Pipeline " , " Creating Image Interface " , " Setting Image Interface");

      swapChainFramebuffers.resize(Cmn->swapChainImageViews.size());

      for (size_t i = 0; i < Cmn->swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            Cmn->swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = Cmn->renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = Cmn->swapChainExtent.width;
        framebufferInfo.height = Cmn->swapChainExtent.height;
        framebufferInfo.layers = 1;

        InternalLog(
          "Creating Vulkan Pipeline " , " Creating Image Interface " , " Checking Image Interface");

        if (vkCreateFramebuffer(Cmn->device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
          InternalReport("Creating Vulkan Pipeline " , " Creating Image Interface " , " Checking Image Interface Failed");
        }
      }
    }; //CreateFrameBuffers

    friend class Center;
  }; //VulkanPipe

  class Cmnd {
    Comm* Cmn;
    VkCommandPool commandPool;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    VkDescriptorPool descriptorPool;
    uint32_t currentFrame = 0;

    char mesoLog[64];
    const char* macroLog = "Integrating Commands";

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    Cmnd(Comm* C) : Cmn(C) {};

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
      VkBufferCreateInfo bufferInfo{};
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = size;
      bufferInfo.usage = usage;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      if (vkCreateBuffer(Cmn->device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
      }

      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(Cmn->device, buffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(Cmn->device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
      }

      vkBindBufferMemory(Cmn->device, buffer, bufferMemory, 0);
    } //createBuffer

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
      VkPhysicalDeviceMemoryProperties memProperties;
      vkGetPhysicalDeviceMemoryProperties(Cmn->physicalDevice, &memProperties);

      for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
          return i;
        }
      }

      throw std::runtime_error("failed to find suitable memory type!");
    } //FindMemoryType

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

      if (vkCreateImage(Cmn->device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
      }

      VkMemoryRequirements memRequirements;
      vkGetImageMemoryRequirements(Cmn->device, image, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(Cmn->device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
      }

      vkBindImageMemory(Cmn->device, image, imageMemory, 0);
    } //CreateImage

    void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
      vkEndCommandBuffer(commandBuffer);

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &commandBuffer;

      vkQueueSubmit(Cmn->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
      vkQueueWaitIdle(Cmn->graphicsQueue);

      vkFreeCommandBuffers(Cmn->device, commandPool, 1, &commandBuffer);
    }//endSingleTimeCommands

    VkCommandBuffer beginSingleTimeCommands() {
      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = commandPool;
      allocInfo.commandBufferCount = 1;

      VkCommandBuffer commandBuffer;
      vkAllocateCommandBuffers(Cmn->device, &allocInfo, &commandBuffer);

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
      else {
        throw std::invalid_argument("unsupported layout transition!");
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

      VkBufferCopy copyRegion{};
      copyRegion.size = size;
      vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

      endSingleTimeCommands(commandBuffer);
    } //CopyBuffer

    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
    }; //indices

    struct UniformBufferObject {
      alignas(16) glm::mat4 model;
      alignas(16) glm::mat4 view;
      alignas(16) glm::mat4 proj;
    }; //UniformBufferObjects

    const int MAX_FRAMES_IN_FLIGHT = 2;

    void CreateCommandPool() {
      memset(mesoLog, 0, sizeof(mesoLog));
      strcpy(mesoLog, "Creating Threads");

      InternalLog(macroLog, mesoLog, " Creating Threadpool");
      
      Comm::QueueFamilyIndices queueFamilyIndices = Cmn->FindQueueFamilies(Cmn->physicalDevice);

      VkCommandPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
      poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

      InternalLog(macroLog , mesoLog," Checking Theadpool");

      if (vkCreateCommandPool(Cmn->device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        InternalReport(macroLog , mesoLog, " Threapool Creation Failed");
      }
    }; //CreateCommandPool

    void CreateTextureImage() {
      memset(mesoLog, 0, sizeof(mesoLog));
      strcpy(mesoLog, "Creating Texture Interface");

      IL(macroLog, mesoLog, "Creating Texture");
      
      auto t = pb::Utils::Input::TextureFromFile("textures/texture.jpg");

      VkDeviceSize imageSize = t->width * t->height * 3;

      IL(macroLog, mesoLog, "Building Interface");

      VkBuffer stagingBuffer;
      VkDeviceMemory stagingBufferMemory;
      createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

      void* data;
      vkMapMemory(Cmn->device, stagingBufferMemory, 0, imageSize, 0, &data);
      memcpy(data, t->bits, static_cast<size_t>(imageSize));
      vkUnmapMemory(Cmn->device, stagingBufferMemory);

      createImage(t->width, t->height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

      IL(macroLog, mesoLog, "Assigning Interface Constants");

      TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      CopyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(t->width), static_cast<uint32_t>(t->height));
      TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

      InternalLog(macroLog, mesoLog, "Assigning Interface to Hardware");

      vkDestroyBuffer(Cmn->device, stagingBuffer, nullptr);
      vkFreeMemory(Cmn->device, stagingBufferMemory, nullptr);
    }; //CreateTextureImage

    void CreateVertexBuffer() {
      VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

      VkBuffer stagingBuffer;
      VkDeviceMemory stagingBufferMemory;
      createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

      void* data;
      vkMapMemory(Cmn->device, stagingBufferMemory, 0, bufferSize, 0, &data);
      memcpy(data, vertices.data(), (size_t)bufferSize);
      vkUnmapMemory(Cmn->device, stagingBufferMemory);

      createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

      CopyBuffer(stagingBuffer, vertexBuffer, bufferSize);

      vkDestroyBuffer(Cmn->device, stagingBuffer, nullptr);
      vkFreeMemory(Cmn->device, stagingBufferMemory, nullptr);
    }; //CreateVertexBuffer

    void CreateIndexBuffer() {
      VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

      VkBuffer stagingBuffer;
      VkDeviceMemory stagingBufferMemory;
      createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

      void* data;
      vkMapMemory(Cmn->device, stagingBufferMemory, 0, bufferSize, 0, &data);
      memcpy(data, indices.data(), (size_t)bufferSize);
      vkUnmapMemory(Cmn->device, stagingBufferMemory);

      createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

      CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

      vkDestroyBuffer(Cmn->device, stagingBuffer, nullptr);
      vkFreeMemory(Cmn->device, stagingBufferMemory, nullptr);
    }; //CreateIndexBuffer

    void CreateUniformBuffers() {
      VkDeviceSize bufferSize = sizeof(UniformBufferObject);

      uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
      uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
      uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

      for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(Cmn->device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
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

      if (vkCreateDescriptorPool(Cmn->device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
      }
    }; //CreateDescriptorPool

    void CreateDescriptorSets() {
      std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, Cmn->descriptorSetLayout);
      VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = descriptorPool;
      allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
      allocInfo.pSetLayouts = layouts.data();

      descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
      if (vkAllocateDescriptorSets(Cmn->device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
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

        vkUpdateDescriptorSets(Cmn->device, 1, &descriptorWrite, 0, nullptr);
      }
    }; //CreateDescriptorSets

    void CreateCommandBuffers() {
      commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.commandPool = commandPool;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

      if (vkAllocateCommandBuffers(Cmn->device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
      }
    }; //CreateCommandBuffers

    friend class Center;
  }; //VulkanCmnd


  class Loop {
    Comm* Cmn;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    //char mesoLog[64];
    //const char* macroLog = "Running Render";

    void RecreateSwapChain() {
      int width = 0, height = 0;
      glfwGetFramebufferSize(window, &width, &height);
      while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
      }

      vkDeviceWaitIdle(Cmn->device);

      CleanupSwapChain();

      CreateSwapChain();
      CreateImageViews();
      CreateFramebuffers();
    }

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
        if (vkCreateSemaphore(Cmn->device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
          vkCreateSemaphore(Cmn->device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
          vkCreateFence(Cmn->device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
          throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
      }
    }; //Create Sync Objects

    void RenderFrame() {
      vkWaitForFences(Cmn->device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

      uint32_t imageIndex;
      VkResult result = vkAcquireNextImageKHR(Cmn->device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

      if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
      }
      else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
      }

      updateUniformBuffer(currentFrame);

      vkResetFences(Cmn->device, 1, &inFlightFences[currentFrame]);

      vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
      recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

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
        recreateSwapChain();
      }
      else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
      }

      currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
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
    };

    void MainLoop(GLFWwindow* win) {
      CreateSyncObjects();

      while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        RenderFrame();
      } //While window should close

      vkDeviceWaitIdle(Cmn->device);
    }; //MainLoop

    friend class Center;
  }; //Loop

  struct Center :
    GLFW {

    Init* VInit;
    Pipe* VPipe;
    Cmnd* VCmnd;
    Comm* Cmn;

    //Update debugCallback

    Center(pb::Config::Render* RendConf) {
      GLFW::rendConf = RendConf;

      
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
      VPipe->CreateFramebuffers();

      VCmnd->CreateCommandPool();
      VCmnd->CreateTextureImage();
      VCmnd->CreateVertexBuffer();
      VCmnd->CreateIndexBuffer();
      VCmnd->CreateUniformBuffers();
      VCmnd->CreateDescriptorPool();
      VCmnd->CreateDescriptorSets();
      VCmnd->CreateCommandBuffers();
      
      VCmnd->MainLoop();
    }; //Vulkan13

    ~Center() {
      delete VInit;
      delete VPipe;
      delete VCmnd;
      delete Cmn;
    }; //Center

  }; //Vulkan
}

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

    InternalLog("Initializing Render Engine " , " Selecting Engine " , " Beginning Render Segments");

    //If we have not switched Engines
    switch (rend) {
    case VULKAN13:
      rendClass->V = new Vulkan(RendConf);

      //If Render Fails
      //RendConf->SetRenderEngine(DirectX);

    default:
      InternalLog("Initializing Render Engine ", " Selecting Engine ", " All Possible Renders Failed!");
      pb::Utils::Output::FlushtoLog();
      terminate();

    } //Switch
  }; //GFX Constrct

  ~GFX() {
    delete rendClass;
  }; //GFX
};

inline void pb::RunRender() {
  auto g = new PRIVATEPB::GFX();
  delete g;
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