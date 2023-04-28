#include <PRVTPB.h>

struct PRIVATEPB::Features {
  Features() :
    CameraHash({})
  {};

  //Getters
  std::unordered_map
    <const char*, pb::Feature::Camera>
    GetCameraHash()
  {
    return CameraHash;
  };

  bool GetConfirmed() { return Confirmed; };

  //Setters
  void SetConfirmed(bool b) { Confirmed = b; };

private:
  std::unordered_map<const char*, pb::Feature::Camera> CameraHash;

  bool Confirmed;
}; //FEATURES

  PRIVATEPB::GFX::GFX(pb::Config::Render* rendConf, PRIVATEPB::Features* features, PRIVATEPB::Control* control) {
    UINT rend = rendConf->GetRenderEngine();
    bool init = 0;

    InternalLog("Initializing Render Engine " , 
      " Selecting Engine " , 
      " Beginning Render Segments"
    ); //InternalLog

    void* render = NULL;

    try {
      //Consistent Loop
      while (1) {
        //Select Version Here
        switch (rend) {
        case DIRECTX12:
          if (LINUX) {
            InternalReport(
              "Initializing Render Engine ",
              " Selecting Engine ",
              "ImproperOS: LINUX");
          } //InternalReport - LINUX

          if (OSX) {
            InternalReport(
              "Initializing Render Engine ",
              " Selecting Engine ",
              "ImproperOS: OSX");
          } //InternalReport - OSX

          render = new DirectX12(rendConf, features, control);
          delete render;
          return;

        case VULKAN13:
          if (OSX) { 
            InternalReport(
              "Initializing Render Engine ", 
              " Selecting Engine ", 
              "ImproperOS: OSX"); 
          } //InternalReport - OSX

          render = new Vulkan13(rendConf, features, control);
          delete render;
          return;

        default:
          InternalLog("Initializing Render Engine ", " Selecting Engine ", " All Possible Renders Failed!");
          pb::Utils::Output::FlushtoLog();
          terminate();

        } //Switch
      } //While
    } //Try

    //If Error Found
    catch (const std::exception& exc) {
      InternalLog("Initializing Render Engine ", " Selecting Engine ", exc.what());

      if (render != NULL) { delete render; };

      rend -= 1;
      
      rendConf
      ->SetRenderEngine(rend);
    } //Catch

    catch (...) {
      InternalLog("Initializing Render Engine ", " Selecting Engine ", "Unknown Exception Occured");

      delete render;

      rend -= 1;

      rendConf
        ->SetRenderEngine(rend);
    } //Catch
  }; //GFX Constrct

  PRIVATEPB::GFX::~GFX() {
  }; //GFX


void pb::Config::Render::SetRenderEngine(UINT renderEngine) { RenderEngine = renderEngine; };
void pb::Config::Render::SetWindowWidth(UINT windowWidth) { WindowWidth = windowWidth; };
void pb::Config::Render::SetWindowHeight(UINT windowHeight) { WindowHeight = windowHeight; };
void pb::Config::Render::SetWindowName(const char* windowName) { WindowName = windowName; };
void pb::Config::Render::SetAppVersion(const char* version) { AppVersion = version; };
void pb::Config::Render::SetFullscreenBool(bool fScreen) { FullScreen = fScreen; };
void pb::Config::Render::SetDirectXInstanceID(HINSTANCE hInstance) { HInstance = hInstance; };
void pb::Config::Render::SetDirectXCmdShow(INT nCmdShow) { NCmdShow = nCmdShow; };

HINSTANCE pb::Config::Render::GetDirectXInstanceID() { return HInstance; };
UINT pb::Config::Render::GetRenderEngine() { return RenderEngine; };
UINT pb::Config::Render::GetWindowWidth() { return WindowWidth; };
UINT pb::Config::Render::GetWindowHeight() { return WindowHeight; };
INT pb::Config::Render::GetDirectXCmdShow() { return NCmdShow; };
const char* pb::Config::Render::GetWindowName() { return WindowName; };
const char* pb::Config::Render::GetAppVersion() { return AppVersion; };
bool pb::Config::Render::GetFullscreenBool() { return FullScreen; };
