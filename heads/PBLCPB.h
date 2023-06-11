#pragma once
#include <pbMacros.h>


namespace pb {
  namespace RenderEngine {
    enum {
      DIRECTX12,
      DIRECTX11,
      VULKAN13
    }; //RenderEngine
  }; //RenderEngine

  namespace StreamOutputs {
    enum {
      COUT,
      CERR,
      FTX //Remind self of what this is again
    }; //StreamOutputs
  }; //ConsoleStreamOutputs

  namespace Polygons {
    enum Solids {
      DIGONAL_PRISM,
      TRIANGULAR_PRISM,
      SQUARE_PRISM,
      PENTAGONAL_PRISM,
      HEXAGONAL_PRISM,
      HEPTAGONAL_PRISM,
      OCTAGONAL_PRISM,
      ENNEAGONAL_PRISM,
      DECAGONAL_PRISM
    }; // SOLIDS

    enum Antiprisms {
      DIGONAL_ANTIPRISM = DECAGONAL_PRISM + 1,
      TRIANGULAR_ANTIPRISM,
      SQUARE_ANTIPRISM,
      PENTAGONAL_ANTIPRISM,
      HEXAGONAL_ANTIPRISM,
      HEPTAGONAL_ANTIPRISM,
      OCTAGONAL_ANTIPRISM,
      ENNEAGONAL_ANTIPRISM,
      DECAGONAL_ANTIPRISM
    }; //ANTIPRISMS

    enum PlantonicSolids {
      CUBE = SQUARE_PRISM,
      TETRAHEDRON = DECAGONAL_ANTIPRISM + 1,
      OCTAHEDRON,
      ICOSAHEDRON,
      DODECAHEDRON
    }; //PlatonicSolids
  }; //Polygons

  namespace Keybindings {
    enum MiscLower {
      KEYBOARD_APOSTROPHE,
      KEYBOARD_COMMA,
      KEYBOARD_MINUS,
      KEYBOARD_PERIOD,
      KEYBOARD_SLASH,
      KEYBOARD_FORWARDSLASH = KEYBOARD_SLASH,
      KEYBOARD_SEMICOLON,
      KEYBOARD_EQUAL,
      KEYBOAD_BRACKET_LEFT_SQUARE,
      KEYBOARD_BRACKET_RIGHT_SQUARE,
      KEYBOARD_BACKSLASH,
      KEYBOARD_ACCENT_GRAVE,
      KEYBOARD_BACKTICK = KEYBOARD_ACCENT_GRAVE
    }; //MiscLower

    enum MiscUpper {
      KEYBOARD_QUOTATION = KEYBOARD_BACKTICK + 1,
      KEYBOARD_LESS_THAN,
      KEYBOARD_GREATER_THAN,
      KEYBOARD_UNDERSCORE,
      KEYBOARD_QUESTION,
      KEYBOARD_COLON,
      KEYBOARD_PLUS,
      KEYBOARD_PIPE,
      KEYBOARD_BRACKET_LEFT_CURLED,
      KEYBOARD_BRACKET_RIGHT_CURLED,
      KEYBOARD_TILDE
    }; //MiscUpper

    enum MiscOther {
      KEYBOARD_TAB = KEYBOARD_TILDE + 1,
      KEBOARD_ENTER,
      KEYBOARD_BACKSPACE,
      KEYBOARD_INSERT,
      KEYBOARD_DELETE,
      KEYBOARD_ESCAPE,
      KEYBOARD_END,
      KAYBOARD_CAPSLOCK,
      KEYBOARD_SPACE
    }; //MiscOther

    enum ScreenControl {
      KEYBOARD_PAGE_UP = KEYBOARD_SPACE + 1,
      KEYBOARD_PAGE_DOWN,
      KEYBOARD_PAGE_HOME,
      KEYBOARD_PAGE_END,
      KEYBOARD_SCROLL_LOCK,
      KEYBOARD_NUM_LOCK,
      KEYBOARD_PRINT_SCREEN,
      KEYBOARD_PAUSE
    }; //ScreenControl

    enum LeftControlKeys {
      KEYBOARD_LEFT_SHIFT = KEYBOARD_PAUSE + 1,
      KEYBOARD_LEFT_CONTROL,
      KEYBOARD_LEFT_ALT,
      KEYBOARD_LEFT_SUPER
    }; //LeftControlKeys

    enum RightControlKeys {
      KEYBOARD_RIGHT_SHIFT = KEYBOARD_LEFT_SUPER + 1,
      KEYBOARD_RIGHT_CONTROL,
      KEYBOARD_RIGHT_ALT,
      KEYBOARD_RIGHT_SUPER,
      KEYBOARD_MENU
    }; //RightControlKeys

    enum Arrows {
      KEYBOARD_RIGHT = KEYBOARD_MENU + 1,
      KEYBOARD_LEFT,
      KEYBOARD_DOWN,
      KEYBOARD_UP
    }; //Arrows

    enum FunctionKeys {
      KEYBOARD_FUNCTION_1 = KEYBOARD_UP + 1,
      KEYBOARD_FUNCTION_2,
      KEYBOARD_FUNCTION_3,
      KEYBOARD_FUNCTION_4,
      KEYBOARD_FUNCTION_5,
      KEYBOARD_FUNCTION_6,
      KEYBOARD_FUNCTION_7,
      KEYBOARD_FUNCTION_8,
      KEYBOARD_FUNCTION_9,
      KEYBOARD_FUNCTION_10,
      KEYBOARD_FUNCTION_11,
      KEYBOARD_FUNCTION_12,
      KEYBOARD_FUNCTION_13,
      KEYBOARD_FUNCTION_14,
      KEYBOARD_FUNCTION_15,
      KEYBOARD_FUNCTION_16,
      KEYBOARD_FUNCTION_17,
      KEYBOARD_FUNCTION_18,
      KEYBOARD_FUNCTION_19,
      KEYBOARD_FUNCTION_20,
      KEYBOARD_FUNCTION_21,
      KEYBOARD_FUNCTION_22,
      KEYBOARD_FUNCTION_23,
      KEYBOARD_FUNCTION_24,
      KEYBOARD_FUNCTION_25
    }; //FunctionKeys

    enum Numpad {
      KEYBOARD_NUMPAD_0 = KEYBOARD_FUNCTION_25 + 1,
      KEYBOARD_NUMPAD_1,
      KEYBOARD_NUMPAD_2,
      KEYBOARD_NUMPAD_3,
      KEYBOARD_NUMPAD_4,
      KEYBOARD_NUMPAD_5,
      KEYBOARD_NUMPAD_6,
      KEYBOARD_NUMPAD_7,
      KEYBOARD_NUMPAD_8,
      KEYBOARD_NUMPAD_9,
      KEYBOARD_NUMPAD_DECIMAL,
      KEYBOARD_NUMPAD_DIVIDE,
      KEYBOARD_NUMPAD_MULTIPLY,
      KEYBOARD_NUMPAD_SUBTRACT,
      KEYBOARD_NUMPAD_ADD,
      KEYBOARD_NUMPAD_ENTER,
      KEYBOARD_NUMPAD_EQUAL,
    }; //NumPad

    enum NumbersLower {
      KEYBOARD_ZERO = KEYBOARD_NUMPAD_EQUAL + 1,
      KEYBOARD_ONE,
      KEYBOARD_TWO,
      KEYBOARD_THREE,
      KEYBOARD_FOUR,
      KEYBOARD_FIVE,
      KEYBOARD_SIX,
      KEYBOARD_SEVEN,
      KEYBOARD_EIGHT,
      KEYBOARD_NINE
    }; //NumbersLower

    enum NumbersUpper {
      KEYBOARD_RIGHT_PARENTHESES = KEYBOARD_NINE + 1,
      KEYBOARD_LEFT_PARENTHESES,
      KEYBOARD_STAR,
      KEYBOARD_AMPERSAND,
      KEYBOARD_CARROT,
      KEYBOARD_PERCENT,
      KEYBOARD_MODOLUS = KEYBOARD_PERCENT,
      KEYBOARD_DOLLAR,
      KEYBOARD_HASHTAG,
      KEYBOARD_AT,
      KEYBOARD_EXCLAMATION
    }; //NumbersUpper

    enum LettersUpper {
      KEYBOARD_A_UPPER = KEYBOARD_EXCLAMATION + 1,
      KEYBOARD_B_UPPER,
      KEYBOARD_C_UPPER,
      KEYBOARD_D_UPPER,
      KEYBOARD_E_UPPER,
      KEYBOARD_F_UPPER,
      KEYBOARD_G_UPPER,
      KEYBOARD_H_UPPER,
      KEYBOARD_I_UPPER,
      KEYBOARD_J_UPPER,
      KEYBOARD_K_UPPER,
      KEYBOARD_L_UPPER,
      KEYBOARD_M_UPPER,
      KEYBOARD_N_UPPER,
      KEYBOARD_O_UPPER,
      KEYBOARD_P_UPPER,
      KEYBOARD_Q_UPPER,
      KEYBOARD_R_UPPER,
      KEYBOARD_S_UPPER,
      KEYBOARD_T_UPPER,
      KEYBOARD_U_UPPER,
      KEYBOARD_V_UPPER,
      KEYBOARD_W_UPPER,
      KEYBOARD_X_UPPER,
      KEYBOARD_Y_UPPER,
      KEYBOARD_Z_UPPER
    }; //LettersUpper

    enum LettersLower {
      KEYBOARD_A_LOWER = KEYBOARD_Z_UPPER + 1,
      KEYBOARD_B_LOWER,
      KEYBOARD_C_LOWER,
      KEYBOARD_D_LOWER,
      KEYBOARD_E_LOWER,
      KEYBOARD_F_LOWER,
      KEYBOARD_G_LOWER,
      KEYBOARD_H_LOWER,
      KEYBOARD_I_LOWER,
      KEYBOARD_J_LOWER,
      KEYBOARD_K_LOWER,
      KEYBOARD_L_LOWER,
      KEYBOARD_M_LOWER,
      KEYBOARD_N_LOWER,
      KEYBOARD_O_LOWER,
      KEYBOARD_P_LOWER,
      KEYBOARD_Q_LOWER,
      KEYBOARD_R_LOWER,
      KEYBOARD_S_LOWER,
      KEYBOARD_T_LOWER,
      KEYBOARD_U_LOWER,
      KEYBOARD_V_LOWER,
      KEYBOARD_W_LOWER,
      KEYBOARD_X_LOWER,
      KEYBOARD_Y_LOWER,
      KEYBOARD_Z_LOWER
    }; //LettersLower
  }; //Keybindings

  namespace Inputs {
    enum {
      OUTPUT = Keybindings::KEYBOARD_Z_LOWER + 1
    }; //enum
  }; //OtherOutputs 

  namespace Outputs {
    enum Motion {
      ARCHED,
      JAGGED,
      TELEPORT,
      LINE
    }; //enum

    enum TriggerOutputs {
      TRIGGER = LINE + 1,
      MOTION
    }; //TriggerOutputs
  }; //Outputs

  namespace Features {
    enum {
      CAMERA
    }; //enum
  }; //Features

  namespace Units {
    enum {
      RADIAN,
      DEGREE
    }; //enum
  }; //Namespace

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
        UINT RenderEngine = RenderEngine::VULKAN13;
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
      void SetName(const char* name) noexcept;

      const char* GetName() noexcept;
      
    protected:
      const char* Name = "AnonFeature";
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
      Camera(const char* filename, const char* name) noexcept;
      Camera(const char* name) noexcept;
      Camera() noexcept;

      void SetWorldPos(const std::array<float, 3> array) noexcept;
      void SetWorldPos(const float x, const float y, const float z) noexcept;
      void SetViewDirection(const std::array<float, 3> array) noexcept;
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

      void CopyFromExtern(pb::Feature::Camera* camera);
      void CopyFromIntern(pb::Feature::Camera* camera);

    private:
      std::array<float, 3> WorldPos = {0, 0, 0};
      std::array<float, 3> ViewDirection = {0, 0, 0};
      UINT FOVUnit = 0x00;
      float FOV = 45.0f;
      float FarClip = 1000.0f;
      float NearClip = 1.0f;
    }; //Camera

    struct Scene : 
      Feature {
      Scene(std::vector<const char*> filenames);
      Scene(const char*);
      Scene();

      void AddToCamVec(Camera* Camera);
      void CopyFromExtern(pb::Feature::Scene* scene);
      void CopyFromIntern(pb::Feature::Scene* scene);

      void SetCamVec(std::vector<pb::Feature::Camera*> camVec);

      std::vector<Camera*> GetCamVec();

    private:
      std::vector<Camera*> CamVec;
    }; //Scene

    void AddFeature(Camera* Cam);
    void AddFeature(Scene* Scene);

    void ConfirmFeatures();
  }; //FEATURES

  namespace Control {
    struct Trigger {
      void SetInput(const UINT inputType, const char* inputName) noexcept;
      void SetInputType(const UINT inputType) noexcept;
      void SetInputName(const char* inputName) noexcept;

      void SetOutput(const UINT outputType, const char* outputName) noexcept;
      void SetOutputType(const UINT outputType) noexcept;
      void SetOutputName(const char* outputName) noexcept;

      UINT GetInputType() noexcept;
      const char* GetInputName() noexcept;

      UINT GetOutputType() noexcept;
      const char* GetOuputName() noexcept;
    
    private:
      UINT InputType = 0x00;
      UINT OutputType = 0x00;

      const char* OutputName;
      const char* InputName;
    }; //Trigger

    struct Motion :
      Trigger {
      //Inputs
      void SetInputLocation(const float x, const float y, const float z) noexcept; 
      //void SetInputVolume(const float x, const float y, const float z) noexcept; 
      //void SetInputFeature(const float x, const float y, const float z) noexcept; 

      void SetOutputLocation(const float x, const float y, const float z) noexcept;
      void SetOutputFeature(INT8 featType, const char* featName) noexcept;
      void SetOutputAccel(const float x) noexcept;
      void SetOutputGravity(const float x) noexcept;

      std::array<float, 3> GetInputLocation() noexcept;
      std::array<float, 3> GetOutputLocation() noexcept;
      float GetOutputAccel() noexcept;
      float GetOutputGravity() noexcept;
      float GetOutputFeature() noexcept;

    private:
      std::array<float, 3> OutputLocation;
      std::array<float, 3> InputLocation;
      float Gravity = 1.0f;
      float Accel = 1.0f;
      INT8 featureType;
      const char* InputFeatureName;
      const char* OutputFeatureName;
      const char* MotionName;
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
      }; //Texture

      static Texture* TextureFromFile(const char* filename);
      static pb::Feature::Scene* SceneFromFiles(std::vector<const char*> filenames);
      static pb::Feature::Camera* CamFromFile(const char* filename, const char* name);
    }; //Input
  }; //UTILS


  namespace Client {
    void ConfirmClients();
  };

  extern void RunRender();

}; //POLYBLOCK

  struct Client {
    static void AddClient();
    static void Confirm();
    static void Start();
  }; //Client
