#include <PRVTPB.h>

  PRIVATEPB::GFX::GFX(pb::Config::Render* rendConf) {
    union API {
      PRIVATEPB::Vulkan* V;
      PRIVATEPB::DirectX* D;
      PRIVATEPB::OpenGL* O;

      ~API() {};
    }; //Unions

    API* rendClass = new API();

    UINT rend = rendConf->GetRenderEngine();
    bool init = 0;

    InternalLog("Initializing Render Engine " , " Selecting Engine " , " Beginning Render Segments");

    //If we have not switched Engines
    switch (rend) {
    case VULKAN13:
      rendClass->V = new Vulkan(rendConf);
      delete rendClass->V;

      //If Render Fails
      //RendConf->SetRenderEngine(DirectX);

    default:
      InternalLog("Initializing Render Engine ", " Selecting Engine ", " All Possible Renders Failed!");
      pb::Utils::Output::FlushtoLog();
    } //Switch

    delete rendClass;
  }; //GFX Constrct

  PRIVATEPB::GFX::~GFX() {
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