#include <Polyblock.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  auto Utils = new pb::Config::Utils();
  auto Render = new pb::Config::Render();

  auto Camera = new pb::Feature::Camera();

  Render->SetRenderEngine(pb::RenderEngine::VULKAN13);
  Render->SetDirectXInstanceID(hInstance);
  Render->SetDirectXCmdShow(nCmdShow);
  Render->SetWindowWidth(600);
  Render->SetWindowHeight(600);

  pb::Config::AddConfig(Utils);
  pb::Config::AddConfig(Render);
  pb::Config::ConfirmConfigs();

  Camera->SetWorldPos(0, 0, 0);
  Camera->SetViewDirection(0, 0, 0);
  Camera->SetFarClip(1000);
  Camera->SetNearClip(10);
  Camera->SetName("Test");

  pb::Feature::AddFeature(Camera);

  pb::Client::ConfirmClients();
  pb::RunRender();

  return 0;
}