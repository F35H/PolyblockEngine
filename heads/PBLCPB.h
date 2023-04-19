#pragma once
#include <pbMacros.h>


namespace pb {
 
  //CONFIGS
  namespace Config {

    //PUBLIC UTIL CONFIG
    struct Utils {
      void SetLogBuffer(std::ostream* logBuffer) noexcept;
      void SetLogDelimiter(std::string delimeter) noexcept;
      void SetLogSegments(INT segment) noexcept;
      void SetLogTimed(bool timed) noexcept;

      std::ostream* GetLogBuffer() noexcept;
      std::string GetLogDelimiter() noexcept;
      INT GetLogSegments() noexcept;
      bool IsLogTimed() noexcept;

    private:
      std::ostream* LogBuffer = &std::cout;
      
      std::string Delimeter = " | ";
      
      INT Segments = 3;

      bool Timed = true;

    }; //UTILS


      //PUBLIC RENDER CONFIG
      struct Render {
        void SetRenderEngine(UINT renderEngine);
        void SetWindowWidth(UINT windowWidth);
        void SetWindowHeight(UINT windowHeight);
        void SetWindowName(const char* windowName);
        void SetAppVersion(const char* version);
        void SetFullscreenBool(bool fScreen);
        void SetDirectXInstanceID(HINSTANCE hInstance);
        void SetDirectXCmdShow(INT nCmdShow);


        UINT GetRenderEngine();
        UINT GetWindowWidth();
        UINT GetWindowHeight();
        bool GetFullscreenBool();
        const char* GetWindowName();
        const char* GetAppVersion();
        HINSTANCE GetDirectXInstanceID();
        INT GetDirectXCmdShow();

      private:
        UINT WindowHeight = NULL; 
        UINT WindowWidth = NULL; 
        UINT RenderEngine = VULKAN13;
        bool FullScreen = false;
        const char* AppVersion = "0.0.1.0";
        const char* WindowName = "Polyblock Engine";
        HINSTANCE HInstance = NULL;
        INT NCmdShow = NULL;

      }; //RENDER


      void AddConfig(pb::Config::Utils* U);
      void AddConfig(pb::Config::Render* R);

      void ConfirmConfigs();

  }; //CONFIG

  namespace Feature {
    struct Feature {
      void SetName() noexcept;
      
      const char* GetName() noexcept;

    protected:
      const char* Name;
    }; //Feature
    
    struct Block {
      //Texture
      //Color
      //Polyhedra
      //Destruction
      //Ability
      //Size
    }; //Block

    struct Camera :
      Feature {
      void SetWorldPos(const float x, const float y, const float z) noexcept;
      void SetViewDirection(const float x, const float y, const float z) noexcept;
      void SetNearClip(const float nearClip) noexcept;
      void SetFarClip(const float farClip) noexcept;
      void SetFOVUnit(const UINT unit) noexcept;
      void SetFOV(const float fovDeg) noexcept;

      std::array<float, 3> GetWorldPos() noexcept;
      std::array<float, 3> GetViewDirection() noexcept;
      float GetNearClip() noexcept;
      float GetFarClip() noexcept;
      UINT GetFOVUnit() noexcept;
      float GetFOV() noexcept;

    private:
      std::array<float, 3> WorldPos = {0, 0, 0};
      std::array<float, 3> ViewDirection = {0, 0, 0};
      UINT FOVUnit = 0x00;
      float FOV = 45.0f;
      float FarClip = 1000.0f;
      float NearClip = 1.0f;
    }; //Camera

    void AddFeature(Camera* Cam);

    void ConfirmFeatures();
  }; //FEATURES

  namespace Control {
    struct Trigger {
      void SetInput(const UINT inputType) noexcept;
      void SetInputControl(const char* internal) noexcept;

      void SetOutput(const UINT outputType) noexcept;
      void SetOutputControl(const char* internal) noexcept;

      UINT GetInput() noexcept;
      const char* GetInputControl() noexcept;

      UINT GetOutput() noexcept;
      const char* GetOnputControl() noexcept;
    
    private:
      UINT Input = 0x00;
      UINT Output = 0x00;

      const char* OutputControl;
      const char* InputControl;
    }; //Trigger

    struct Motion :
      Trigger {
      //Inputs
      void SetInputLocation(const float x, const float y, const float z) noexcept;
      void SetInputVolume(const float x, const float y, const float z) noexcept;
      void SetInputFeature(const float x, const float y, const float z) noexcept; 
      void SetInputVerticleAngle(const float angle) noexcept;
      void SetInputHorizontalAngle(const float angle) noexcept;
      
      void SetOutputFeature(const float x, const float y, const float z) noexcept;
      void SetOutputLocation(const float x, const float y, const float z) noexcept;
      void SetOutputVelocity(const float x) noexcept;

      std::array<float, 3> GetInputLocation() noexcept;
      
      std::array<float, 3> GetOutputLocation() noexcept;
      float GetOutputVelocity() noexcept;

    private:
      std::array<float, 3> OutputLocation;
      std::array<float, 3> InputLocation;
      float Velocity = 1.0f;
    }; //Motion

    void AddMotion(pb::Control::Motion* M);
    void AddTrigger(pb::Control::Trigger* T);

    void ConfirmControls();
  }; //Control

  //UTILS
  namespace Utils {
    struct Output {
      static void FlushtoLog();
      
      static void WritetoLog(std::string str) noexcept;
      static void WritetoLog(std::string macro, std::string micro) noexcept;
      static void WritetoLog(std::string macro, std::string meso, std::string micro) noexcept;
      
      static void WritetoLog(const char* str) noexcept;
      static void WritetoLog(const char* macro, const char* micro) noexcept;
      static void WritetoLog(const char* macro, const char* meso, const char* micro) noexcept;

      static void WritetoTimedLog(const char* str) noexcept;
      static void WritetoTimedLog(std::string macro, std::string micro) noexcept;
      static void WritetoTimedLog(std::string macro, std::string meso, std::string micro) noexcept;

      static void WritetoTimedLog(std::string str) noexcept;
      static void WritetoTimedLog(const char* macro, const char* micro) noexcept;
      static void WritetoTimedLog(const char* macro, const char* meso, const char* micro) noexcept;


      static void ThrowError(std::string str) noexcept;
      static void ThrowError(std::string macro, std::string micro) noexcept;
      static void ThrowError(std::string macro, std::string meso, std::string micro) noexcept;

      static void ThrowError(const char* str) noexcept;
      static void ThrowError(const char* macro, const char* micro) noexcept;
      static void ThrowError(const char* macro, const char* meso, const char* micro) noexcept;

      static void ThrowTimedError(const char* str) noexcept;
      static void ThrowTimedError(std::string macro, std::string micro) noexcept;
      static void ThrowTimedError(std::string macro, std::string meso, std::string micro) noexcept;

      static void ThrowTimedError(std::string str) noexcept;
      static void ThrowTimedError(const char* macro, const char* micro) noexcept;
      static void ThrowTimedError(const char* macro, const char* meso, const char* micro) noexcept;
    }; //OUTPUT

    struct Input {
      struct Texture {
        INT64 height = 0;
        INT64 width = 0;
        BYTE* bits = 0;
      };

      static Texture* TextureFromFile(const char* filename);
    }; //Input

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
