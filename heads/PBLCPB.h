#pragma once
#include <pbMacros.h>


namespace pb {
 
  //CONFIGS
  namespace Config {

    //PUBLIC UTIL CONFIG
    struct Utils {
      void SetLogBuffer(std::ostream* logBuffer);

      std::ostream* GetLogBuffer();

    private:
      std::ostream* logBuffer = &std::cout;

    }; //UTILS


      //PUBLIC RENDER CONFIG
      struct Render {
        void SetRenderEngine(UINT renderEngine);
        void SetWindowWidth(UINT windowWidth);
        void SetWindowHeight(UINT windowHeight);
        void SetWindowName(const char* windowName);
        void SetAppVersion(const char* version);


        UINT GetRenderEngine();
        UINT GetWindowWidth();
        UINT GetWindowHeight();
        const char* GetWindowName();
        const char* GetAppVersion();


      private:
        const char* WindowName = "Polyblock Engine";
        const char* AppVersion = "0.0.1.0";
        UINT WindowHeight = NULL; 
        UINT WindowWidth = NULL; 
        UINT RenderEngine = VULKAN13;

      }; //RENDER


      void AddConfig(pb::Config::Utils* U);
      void AddConfig(pb::Config::Render* R);
      void AddConfig(std::shared_ptr<pb::Config::Utils> U);
      void AddConfig(std::shared_ptr<pb::Config::Render> R);

      void ConfirmConfigs();

  }; //CONFIG


  //UTILS
  namespace Utils {
    struct Output {
      static void FlushtoLog();
      static void WritetoLog(const std::string str);
      static void WritetoLog(const char* str);
      static void WritetoTimedLog(const std::string str);
      static void WritetoTimedLog(const char* str);

    }; //OUTPUT

  }; //UTILS


  namespace Client {
    void ConfirmClients();
  };

  extern void RunRender();

}; //POLYBLOCK





  namespace GFX {

    class DirectX {

    };
    
    class GLFW {
      struct WorkArea {
        int xpos, ypos;
        int width, height;

        WorkArea(GLFWmonitor* monitor);
      }; //workArea


      struct Monitor {
        const WorkArea* workArea;
        const GLFWvidmode* vidMode;
        GLFWmonitor* mon;

        Monitor(GLFWmonitor* monitor);
      }; //Monitor


      struct Window {
        GLFW::Monitor* mon;
        GLFWwindow* win;
        std::string winName;
        int winHeight;
        int winWidth;
        INT8 indice = 1;

        Window(GLFW::Monitor* monitor, const char* winname, int winwidth, int winheight);
        Window(GLFW::Monitor* monitor, std::string winname, int winwidth, int winheight);
        Window(GLFW::Monitor* monitor, const char* winname);
        Window(GLFW::Monitor* monitor, std::string winname);
      }; //window


    protected:
      Monitor* primMon;
      Window* primWin;

      GLFW() = default;
      void createWindow(int winwidth, int winheight, std::string winname);
      void createWindow(int winwidth, int winheight, const char* winname);
      void createWindow(std::string winname);
      void createWindow(const char* winname);

    }; //GLFW

    class OpenGL :
      GLFW {

    };

    class Vulkan :
      GLFW {

      struct pbInit {
        const std::vector<const char*> deviceExtensions = {
          VK_KHR_SWAPCHAIN_EXTENSION_NAME
        }; //device Enxtensions

        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

        struct QFamIndices {
          std::optional<uint32_t> gfxFam;
          std::optional<uint32_t> presFam;

          bool isComplete();

          QFamIndices() = default;
        }; //QFamIndices

        const std::vector<const char*> validLayers = {
        "VK_LAYER_KHRONOS_validation" };

        struct SwapChainSupportDetails {
          VkSurfaceCapabilitiesKHR capabilities;
          std::vector<VkSurfaceFormatKHR> formats;
          std::vector<VkPresentModeKHR> presentModes;
        }; //SwapChainSupportDetails

        pbInit(GLFWwindow* primWin);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice d);
        void populateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        QFamIndices findQFams(VkPhysicalDevice d);

        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
          const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
          VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
          void* pUserData);

      }; //pbInit


      struct pbPipeline {

        pbPipeline();

      }; //pbPipeLine


      struct pbControl {

      }; //pbControl


      struct pbMain {

      }; //pbMain

      Vulkan();

    }; //Vulkan
    
    class Render {
      union API {
        Vulkan* Vulkan;
        OpenGL* OpenGL;
        DirectX* DirectX;
      }; //API

      void RunRender();
    }; //Render
  } //GFX


  struct Client {
    static void AddClient();
    static void Confirm();
    static void Start();
  }; //Client
