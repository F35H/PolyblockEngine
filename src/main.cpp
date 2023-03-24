#include <Polyblock.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  auto Utils = new pb::Config::Utils();
  auto Render = new pb::Config::Render();

  Render->SetRenderEngine(VULKAN13);
  Render->SetDirectXInstanceID(hInstance);
  Render->SetDirectXCmdShow(nCmdShow);

  pb::Config::AddConfig(Utils);
  pb::Config::AddConfig(Render);
  pb::Config::ConfirmConfigs();

  pb::Client::ConfirmClients();
  pb::RunRender();

  return 0;
}