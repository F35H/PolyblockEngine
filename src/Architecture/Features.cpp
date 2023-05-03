#include <PRVTPB.h>

/*CONSTRUCTORS*/
inline pb::Feature::Scene::Scene() {}
inline pb::Feature::Scene::Scene(const char* name) { SetName(name); }
inline pb::Feature::Scene::Scene(std::vector<const char*> filenames) {
  auto scene = pb::Utils::Input::SceneFromFiles(filenames);

  CopyFromExtern(scene);
  delete scene;
}; //Scene Filename

inline pb::Feature::Camera::Camera() noexcept {};
inline pb::Feature::Camera::Camera(const char* name) {
  Name = name;
}; //Cam Ctor - Name
inline pb::Feature::Camera::Camera(const char* filename, const char* name) {
  auto cam = pb::Utils::Input::CamFromFile(filename, name);

  CopyFromExtern(cam);
  delete cam;
}; //Camera filename

/* GETTERS */
inline const char* pb::Feature::Feature::GetName() noexcept { return Name; };
inline std::array<float, 3> pb::Feature::Camera::GetWorldPos() noexcept { return WorldPos; };
inline std::array<float, 3> pb::Feature::Camera::GetViewDirection() noexcept { return ViewDirection; };
inline float pb::Feature::Camera::GetNearClip() noexcept { return NearClip; };
inline float pb::Feature::Camera::GetFarClip() noexcept { return FarClip; };
inline UINT pb::Feature::Camera::GetFOVUnit() noexcept { return FOVUnit; };
inline float pb::Feature::Camera::GetFOV() noexcept { return FOV; }

/* SETTERS */
inline void pb::Feature::Feature::SetName(const char* name) noexcept { Name = name; };
inline void pb::Feature::Camera::SetWorldPos(const float x, const float y, const float z) noexcept {
  WorldPos[0] = x;
  WorldPos[1] = y;
  WorldPos[2] = z;
}; //SetWorldPos
inline void pb::Feature::Camera::SetWorldPos(const std::array<float, 3> array) { WorldPos = array; };
inline void pb::Feature::Camera::SetViewDirection(const float x, const float y, const float z) noexcept {
  ViewDirection[0] = x;
  ViewDirection[1] = y;
  ViewDirection[2] = z;
}; //SetViewDirection
inline void pb::Feature::Camera::SetViewDirection(const std::array<float, 3> array) { ViewDirection = array; };
inline void pb::Feature::Camera::SetNearClip(const float nearClip) noexcept { NearClip = nearClip; };
inline void pb::Feature::Camera::SetFarClip(const float farClip) noexcept { FarClip = farClip; };
inline void pb::Feature::Camera::SetFOVUnit(const UINT unit) noexcept { FOVUnit = unit; };
inline void pb::Feature::Camera::SetFOV(const float fov) noexcept { FOV = fov; };

/* COPY */
inline void pb::Feature::Camera::CopyFromExtern(pb::Feature::Camera* cam) {
  FarClip = cam->GetFarClip();
  FOV = cam->GetFOV();
  FOVUnit = cam->GetFOVUnit();
  Name = cam->GetName();
  NearClip = cam->GetNearClip();
  ViewDirection = cam->GetViewDirection();
  WorldPos = cam->GetWorldPos();
}; //CopyFromExtern
inline void pb::Feature::Camera::CopyFromIntern(pb::Feature::Camera* cam) noexcept { 
  cam->SetFarClip(GetFarClip());
  cam->SetNearClip(GetNearClip());
  cam->SetWorldPos(
    GetWorldPos()[0],
    GetWorldPos()[1],
    GetWorldPos()[2]
  ); //SetWorldPos
  SetViewDirection(
    GetViewDirection()[0],
    GetViewDirection()[1],
    GetViewDirection()[2]
  ); //SetViewDirection
  cam->SetFOV(GetFOV());
  cam->SetName(GetName());
  cam->SetFOVUnit(GetFOVUnit());
}; //CopyFromIntern

inline void pb::Feature::Scene::CopyFromExtern(pb::Feature::Scene* scene) {
  SetCamVec(scene->GetCamVec());
  SetName(scene->GetName());
}; //CopyFromExtern - Scene
inline void pb::Feature::Scene::CopyFromIntern(pb::Feature::Scene* scene) {
  scene->SetCamVec(GetCamVec());
  scene->SetName(GetName());
}; //CopyFromIntern

/* Scene Vectors */
inline void pb::Feature::Scene::AddToCamVec(pb::Feature::Camera* Camera) {
  CamVec.emplace_back(Camera);
}; //AddToCameraVec
inline void pb::Feature::Scene::SetCamVec(std::vector<pb::Feature::Camera*> camVec) {
  CamVec = camVec;
}; //AddToCameraVec
inline std::vector<pb::Feature::Camera*> pb::Feature::Scene::GetCamVec() {
  return CamVec;
}; //AddToCameraVec