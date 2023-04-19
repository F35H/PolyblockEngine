#include <Polyblock.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  auto Utils = new pb::Config::Utils();
  auto Render = new pb::Config::Render();

  //auto Camera = new pb::Features::Camera();

  Render->SetRenderEngine(VULKAN13);
  Render->SetDirectXInstanceID(hInstance);
  Render->SetDirectXCmdShow(nCmdShow);
  Render->SetWindowWidth(600);
  Render->SetWindowHeight(600);

  pb::Config::AddConfig(Utils);
  pb::Config::AddConfig(Render);
  pb::Config::ConfirmConfigs();

  //Camera->SetWorldPos();
  //Camera->SetViewDirection();
  //Camera->SetViewFarCut();
  //Camera->SetViewNearCut();

  //pb::Features::AddFeature(Camera);

  pb::Client::ConfirmClients();
  pb::RunRender();

  return 0;
}