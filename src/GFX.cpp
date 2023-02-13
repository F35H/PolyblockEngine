#include <PRVTPB.h>

struct PRIVATEPB::GFX {
  union API {
    Vulkan* V;
    DirectX* D;
    OpenGL* O;

    ~API(){};
  }; //Union

  API* rendClass = new API();

  GFX(pb::Config::Render* rendConf) {
    
    UINT rend = rendConf->GetRenderEngine();
    bool init = 0;

    InternalLog("Initializing Render Engine " , " Selecting Engine " , " Beginning Render Segments");

    //If we have not switched Engines
    switch (rend) {
    case VULKAN13:
      rendClass->V = new Vulkan(rendConf);

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
}; //GFX


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