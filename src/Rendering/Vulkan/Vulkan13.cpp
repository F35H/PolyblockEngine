#include <PRVTPB.h>

/* MACROS */
#define STAGE_BUFF_BITS                   VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
#define VERTEX_BUFF_BITS                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
#define INDEX_BUFF_BITS                   VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
#define UNIFORM_BUFF_BITS             VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT

#define STB_IMAGE_IMPLEMENTATION

inline constexpr INT8 VERTEX_BUFF = 0;
inline constexpr INT8 INDICE_BUFF = 1;
inline constexpr INT8 UNIFORM_BUFF = 2;


/* INCLUDES */
#include <stb_image.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


/* Scene Objects */
struct Object {
  Object() : WorldPos(glm::mat4(1.0f)) {
  }; //Object

  const char* SetName(const char* str) {
    Name = str;

    return Name;
  }; //SetObjectName
  void SetWorldPos(glm::vec3* pos) {
    WorldPos[3][0] = -pos->x;
    WorldPos[3][1] = -pos->y;
    WorldPos[3][2] = -pos->z;
  }; //SetWorldPOs
  void SetWorldPos(float x, float y, float z) {
    WorldPos[3][0] = -x;
    WorldPos[3][1] = -y;
    WorldPos[3][2] = -z;
  }; //SetWorldPOs
  void UpWorldPos(glm::vec3* pos) {
    WorldPos[3][0] += -pos->x;
    WorldPos[3][1] += -pos->y;
    WorldPos[3][2] += -pos->z;
  }; //UpWorldPos
  void DeltaSetWorldPos(glm::vec3* pos, float delta) {
    WorldPos[3][0] = -pos->x * delta;
    WorldPos[3][1] = -pos->y * delta;
    WorldPos[3][2] = -pos->z * delta;
  }; //DeltaSetWorldPos
  void DeltaUpWorldPos(glm::vec3* pos, float delta) {
    WorldPos[3][0] += -pos->x * delta;
    WorldPos[3][1] += -pos->y * delta;
    WorldPos[3][2] += -pos->z * delta;
  }; //DeltaUpWorldPos
  glm::mat4 GetWorldPosMat() {
    return WorldPos;
  }; //GetWorldPos
  glm::vec3 GetWorldPosVec() {
    return {
      WorldPos[3][0],
      WorldPos[3][1],
      WorldPos[3][2]
    }; //return
  }; //GetWorldPos
  const char* GetName() {
    return Name;
  }; //Get Name
protected:
  glm::mat4x4 WorldPos;
  const char* Name;
}; //Object

struct Camera :
  Object {
  Camera(pb::Feature::Camera* cam, float width, float height) {
    auto wp = cam->GetWorldPos(); //WorldPos
    auto vm = cam->GetViewDirection(); //ViewMat
    FOV = cam->GetFOV();
    NearClip = cam->GetNearClip();
    FarClip = cam->GetFarClip();
    FOVUnit = cam->GetFOVUnit();
    Name = cam->GetName();

    SetWorldPos(wp[0], wp[1], wp[2]);
    SetViewMatrix(vm[0], vm[1], vm[2]);
    SetPerspectiveProj(width, height);
  }; //Camera

  glm::mat4 SetPerspectiveProj(float width, float height) {
    if (FOVUnit == RADIAN) {
      ProjMat = glm::perspective(FOV, width / height, NearClip, FarClip);
    } //If Radian
    else if (FOVUnit == DEGREE) {
      ProjMat = glm::perspective(glm::radians(FOV), width / height, NearClip, FarClip);
    } // else if degree
    else {
      InternalReport("Writing Perspective Projection", "Selecting Between Units", "Improper Unit Selected");
    }; //Throw Error 
    ProjMat[1][1] *= -1;
    return ProjMat;
  }; //SetPerspectiveProj

  glm::mat4 SetViewMatrix(float x, float y, float z) {
    ViewMat = glm::lookAt(
      glm::vec3(2.0f, 2.0f, 2.0f), //Eye
      glm::vec3(
        -x + WorldPos[3][0], //Look At X
        -y + WorldPos[3][1], //Look At Y
        -z + WorldPos[3][2]), //Look At Z
      glm::vec3(0.0f, 1.0f, 0.0f)); //Height
    return ViewMat;
  }; //SetViewMatrix

  glm::mat4 GetPerspectiveProj() {
    return ProjMat;
  }; //GetPerspectiveProj

  glm::mat4 GetViewMatrix() {
    return ViewMat;
  }; //GetViewMatrix

private:
  glm::mat4 ViewMat;
  glm::mat4 ProjMat;

  float FOV;
  float NearClip;
  float FarClip;
  UINT FOVUnit;
}; //Camera

struct Motion {
  INT8 featureType;
  INT8 input;
  INT8 output;
  INT8 angleUnit;
  bool disabled;
  float time;
  float accel;
  float speed;
  float gravity;
  const char* name;
  const char* featureName;

  glm::vec3 outputLocation;
  glm::vec3 inputLocation;

  void CalculateArch(glm::vec3* currentPos) {
    
    float tSquaredHalf = -(time * time) * 0.5;
    float xRoot = (-2 / accel * 2);
    currentPos->x += accel * tSquaredHalf + time * (outputLocation.x / xRoot);
    currentPos->y += gravity * tSquaredHalf + time * (outputLocation.y / (-2 / gravity * 2));
    currentPos->z += accel * tSquaredHalf + time * (outputLocation.z / xRoot);

    if (!CheckWorldPos(currentPos)) {
      *currentPos = outputLocation;
      SetDisabled(false);
    }; //CheckWorldPos
  }; //CalculateArch
  void CalculateTeleport(glm::vec3* currentPos) {
    *currentPos = outputLocation;
    SetDisabled(false);
  }; //CalculateTeleport
  void CalculateLine(glm::vec3* currentPos) {
    if (outputLocation.x < 0) {
      currentPos->x -= time * (speed + accel);
    }
    else {
      currentPos->x += time * (speed + accel);
    }; //if outputLocation.x < 0
    
    //make this optional
    float accelGrav = 
      accel > gravity ? 
      (accel - gravity) : accel;

    if (outputLocation.y < 0) {
      currentPos->y -= time * (accelGrav);
    }
    else {
      currentPos->y += time * (accelGrav);
    }; //else

    if (outputLocation.z < 0) {
      currentPos->z -= time * (speed + accel);
    }
    else {
      currentPos->z += time * (speed + accel);
    }; //if outputLocation.y < 0
  }; //CalculateLine
  bool CheckWorldPos(glm::vec3* posToCheck) {
    //Negatives
    if (outputLocation.x < 0 && posToCheck->x < outputLocation.x) {
      return false;
    }; //If X < 0
    if (outputLocation.y < 0 && posToCheck->y < outputLocation.y) {
      return false;
    }; //If Y < 0
    if (outputLocation.z < 0 && posToCheck->z < outputLocation.z) {
      return false;
    }; //If Z < 0

    //Positives
    if (outputLocation.x > 0 && posToCheck->x > outputLocation.x) {
      return false;
    }; //If X > 0
    if (outputLocation.y > 0 && posToCheck->y > outputLocation.y) {
      return false;
    }; //If Y > 0
    if (outputLocation.z > 0 && posToCheck->z > outputLocation.z) {
      return false;
    }; //If Z > 0
  }; //CheckWorldPos
  void AddDelta(float timeInput) {
    time += timeInput;
  }; //AddDelta
  void SetDisabled(bool b) {
    disabled = b;
  }; //SetDisabled
  const char* GetName() {
    return name;
  }; //GetName
  INT8 GetOutput() {
    return output;
  }; //GetOutput
  INT8 GetInput() {
    return input;
  }; //GetInput
  INT8 GetFeatureType() {
    return featureType;
  }; //GetOutput
  const char* GetFeatureName() {
    return featureName;
  }; //GetInput
  bool GetDisabled() {
    return disabled;
  }; //GetDisabled
}; //Motion


/* EXTERNAL CLASSES*/
struct PRIVATEPB::Features {
  Features() :
    CameraVector({})
  {};

  //Getters
  std::vector
    <pb::Feature::Camera*>
    GetCameraVector()
  {
    return CameraVector;
  };

  bool GetConfirmed() { return Confirmed; };

  //Setters
  void SetConfirmed(bool b) { Confirmed = b; };

private:
  std::vector<pb::Feature::Camera*> CameraVector;

  bool Confirmed;
}; //FEATURES


/* Interfaces */
struct ControlSet {
  ControlSet() {
  }; //CtrlSet Ctor

  std::unordered_map <
    //ControlType and Pair
    INT8, std::unordered_multimap
    //Pair of InputType and Name of Control
    <INT8, const char*>
  > InputHash;
  std::unordered_map <INT8, Motion*> MotionHash;
}; //ControlSet

struct FeatureSet {
  std::unordered_map
    <const char*, Camera*>
    BuildCameraHash(
      std::vector
      <pb::Feature::Camera*>
      cameraVec,
      pb::Config::Render* rendConf
    ) { //BuildCameraHash
    RendConf = rendConf;
    CameraHash = {};

    for (auto& vecCam : cameraVec) {
      auto hashCam = new Camera(
        vecCam,
        rendConf->GetWindowWidth(),
        rendConf->GetWindowHeight()
      ); //New Camera

      CameraHash.try_emplace(hashCam->GetName(), hashCam);
    }; //for camHash

    return CameraHash;
  }; //GetCameraHash

  std::unordered_map
    <const char*, Camera*>
    CameraHash;

  pb::Config::Render* RendConf;
}; //FeatureSet

struct PbInterface {
  PbInterface(pb::Config::Render* R, FeatureSet* F, ControlSet* C) :
    Controls(C),
    Features(F),
    RenderConf(R)
  {}; //PbInterface

  ControlSet* GetControls() { return Controls; };
  FeatureSet* GetFeatures() { return Features; };
  pb::Config::Render* GetRenderConf() { return RenderConf; };

  std::unordered_map <INT8, std::unordered_multimap<INT8, const char*>>
    GetInputMap() {
    return Controls->InputHash;
  }; //GetInputMap

  Camera* GetFeature(const char* name) {
    return Features->CameraHash.at(name);
  }; //GetCamera

  void IncreaseInput(int inputType, int controlType, const char* name) {
    auto multiHash = Controls->InputHash.find(controlType);
    multiHash->second.emplace(inputType, name);
  }; //IncreaseInput

  void DecreaseInput(int inputType, int controlType, const char* name) {
    auto multiHash = Controls->InputHash.find(controlType)->second;
    auto inputPair = multiHash.equal_range(inputType);
    auto& inputItrStart = inputPair.first;
    auto& inputItrEnd = inputPair.second;
    for (; inputItrStart != inputItrEnd; ++inputItrStart) {
      if (inputItrStart->second == name
        && inputItrStart->first == inputType) {
        multiHash.erase(inputItrStart);
        return;
      }; //if CorrectVar
    }; //for 
  }; //DecreaseInput

  bool CheckCapitalCondition() {
    return false;
  }; //CheckCapitalCondition

private:
  ControlSet* Controls;
  FeatureSet* Features;
  pb::Config::Render* RenderConf;
}; //PbInterface


/*Scene*/
struct Scene {
  std::chrono::steady_clock::time_point delta;
  std::unordered_map<const char*, Camera*> CameraHash;
  std::unordered_map<const char*, Motion*> MotionHashByName;
  std::unordered_multimap<INT8, Motion*> MotionHashByInput;
  std::vector<Motion*> CurrentMotions;
  Camera* currentCamera;

  PbInterface* pb;

  Scene() {};

  //Getters
  float GetDelta() {
    auto cT = std::chrono::high_resolution_clock::now();

    auto d = std::chrono::duration<float,
      std::chrono::seconds::period>(cT -
        delta).count();

    return d;
  }; //GetDelta
  Camera* GetCamera() {
    return currentCamera;
  }; //GetCamera
  
  void ChangeCamera(const char* Name) {
    
  }; //Change Camera

  //Controls Between Frames
  void UpdateObjectPos(Motion* motion, glm::vec3* currentPos) {
    switch (motion->GetOutput()) {
    case ARCHED:
      motion->CalculateArch(currentPos);
      return;

    //case JAGGED:
    //  motion->CalculateJag(vecPos);
    //  return vecPos;

    case TELEPORT:
      motion->CalculateTeleport(currentPos);
      return;

    case LINE:
      motion->CalculateLine(currentPos);
      return;
    } //Switch MotionTypes
  }; //UpdateCamera
  
  bool ConfirmMotionReady(Motion* motion) {
    return true;
  }; //ConfirmMotionReady
  void ActivateKey(bool keyPressed, int key) {
    INT8 i = 0;
    
    switch (key) {
    case GLFW_KEY_APOSTROPHE:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_QUOTATION
        : KEYBOARD_APOSTROPHE;
      break;    
    case GLFW_KEY_COMMA:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_LESS_THAN
        : KEYBOARD_COMMA;
      break;    
    case GLFW_KEY_MINUS:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_UNDERSCORE
        : KEYBOARD_MINUS;
      break;    
    case GLFW_KEY_PERIOD:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_GREATER_THAN
        : KEYBOARD_PERIOD;
      break;    
    case GLFW_KEY_SLASH:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_QUESTION
        : KEYBOARD_SLASH;
      break;    
    case GLFW_KEY_0:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_RIGHT_PARENTHESES
        : KEYBOARD_ZERO;
      break;    
    case GLFW_KEY_1:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_EXCLAMATION
        : KEYBOARD_ONE;
      break;    
    case GLFW_KEY_2:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_AT
        : KEYBOARD_TWO;
      break;    
    case GLFW_KEY_3:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_HASHTAG
        : KEYBOARD_THREE;
      break;    
    case GLFW_KEY_4:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_DOLLAR
        : KEYBOARD_FOUR;
      break;    
    case GLFW_KEY_5:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_PERCENT
        : KEYBOARD_FIVE;
      break;    
    case GLFW_KEY_6:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_CARROT
        : KEYBOARD_SIX;
      break;    
    case GLFW_KEY_7:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_AMPERSAND
        : KEYBOARD_SEVEN;
      break;    
    case GLFW_KEY_8:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_STAR
        : KEYBOARD_EIGHT;
      break;    
    case GLFW_KEY_9:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_LEFT_PARENTHESES
        : KEYBOARD_NINE;
      break;    
    case GLFW_KEY_SEMICOLON:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_COLON
        : KEYBOARD_SEMICOLON;
      break;    
    case GLFW_KEY_EQUAL:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_PLUS
        : KEYBOARD_EQUAL;
      break;    
    case GLFW_KEY_A:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_A_UPPER
        : KEYBOARD_A_LOWER;    
      break;
    case GLFW_KEY_B:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_B_UPPER
        : KEYBOARD_B_LOWER;    
      break;
    case GLFW_KEY_C:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_C_UPPER
        : KEYBOARD_C_LOWER;    
      break;
    case GLFW_KEY_D:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_D_UPPER
        : KEYBOARD_D_LOWER;    
      break;
    case GLFW_KEY_E:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_E_UPPER
        : KEYBOARD_E_LOWER;    
      break;
    case GLFW_KEY_F:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_F_UPPER
        : KEYBOARD_F_LOWER;    
      break;
    case GLFW_KEY_G:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_G_UPPER
        : KEYBOARD_G_LOWER;    
      break;
    case GLFW_KEY_H:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_H_UPPER
        : KEYBOARD_H_LOWER;    
      break;
    case GLFW_KEY_I:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_I_UPPER
        : KEYBOARD_I_LOWER;    
      break;
    case GLFW_KEY_J:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_J_UPPER
        : KEYBOARD_J_LOWER;    
      break;
    case GLFW_KEY_K:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_K_UPPER
        : KEYBOARD_K_LOWER;    
      break;
    case GLFW_KEY_L:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_L_UPPER
        : KEYBOARD_L_LOWER;    
      break;
    case GLFW_KEY_M:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_M_UPPER
        : KEYBOARD_M_LOWER;    
      break;
    case GLFW_KEY_N:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_N_UPPER
        : KEYBOARD_N_LOWER;    
      break;
    case GLFW_KEY_O:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_O_UPPER
        : KEYBOARD_O_LOWER;    
      break;
    case GLFW_KEY_P:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_P_UPPER
        : KEYBOARD_P_LOWER;    
      break;
    case GLFW_KEY_Q:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_Q_UPPER
        : KEYBOARD_Q_LOWER;    
      break;
    case GLFW_KEY_R:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_R_UPPER
        : KEYBOARD_R_LOWER;    
      break;
    case GLFW_KEY_S:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_S_UPPER
        : KEYBOARD_S_LOWER;    
      break;
    case GLFW_KEY_T:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_T_UPPER
        : KEYBOARD_T_LOWER;    
      break;
    case GLFW_KEY_U:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_U_UPPER
        : KEYBOARD_U_LOWER;    
      break;
    case GLFW_KEY_V:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_V_UPPER
        : KEYBOARD_V_LOWER;    
      break;
    case GLFW_KEY_W:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_W_UPPER
        : KEYBOARD_W_LOWER;    
      break;
    case GLFW_KEY_X:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_X_UPPER
        : KEYBOARD_X_LOWER;    
      break;
    case GLFW_KEY_Y:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_Y_UPPER
        : KEYBOARD_Y_LOWER;    
      break;
    case GLFW_KEY_Z:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_Z_UPPER
        : KEYBOARD_Z_LOWER;    
      break;
    case GLFW_KEY_LEFT_BRACKET:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_BRACKET_LEFT_CURLED
        : KEYBOARD_BRACKET_LEFT_SQUARE;    
      break;
    case GLFW_KEY_BACKSLASH:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_PIPE
        : KEYBOARD_BACKSLASH;    
      break;
    case GLFW_KEY_RIGHT_BRACKET:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_BRACKET_RIGHT_CURLED
        : KEYBOARD_BRACKET_RIGHT_SQUARE;    
      break;
    case GLFW_KEY_GRAVE_ACCENT:
      i = (pb->CheckCapitalCondition()) 
        ? KEYBOARD_TILDE
        : KEYBOARD_BACKTICK;    
      break;
    case GLFW_KEY_ENTER:
      i = KEYBOARD_ENTER;
      break;
    case GLFW_KEY_TAB:
      i = KEYBOARD_TAB;
      break;
    case GLFW_KEY_BACKSPACE:
      i = KEYBOARD_BACKSPACE;
      break;
    case GLFW_KEY_INSERT:
      i = KEYBOARD_INSERT;
      break;
    case GLFW_KEY_DELETE:
      i = KEYBOARD_DELETE;
      break;
    case GLFW_KEY_RIGHT:
      i = KEYBOARD_RIGHT;
      break;
    case GLFW_KEY_LEFT:
      i = KEYBOARD_LEFT;
      break;
    case GLFW_KEY_DOWN:
      i = KEYBOARD_DOWN;
      break;
    case GLFW_KEY_UP:
      i = KEYBOARD_UP;
      break;
    case GLFW_KEY_PAGE_DOWN:
      i = KEYBOARD_PAGE_DOWN;
      break;
    case GLFW_KEY_PAGE_UP:
      i = KEYBOARD_PAGE_UP;
      break;
    case GLFW_KEY_END:
      i = KEYBOARD_END;
      break;
    case GLFW_KEY_CAPS_LOCK:
      i = KEYBOARD_CAPSLOCK;
      break;
    case GLFW_KEY_SCROLL_LOCK:
      i = KEYBOARD_SCROLL_LOCK;
      break;
    case GLFW_KEY_NUM_LOCK:
      i = KEYBOARD_NUM_LOCK;
      break;
    case GLFW_KEY_PRINT_SCREEN:
      i = KEYBOARD_PRINT_SCREEN;
      break;
    case GLFW_KEY_PAUSE:
      i = KEYBOARD_PAUSE;
      break;
    case GLFW_KEY_F1:
      i = KEYBOARD_FUNCTION_1;
      break;
    case GLFW_KEY_F2:
      i = KEYBOARD_FUNCTION_2;
      break;
    case GLFW_KEY_F3:
      i = KEYBOARD_FUNCTION_3;
      break;
    case GLFW_KEY_F4:
      i = KEYBOARD_FUNCTION_4;
      break;
    case GLFW_KEY_F5:
      i = KEYBOARD_FUNCTION_5;
      break;
    case GLFW_KEY_F6:
      i = KEYBOARD_FUNCTION_6;
      break;
    case GLFW_KEY_F7:
      i = KEYBOARD_FUNCTION_7;
      break;
    case GLFW_KEY_F8:
      i = KEYBOARD_FUNCTION_8;
      break;
    case GLFW_KEY_F9:
      i = KEYBOARD_FUNCTION_9;
      break;
    case GLFW_KEY_F10:
      i = KEYBOARD_FUNCTION_10;
      break;
    case GLFW_KEY_F11:
      i = KEYBOARD_FUNCTION_11;
      break;
    case GLFW_KEY_F12:
      i = KEYBOARD_FUNCTION_12;
      break;
    case GLFW_KEY_F13:
      i = KEYBOARD_FUNCTION_13;
      break;
    case GLFW_KEY_F14:
      i = KEYBOARD_FUNCTION_14;
      break;
    case GLFW_KEY_F15:
      i = KEYBOARD_FUNCTION_15;
      break;
    case GLFW_KEY_F16:
      i = KEYBOARD_FUNCTION_16;
      break;
    case GLFW_KEY_F17:
      i = KEYBOARD_FUNCTION_17;
      break;
    case GLFW_KEY_F18:
      i = KEYBOARD_FUNCTION_18;
      break;
    case GLFW_KEY_F19:
      i = KEYBOARD_FUNCTION_19;
      break;
    case GLFW_KEY_F20:
      i = KEYBOARD_FUNCTION_20;
      break;
    case GLFW_KEY_F21:
      i = KEYBOARD_FUNCTION_21;
      break;
    case GLFW_KEY_F22:
      i = KEYBOARD_FUNCTION_22;
      break;
    case GLFW_KEY_F23:
      i = KEYBOARD_FUNCTION_23;
      break;
    case GLFW_KEY_F24:
      i = KEYBOARD_FUNCTION_24;
      break;
    case GLFW_KEY_F25:
      i = KEYBOARD_FUNCTION_25;
      break;
    case GLFW_KEY_KP_0:
      i = KEYBOARD_NUMPAD_0;
      break;
    case GLFW_KEY_KP_1:
      i = KEYBOARD_NUMPAD_1;
      break;
    case GLFW_KEY_KP_2:
      i = KEYBOARD_NUMPAD_2;
      break;
    case GLFW_KEY_KP_3:
      i = KEYBOARD_NUMPAD_3;
      break;
    case GLFW_KEY_KP_4:
      i = KEYBOARD_NUMPAD_4;
      break;
    case GLFW_KEY_KP_5:
      i = KEYBOARD_NUMPAD_5;
      break;
    case GLFW_KEY_KP_6:
      i = KEYBOARD_NUMPAD_6;
      break;
    case GLFW_KEY_KP_7:
      i = KEYBOARD_NUMPAD_7;
      break;
    case GLFW_KEY_KP_8:
      i = KEYBOARD_NUMPAD_8;
      break;
    case GLFW_KEY_KP_9:
      i = KEYBOARD_NUMPAD_9;
      break;
    case GLFW_KEY_KP_DECIMAL:
      i = KEYBOARD_NUMPAD_DECIMAL;
      break;
    case GLFW_KEY_KP_DIVIDE:
      i = KEYBOARD_NUMPAD_DIVIDE;
      break;
    case GLFW_KEY_KP_MULTIPLY:
      i = KEYBOARD_NUMPAD_MULTIPLY;
      break;
    case GLFW_KEY_KP_SUBTRACT:
      i = KEYBOARD_NUMPAD_SUBTRACT;
      break;
    case GLFW_KEY_KP_ADD:
      i = KEYBOARD_NUMPAD_ADD;
      break;
    case GLFW_KEY_KP_ENTER:
      i = KEYBOARD_NUMPAD_ENTER;
      break;
    case GLFW_KEY_KP_EQUAL:
      i = KEYBOARD_NUMPAD_EQUAL;
      break;
    case GLFW_KEY_LEFT_SHIFT:
      i = KEYBOARD_LEFT_SHIFT;
      break;
    case GLFW_KEY_LEFT_CONTROL:
      i = KEYBOARD_LEFT_ALT;
      break;
    case GLFW_KEY_LEFT_ALT:
      i = KEYBOARD_LEFT_SUPER;
      break;
    case GLFW_KEY_LEFT_SUPER:
      i = KEYBOARD_LEFT_SUPER;
      break;
    case GLFW_KEY_RIGHT_SHIFT:
      i = KEYBOARD_RIGHT_SHIFT;
      break;
    case GLFW_KEY_RIGHT_CONTROL:
      i = KEYBOARD_RIGHT_CONTROL;
      break;
    case GLFW_KEY_RIGHT_SUPER:
      i = KEYBOARD_RIGHT_SUPER;
      break;
    case GLFW_KEY_MENU:
      i = KEYBOARD_MENU;
      break;
    case GLFW_KEY_UNKNOWN:
      InternalLog(
        "Reading keyboard Input",
        "Returning Key Unknown",
        "No Action Commited"
      ); //InternalLog
      return;
    }; //Key Switch

    auto motionPair = MotionHashByInput.equal_range(i);
    auto& motionItrStart = motionPair.first;
    auto& motionItrEnd = motionPair.second;
    for (; motionItrStart != motionItrEnd; ++motionItrStart) {
      auto motion = motionItrStart->second;

      if ((motion->GetInput() == i)
        && (!motion->GetDisabled())
        && ConfirmMotionReady(motion)) {
        if (keyPressed) {
          pb->IncreaseInput(i, MOTION, motion->GetName());
        } //if keyPressed
        else {
          pb->DecreaseInput(i, MOTION, motion->GetName());
        }; //else keyPressed
      }// if (motionData = GetInput)        
    }; //For motionItrs
  }; //ActivateKey
  void ActivateMotion(Motion* motion) {
    motion->SetDisabled(true);
    motion->AddDelta(GetDelta());

    switch (motion->GetFeatureType()) {
    case CAMERA:
      auto cam = CameraHash.find(motion->GetFeatureName())->second;
      auto vecPos = &cam->GetWorldPosVec();
      UpdateObjectPos(motion, vecPos);
      cam->UpWorldPos(vecPos);
      return;
    }; //TYPE SWITCH
  }; //ActivateMotion
  
  void UpdateFrameVariables() {
    //Trigger Delta
    delta = std::chrono
      ::high_resolution_clock::now();


    //Activate Controls
    for (auto& ctrlType : pb->GetInputMap()) {
      for (auto& motion : ctrlType.second) {
        CurrentMotions.emplace_back(
          MotionHashByName.find(motion.second)
        ); //CurrentMotions.emplace_back
      }; // for motion in map
    }; //GetInputMap
    

    //ActivateMotions
    int control = MOTION;
    auto motionItr = CurrentMotions.begin();
    for (auto i = CurrentMotions.size(); i > 0; --i) {
      switch (control) {
      case MOTION: {
        if (!CurrentMotions[i]->GetDisabled()) {
          CurrentMotions.erase(motionItr);
        }; //if Enabled
        ActivateMotion(CurrentMotions[i]);
        if (i == 1) {
          ActivateMotion(CurrentMotions[0]);
          //i = ControlVector.size();
        }; // if I == 1
        ++motionItr;
      } //case Motion
      }; //Control
    }; //for itr.size


  }; //UpdateFrameVariables
}; //Scene


/* Variables for General Logic */
Scene* currentScene;


/* CALLBACKS */
void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    return currentScene->ActivateKey(true, key);
  } //GLFW_PRESS
  else if (key == GLFW_RELEASE) {
    return currentScene->ActivateKey(false, key);
  }; //GLFWRELEASE
} //KeyboardCallback

inline static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
  InternalLog("Internal Debugger", "Validation Layer:", pCallbackData->pMessage);

  return VK_FALSE;
} //CallBack


struct GLFWInterface {
  pb::Config::Render* RendConf;
  GLFWmonitor* mon;
  GLFWwindow* win;
  const GLFWvidmode* vidMode;
  INT winXPos;
  INT winYPos;
  INT monWidth;
  INT monHeight;
  INT fbHeight;
  INT fbWidth;
  UINT32 extensionCount;
  const char** extensions;

  GLFWInterface(pb::Config::Render* rendConf) {
    InternalLog("Creating Window", "Initializing GLFW", "Initializing");

    RendConf = rendConf;

    if (!glfwInit()) {
      InternalReport("Creating Window", "Initializing GLFW", "Initialization Failed");
    } //If Init

    InternalLog("Creating Window", "Creating Monitor", "Collecting Stats");

    mon = glfwGetPrimaryMonitor();
    vidMode = glfwGetVideoMode(mon);

    glfwGetMonitorWorkarea(
      mon, &winXPos, &winYPos,
      &monWidth, &monHeight);

    InternalLog(
      "Creating Window ", 
      " Selecting Window ", 
      " Collecting Configurations");

    FINT16 winHeight = 
      rendConf->GetWindowHeight();
    FINT16 winWidth = 
      rendConf->GetWindowWidth();
    const char* winName = 
      rendConf->GetWindowName();

    if (winHeight && winWidth) {
      CreateWin(winHeight, winWidth, winName);
    }
    else {
      CreateWin(winName);
    } //Else

    extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    glfwGetFramebufferSize(win, &fbWidth, &fbHeight);
    glfwSetKeyCallback(win, KeyboardCallback);
  }; //GLFW

  inline void CreateWin(FINT16 winWidth, FINT16 winHeight, const char* winName) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    InternalLog("Creating Window", "Creating Window", "Creating Window");

    if (RendConf->GetFullscreenBool()) {
      win = glfwCreateWindow(winWidth, winHeight, winName, mon, nullptr);
    }
    else {
      win = glfwCreateWindow(winWidth, winHeight, winName, nullptr, nullptr);
    } //ifFullscreen

    if (!win) {
      InternalReport("Creating Window", "Creating Window", "Window Creation Failed");
    }
  }; //createWindow

  inline void CreateWin(const char* winName) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    InternalLog("Creating Window ", " Creating Window ", " Creating Window");

    if (RendConf->GetFullscreenBool()) {
      win = glfwCreateWindow(monWidth, monHeight, winName, mon, nullptr);
    }
    else {
      win = glfwCreateWindow(monWidth, monHeight, winName, nullptr, nullptr);
    } //ifFullscreen

    if (!win) {
      InternalReport("Creating Window ", " Creating Window ", " Window Creation Failed");
    }

    RendConf->SetWindowHeight(monHeight);
    RendConf->SetWindowWidth(monWidth);
  }; //createWindow

  inline void CreateInterface(VkInstance vkInterface, VkSurfaceKHR* vkSurface) {
    InternalLog(
      "Initializing Vulkan ", 
      " Creating Window Surface ", 
      " Creating and Confirming");

    if (glfwCreateWindowSurface(vkInterface, win, nullptr, vkSurface) != VK_SUCCESS) {
      InternalReport("Initializing Vulkan ", " Creating Window Surface ", " Creation Failed");
    } //If Surface
  }; //CreateInterface

  inline void RefreshStats() {
    mon = glfwGetPrimaryMonitor();
    vidMode = glfwGetVideoMode(mon);

    glfwGetMonitorWorkarea(
      mon, &winXPos, &winYPos,
      &monWidth, &monHeight);

    glfwGetRequiredInstanceExtensions(&extensionCount);
    glfwGetFramebufferSize(win, &fbWidth, &fbHeight);
  };

  ~GLFWInterface() {};

}; //GLFW

struct Vertex {
  glm::vec4 color;
  glm::vec3 pos;
  glm::vec2 texCoord;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
  }
};

struct Block :
  public Object {

  std::vector<Vertex> vertices;
  std::vector<UINT16> indices;
  glm::vec3 blockColor;
  UINT polyhedra;

  Block(unsigned int type) {
    polyhedra = type;
  }; //Block

  void SetBlockColor(glm::vec3 color) {
    blockColor = color;
  }; //SetBlockColor

  void SetBlockRed(float red) {
    if (red > 1.0f) { InternalReport("Assigning Blocks", "Setting Red Color", "Assigned Float too High"); };

    blockColor[0] = red;
  }; //SetBlockRed

  void SetBlockGreen(float green) {
    if (green > 1.0f) { InternalReport("Assigning Blocks", "Setting Green Color", "Assigned Float too High"); };

    blockColor[1] = green;
  }; //SetBlockGreen 

  void SetBlockBlue(float blue) {
    if (blue > 1.0f) { InternalReport("Assigning Blocks", "Setting Blue Color", "Assigned Float too High"); };

    blockColor[2] = blue;
  }; //SetBlockRed

  void SetBlockAlpha(float alpha) {
    if (alpha > 1.0f) { InternalReport("Assigning Blocks", "Setting Red Color", "Assigned Float too High"); };

    blockColor[3] = alpha;
  }; //SetBlockRed

  void Activate() {
    auto texVec = getBlockTextures(polyhedra);
    auto vertVec = getBlockVertices(polyhedra);
    auto colorVec = getBlockColors(polyhedra);
    indices = getBlockIndices(polyhedra);

    if ((colorVec.size() % 4) != 0) {
      InternalReport(
        "Loading Blocks",
        "Loading Vertices",
        "Vertex Data Failed: Colors");
    }; //If ColorVec

    if ((vertVec.size() % 3) != 0) {
      InternalReport(
        "Loading Blocks",
        "Loading Vertices",
        "Vertex Data Failed: Position");
    }; //If vertVec

    if ((indices.size() % 3) != 0) {
      InternalReport(
        "Loading Blocks",
        "Loading Vertices",
        "Vertex Data Failed: Indices");
    }; //vertex

    if ((texVec.size() % 2) != 0) {
      InternalReport(
        "Loading Blocks",
        "Loading Vertices",
        "Vertex Data Failed: Textures");
    }; //texVec

    vertices = {};

    size_t i = 0;
    for (; i < colorVec.size() / 4; ++i) {
      Vertex v = {
        { 
          colorVec[4 * i], 
          colorVec[4 * i + 1], 
          colorVec[4 * i + 2], 
          colorVec[4 * i + 3] 
        },
        { 
          vertVec[3 * i], 
          vertVec[3 * i + 1], 
          vertVec[3 * i + 2] 
        },
        { 
          texVec[2 * i], 
          texVec[2 * i + 1] 
        }
      }; //vertices

      vertices.emplace_back(v);
    }; //for
  }; //Activate
}; //Block

Block* block;

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
}; //SwapChainSupportDetails

inline SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
} //SwapChainSupportDetails

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  } //isComplete
}; //QueueFamilyIndices

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR windowSurface) {
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, windowSurface, &presentSupport);

    if (presentSupport) {
      indices.presentFamily = i;
    }

    if (indices.isComplete()) {
      break;
    }

    i++;
  }

  return indices;
} //QueueFamilyIndices

const std::vector<const char*> validationLayers = {
"VK_LAYER_KHRONOS_validation"
}; //ValidationLayers

const std::vector<const char*> deviceExtensions = {
VK_KHR_SWAPCHAIN_EXTENSION_NAME
}; //deviceExtensions

struct GPUInterface {
  struct SoftInterface {
    VkDevice softInterface;
    VkQueue cmndQueue;
    VkQueue imageQueue;

    SoftInterface(VkInstance vkInterface, VkPhysicalDevice hardInterface, VkSurfaceKHR* surface) {
      InternalLog("Initializing Vulkan ", " Creating Device Interface ", " Queueing Devices");

      QueueFamilyIndices indices = FindQueueFamilies(hardInterface, *surface);

      std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
      std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

      InternalLog("Initializing Vulkan ", " Creating Device Interface ", " Staging Info");

      float queuePriority = 1.0f;
      for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
      }

      VkPhysicalDeviceFeatures deviceFeatures{};
      deviceFeatures.samplerAnisotropy = VK_TRUE;

      VkDeviceCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

      createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
      createInfo.pQueueCreateInfos = queueCreateInfos.data();

      createInfo.pEnabledFeatures = &deviceFeatures;

      createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
      createInfo.ppEnabledExtensionNames = deviceExtensions.data();

      if (DEBUG) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
      }
      else {
        createInfo.enabledLayerCount = 0;
      }

      InternalLog("Initializing Vulkan ", " Creating Device Interface ", " Confirming Virtual Interface");

      if (vkCreateDevice(hardInterface, &createInfo, nullptr, &softInterface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
      }

      InternalLog("Initializing Vulkan ", " Creating Device Interface ", " Applying Interface to Device");

      vkGetDeviceQueue(softInterface, indices.graphicsFamily.value(), 0, &cmndQueue);
      vkGetDeviceQueue(softInterface, indices.presentFamily.value(), 0, &imageQueue);
    };
  };

  struct HardInterface {
    VkPhysicalDevice hardInterface;
    VkPhysicalDeviceProperties hardInterfaceProperties;

    HardInterface(VkInstance vkInterface, VkSurfaceKHR* windowSurface) {
      InternalLog("Initializing Vulkan ", " Picking Physical Device ", " Enumerating GPUs");

      UINT32 deviceCount = 0;
      vkEnumeratePhysicalDevices(vkInterface, &deviceCount, nullptr);

      if (deviceCount == 0) {
        InternalReport("Initializing Vulkan ", " Picking Physical Device ", " No GPU Support Found");
      }

      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(vkInterface, &deviceCount, devices.data());

      InternalLog("Initializing Vulkan ", " Picking Physical Device ", " Testing Pipelines");

      for (const auto& device : devices) {
        QueueFamilyIndices indices = FindQueueFamilies(device, *windowSurface);

        UINT32 extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
          requiredExtensions.erase(extension.extensionName);
        }

        bool swapChainAdequate = false;
        if (requiredExtensions.empty()) {
          SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, *windowSurface);
          swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }


        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        if (indices.isComplete() && requiredExtensions.empty() && swapChainAdequate && supportedFeatures.samplerAnisotropy) {
          hardInterface = device;
          break;
        }
      }

      InternalLog("Initializing Vulkan ", " Picking Physical Device ", " Confirming Pipeline Support");

      if (hardInterface == VK_NULL_HANDLE) {
        InternalReport("Initializing Vulkan ", " Picking Physical Device ", " No Pipeline Support Found");
      }

      vkGetPhysicalDeviceProperties(hardInterface, &hardInterfaceProperties);
    };
  };

  SoftInterface* softInterface;
  HardInterface* hardInterface;

  GPUInterface(VkInstance vkInterface, VkSurfaceKHR* surface) {
    hardInterface = new HardInterface(vkInterface, surface);
    softInterface = new SoftInterface(vkInterface, hardInterface->hardInterface, surface);
  }; //GPUInterface

  ~GPUInterface() {
    delete hardInterface;
    delete softInterface;
  }; //GPUInterface

}; //GpuInterface

struct DebugInterface {


  DebugInterface() {
    
  }; //DebugInterface
}; //DebugInterface

struct VkInterface {
  VkInstance vulkan;
  GPUInterface* gpu;
  GLFWInterface* glfw;
  DebugInterface* debug;
  VkSurfaceKHR windowSurface;
  VkDebugUtilsMessengerEXT debugMessenger; //Move to DebugInterface

  VkInterface(pb::Config::Render* rendConf) {
    glfw = new GLFWInterface(rendConf);
    VulkanInterface(rendConf);
    glfw->CreateInterface(vulkan, &windowSurface);
    gpu = new GPUInterface(vulkan, &windowSurface);
    debug = new DebugInterface();
    if (DEBUG) { ActivateDebug(); }; //Move to DebugInterface
  }; //VkInterface

  void VulkanInterface(pb::Config::Render* rendConf) {
    InternalLog("Initializing Vulkan ", " Creating Program Instance ", " Writing Struct");

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = rendConf->GetWindowName();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "PolyBlock Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;


    InternalLog(
      "Initializing Vulkan ", 
      " Creating Program Instance ", 
      " Collecting Extensions");

    std::vector<const char*> extensions(glfw->extensions, glfw->extensions + glfw->extensionCount);

    if (DEBUG) {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    } //If DEBUG

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();


    InternalLog("Initializing Vulkan ", " Creating Program Instance ", " Collecting Vulkan Debug Data");

    if (DEBUG) {
      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();

      VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
      debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      debugInfo.pfnUserCallback = DebugCallback;

      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugInfo;
    }
    else {
      createInfo.enabledLayerCount = 0;

      createInfo.pNext = nullptr;
    } //ElseIf


    InternalLog("Creating Program Instance ", " Creating Virtual Instance ", " Appoving Instance");

    if (vkCreateInstance(&createInfo, nullptr, &vulkan) != VK_SUCCESS) {
      InternalReport("Creating Program Instance ", " Creating Virtual Instance ", " Instance Creation Failed");
    } //If CreateInstance
  }; //VulkanInterface

  void ActivateDebug() {
    InternalLog(
      "Initializing Vulkan", 
      "Confirming Validation Layers", 
      "Enumerating and Confirming Layers"
    ); //InternalLog

    UINT32 layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
      bool layerFound = false;

      for (const auto& layerProperties : availableLayers) {
        if (strcmp(layerName, layerProperties.layerName) == 0) {
          layerFound = true;
          break;
        } // If LayerName Matches
      } //For LayerProperties

      if (!layerFound) {
        InternalReport("Initializing Vulkan ", " Confirming Validation Layers ", " Failure");
      } //If LayerFound
    } // For LayerNames

    VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
    debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugInfo.pfnUserCallback = DebugCallback;

    InternalLog(
      "Initializing Vulkan ", 
      " Creating Debug Messenger ", 
      " Creating Debugger"
    ); //InternalLog

    auto b = VK_ERROR_EXTENSION_NOT_PRESENT;

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vulkan, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
      b = func(vulkan, &debugInfo, nullptr, &debugMessenger);
    } //If Nullptr

    InternalLog(
      "Initializing Vulkan ", 
      " Confirming Debugger ", 
      " Confirming Debugger"
    ); //InternalLog

    if (b != VK_SUCCESS) {
      InternalReport(
        "Initializing Vulkan ", 
        " Confirming Debugger ", 
        " Confirmation Failed"
      ); //InternalReport
    } // If CeateMessenger
  }; //ActivateDebug

  VkPhysicalDevice GetGPUHardInterface() {
    return gpu->hardInterface->hardInterface;
  }; //GetGPUHardInterface

  VkDevice GetGPUSoftInterface() {
    return gpu->softInterface->softInterface;
  }; //GetGPUSoftInterface

  VkQueue GetCmndQueue() {
    return gpu->softInterface->cmndQueue;
  }; //GetCmdQueue

  VkQueue GetImageQueue() {
    return gpu->softInterface->imageQueue;
  }; //GetImageQueue

  ~VkInterface() {
    delete gpu;
    delete glfw;
  }; //~VkInterface
}; //VkInterface

struct GFXPipeline {
  PbInterface* pbInterface;
  VkInterface* vkInterface;


}; //GFXPipeline

VkInterface* vkInterface;

  VkCommandPool commandPool;

  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(vkInterface->GetGPUHardInterface(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
      if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
        return i;
      }
    }

    throw std::runtime_error("failed to find suitable memory type!");
  } //FindMemoryType

  struct BufferInput {
    BufferInput(VkDeviceSize bufferSize, INT8 bufferType, const void* dataInput) {
        buffSize = bufferSize;
        buffType = bufferType;

        switch (buffType) {
          case VERTEX_BUFF:
            InitStageBuff(STAGE_BUFF_BITS);
            InitDestBuff(VERTEX_BUFF_BITS);

            InjectData(dataInput);
            SendCmdBuffer();
            break;

          case INDICE_BUFF:
            InitStageBuff(STAGE_BUFF_BITS);
            InitDestBuff(INDEX_BUFF_BITS);

            InjectData(dataInput);
            SendCmdBuffer();
            break;

          case UNIFORM_BUFF:
            InitStageBuff(STAGE_BUFF_BITS);
            InitDestBuff(UNIFORM_BUFF_BITS);
            SendUniformBuffer();
            UpdateBuffer(dataInput);
            break;
        };

        DeallocStageBuffer();
    }; //Buffer

    VkBuffer GetBuffer() { return destBuff; };
    
    void UpdateBuffer(const void* dataInput) {
      switch (buffType) {
      case VERTEX_BUFF:
        break;

      case INDICE_BUFF:
        break;

      case UNIFORM_BUFF:
        memcpy(uniPntr, dataInput, buffSize);
        break;
      };
    };

    ~BufferInput() {
      vkDestroyBuffer(vkInterface->GetGPUSoftInterface(), destBuff, nullptr);
      vkFreeMemory(vkInterface->GetGPUSoftInterface(), destData, nullptr);
    };

  private:
    void* uniPntr;
    VkBuffer stageBuff;
    VkBuffer destBuff;
    VkDeviceSize buffSize;
    VkDeviceMemory stageData;
    VkDeviceMemory destData;
    INT8 buffType;

    void InitStageBuff(VkBufferUsageFlags use, VkMemoryPropertyFlags memProp) {
      VkBufferCreateInfo buffInfo{};
      buffInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      buffInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      buffInfo.size = buffSize;
      buffInfo.usage = use;

      if (vkCreateBuffer(vkInterface->GetGPUSoftInterface(), &buffInfo, nullptr, &stageBuff) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
      }

      VkMemoryRequirements memReq; //Memory Requirements
      vkGetBufferMemoryRequirements(vkInterface->GetGPUSoftInterface(), stageBuff, &memReq);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memReq.size;
      allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, memProp);

      if (vkAllocateMemory(vkInterface->GetGPUSoftInterface(), &allocInfo, nullptr, &stageData) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
      }

      vkBindBufferMemory(vkInterface->GetGPUSoftInterface(), stageBuff, stageData, 0);
    }

    void InitDestBuff(VkBufferUsageFlags use, VkMemoryPropertyFlags memProp) {
      VkBufferCreateInfo buffInfo{};
      buffInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      buffInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      buffInfo.size = buffSize;
      buffInfo.usage = use;

      if (vkCreateBuffer(vkInterface->GetGPUSoftInterface(), &buffInfo, nullptr, &destBuff) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
      }

      VkMemoryRequirements memReq; //Memory Requirements
      vkGetBufferMemoryRequirements(vkInterface->GetGPUSoftInterface(), destBuff, &memReq);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memReq.size;
      allocInfo.memoryTypeIndex = findMemoryType(memReq.memoryTypeBits, memProp);

      if (vkAllocateMemory(vkInterface->GetGPUSoftInterface(), &allocInfo, nullptr, &destData) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
      }

      vkBindBufferMemory(vkInterface->GetGPUSoftInterface(), destBuff, destData, 0);
    }

    //Templates are for losers
    void InjectData(const void* input) {
      void* data;
      vkMapMemory(vkInterface->GetGPUSoftInterface(), stageData, 0, buffSize, 0, &data);
      memcpy(data, input, (size_t)buffSize);
      vkUnmapMemory(vkInterface->GetGPUSoftInterface(), stageData);
    }; //InjectData


    void SendCmdBuffer() {
      //Build Buffer
      VkCommandBuffer cmndBuff;

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = commandPool;
      allocInfo.commandBufferCount = 1;

      vkAllocateCommandBuffers(vkInterface->GetGPUSoftInterface(), &allocInfo, &cmndBuff);

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      
      //Record
      vkBeginCommandBuffer(cmndBuff, &beginInfo);

      VkBufferCopy copyRegion{};
      copyRegion.size = buffSize;
      vkCmdCopyBuffer(cmndBuff, stageBuff, destBuff, 1, &copyRegion);

      
      //Recorded
      vkEndCommandBuffer(cmndBuff);

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &cmndBuff;

      vkQueueSubmit(vkInterface->GetCmndQueue(), 1, &submitInfo, VK_NULL_HANDLE);
      vkQueueWaitIdle(vkInterface->GetCmndQueue());

      vkFreeCommandBuffers(vkInterface->GetGPUSoftInterface(), commandPool, 1, &cmndBuff);
    }; //CopyBuffers


    void SendUniformBuffer() {
      vkMapMemory(vkInterface->GetGPUSoftInterface(), destData, 0, buffSize, 0, &uniPntr);


    }; //SendUniformBuffer

    void DeallocStageBuffer() {
      vkDestroyBuffer(vkInterface->GetGPUSoftInterface(), stageBuff, nullptr);
      vkFreeMemory(vkInterface->GetGPUSoftInterface(), stageData, nullptr);
    }; //DeallocStage
  }; //BufferInput

  //Sec. Init


  //Sec. Pipe
  VkImage textureImage;
  VkDeviceMemory textureImageMemory;
  VkImageView textureImageView;
  VkSampler textureSampler;
  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;
  std::vector<VkImageView> swapChainImageViews;
  VkDescriptorSetLayout descriptorSetLayout;
  VkRenderPass renderPass;
  VkExtent2D swapChainExtent;


  //Sec. Cmnd
  BufferInput* vertBuff;
  BufferInput* indiceBuff;
  std::vector<VkDescriptorSet> descriptorSets;
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<BufferInput*> uniformBuffers;
  VkDescriptorPool descriptorPool;
  UINT32 currentFrame = 0;
  const int MAX_FRAMES_IN_FLIGHT = 2;
  char mesoLog[64];
  const char* macroLog = "Integrating Commands";
  VkImage depthImage;
  VkDeviceMemory depthImageMemory;
  VkImageView depthImageView;

  //Sec. Loop
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  bool framebufferResized = false;


  //Sec. Pipe
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
      }
    }

    return availableFormats[0];
  }

  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        return availablePresentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
      return capabilities.currentExtent;
    }
    else {
      int width, height;
      glfwGetFramebufferSize(vkInterface->glfw->win, &width, &height);

      VkExtent2D actualExtent = {
          static_cast<uint32_t>(width),
          static_cast<uint32_t>(height)
      };

      actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
      actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

      return actualExtent;
    }
  }

  VkShaderModule createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(vkInterface->GetGPUSoftInterface(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
      throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
  }

  static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
      throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
  }

  //Sec. Cmnd

  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(vkInterface->GetGPUSoftInterface(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkInterface->GetGPUSoftInterface(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(vkInterface->GetGPUSoftInterface(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(vkInterface->GetGPUSoftInterface(), buffer, bufferMemory, 0);
  } //createBuffer

  void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(vkInterface->GetGPUSoftInterface(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
      throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vkInterface->GetGPUSoftInterface(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(vkInterface->GetGPUSoftInterface(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(vkInterface->GetGPUSoftInterface(), image, imageMemory, 0);
  } //CreateImage

  void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(vkInterface->GetCmndQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(vkInterface->GetCmndQueue());

    vkFreeCommandBuffers(vkInterface->GetGPUSoftInterface(), commandPool, 1, &commandBuffer);
  }//endSingleTimeCommands

  VkCommandBuffer beginSingleTimeCommands() {
    //Copy COmmand Buffer Info
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(vkInterface->GetGPUSoftInterface(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
  } //beginSingleTimeCommands

  void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else {
      throw std::invalid_argument("unsupported layout transition!");
    }

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;  
        if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT) {
        barrier.subresourceRange.aspectMask |=
          VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    vkCmdPipelineBarrier(
      commandBuffer,
      sourceStage, destinationStage,
      0,
      0, nullptr,
      0, nullptr,
      1, &barrier
    ); //vkCmdPipelineBarrier

    endSingleTimeCommands(commandBuffer);
  } //TransitionImageLayout

  void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    }; //VkBufferImageCopy

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(commandBuffer);
  } //CopyBufferToImage

  void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    
    
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    //Generic Copy
    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    //Flush Equivalent
    endSingleTimeCommands(commandBuffer);
  } //CopyBuffer

  struct pushConst {
    alignas(16) glm::vec3 model;
  }; //UniformBufferObjec

  struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
  }; //pushConst

  //Sec. Loop
  void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(instance, debugMessenger, pAllocator);
    }
  } // DestroDebugMessengerEXT

  //Sec. Pipe
  VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
      VkFormatProperties props;
      vkGetPhysicalDeviceFormatProperties(vkInterface->GetGPUHardInterface(), format, &props);

      if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
        return format;
      }
      else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
        return format;
      }
    }

    throw std::runtime_error("failed to find supported format!");
  }

  VkFormat FindDepthFormat() {
    return FindSupportedFormat(
      { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
      VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
  } //VkFormat

  VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(vkInterface->GetGPUSoftInterface(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
      throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
  } //CreateImageView

  inline void CreateSwapChain() {
    InternalLog(
      "Creating Vulkan Pipeline ", 
      " Creating Image Chain ", 
      " Checking for GPU Support"
    ); //InternalLog

    SwapChainSupportDetails swapChainSupport =
      querySwapChainSupport(vkInterface->GetGPUHardInterface(), vkInterface->windowSurface);

    InternalLog(
      "Creating Vulkan Pipeline ", 
      " Creating Image Chain ", 
      " Setting GPU Options"
    ); //InternalLog

    VkSurfaceFormatKHR surfaceFormat =
      chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode =
      chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent =
      chooseSwapExtent(swapChainSupport.capabilities);

    InternalLog(
      "Creating Vulkan Pipeline", 
      "Creating Image Chain", 
      " Setting Chain Length"
    ); //InternalLog

    UINT32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
      imageCount = swapChainSupport.capabilities.maxImageCount;
    } //imageCount > swapChain

    InternalLog(
      "Creating Vulkan Pipeline", 
      "Creating Image Chain", 
      "Informing Chain"
    ); //InternalLog

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vkInterface->windowSurface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = FindQueueFamilies(vkInterface->GetGPUHardInterface(), vkInterface->windowSurface);
    UINT32 queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(vkInterface->GetGPUSoftInterface(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
      InternalReport("Creating Vulkan Pipeline ", " Creating Image Chain ", " Chain Creation Failed");
    } //if vkCreateSwapChain

    InternalLog(
      "Creating Vulkan Pipeline ", 
      "Creating Image Chain ", 
      "Preparing Final Chain"
    ); //InternalLog

    vkGetSwapchainImagesKHR(vkInterface->GetGPUSoftInterface(), swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(vkInterface->GetGPUSoftInterface(), swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
  }; //CreateSwapChain

  void CreateImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
      swapChainImageViews[i] = CreateImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
  } //CreateImageView

  inline void CreateRenderPass() {
    InternalLog("Creating Vulkan Pipeline ", " Creating Image Processors ", " Assigning Image Processors");

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    InternalLog("Creating Vulkan Pipeline ", " Creating Image Processors ", " Confirming Image Processors");

    if (vkCreateRenderPass(vkInterface->GetGPUSoftInterface(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
      InternalReport("Creating Vulkan Pipeline ", " Creating Image Processors ", " Image Processor Creation Failed");
    }
  }; //CreateRenderPass

  void CreateDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(vkInterface->GetGPUSoftInterface(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor set layout!");
    }
  }

  inline void CreateGraphicsPipeline() {
    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Reading SPIR-V");

    auto vertShaderCode = readFile("shaders/shadBin/vert.spv");
    auto fragShaderCode = readFile("shaders/shadBin/frag.spv");

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Creating Shaders Interface");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Setting Shader Uses");

    VkPushConstantRange pushConstRange{};
    pushConstRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstRange.offset = 0;
    pushConstRange.size = sizeof(pushConst);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Creating Shader Stages");

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Creating Shader Interfaces");

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstRange;

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Confirming Interfaces");

    if (vkCreatePipelineLayout(vkInterface->GetGPUSoftInterface(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
      InternalReport("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Interface Confirmation Failed");
    }

    InternalLog(
      "Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Assigning Shader Stages");

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Confirming Graphic Pipeline");

    if (vkCreateGraphicsPipelines(vkInterface->GetGPUSoftInterface(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
      InternalReport("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Graphics Pipeline Confirmation Failed");
    } //If

    InternalLog("Creating Vulkan Pipeline ", " Creating Graphics Pipeline ", " Clearing Unecessary Assets");

    vkDestroyShaderModule(
      vkInterface->GetGPUSoftInterface(), fragShaderModule, nullptr);
    vkDestroyShaderModule(
      vkInterface->GetGPUSoftInterface(), vertShaderModule, nullptr);
  }; //CreateGraphicsPipeline

  void CreateFramebuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
      std::array<VkImageView, 2> attachments = {
          swapChainImageViews[i],
          depthImageView
      };

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = renderPass;
      framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
      framebufferInfo.pAttachments = attachments.data();
      framebufferInfo.width = swapChainExtent.width;
      framebufferInfo.height = swapChainExtent.height;
      framebufferInfo.layers = 1;

      if (vkCreateFramebuffer(vkInterface->GetGPUSoftInterface(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
      }
    }
  } //CreateFrameBuffers
    
  //Sec. Cmnd
  inline void CreateCommandPool() {
    memset(mesoLog, 0, sizeof(mesoLog));
    strcpy(mesoLog, "Creating Threads");

    InternalLog(macroLog, mesoLog, " Creating Threadpool");

    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(vkInterface->GetGPUHardInterface(), vkInterface->windowSurface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    InternalLog(macroLog, mesoLog, " Checking Theadpool");

    if (vkCreateCommandPool(vkInterface->GetGPUSoftInterface(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
      InternalReport(macroLog, mesoLog, "Threapool Creation Failed");
    } //If VkCommandPool = VK_SUCCESS
  }; //CreateCommandPool

  void CreateDepthResources() {
    VkFormat depthFormat = FindDepthFormat();

    createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    depthImageView = CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    TransitionImageLayout(depthImage, depthFormat,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


  } //CreateDepthResources

  inline void CreateTextureImage() {
    memset(mesoLog, 0, sizeof(mesoLog));
    strcpy(mesoLog, "Creating Texture Interface");

    InternalLog(macroLog, mesoLog, "Creating Texture");

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load("textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    int pixCnt = texWidth * texHeight;
    VkDeviceSize imageSize =  pixCnt * 4;

    if (!pixels) {
      throw std::runtime_error("failed to load texture image!");
    }

    //int i = 0;
    //for (; i < pixCnt; ++i) {
    //  std::cout << (int)pixels[i] << "\n";
    //};

    InternalLog(macroLog, mesoLog, "Building Interface");

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(vkInterface->GetGPUSoftInterface(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, imageSize);
    vkUnmapMemory(vkInterface->GetGPUSoftInterface(), stagingBufferMemory);

    createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

    InternalLog(macroLog, mesoLog, "Assigning Interface Constants");

    TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    InternalLog(macroLog, mesoLog, "Assigning Interface to Hardware");

    vkDestroyBuffer(vkInterface->GetGPUSoftInterface(), stagingBuffer, nullptr);
    vkFreeMemory(vkInterface->GetGPUSoftInterface(), stagingBufferMemory, nullptr);
  }; //CreateTextureImage

  void CreateTextureImageView() {
    textureImageView = CreateImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
  } //CreateTextureImageView

  void CreateTextureSampler() {
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(vkInterface->GetGPUHardInterface(), &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if (vkCreateSampler(vkInterface->GetGPUSoftInterface(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
      throw std::runtime_error("failed to create texture sampler!");
    }
  } //CreateTextureSampler

  inline void CreateVertexBuffer() {
    //Record Vertice Buffer
    VkDeviceSize bufferSize =
      sizeof(block->vertices[0])
      * block->vertices.size();

    vertBuff = new BufferInput(bufferSize, VERTEX_BUFF, block->vertices.data());
  }; //CreateVertexBuffer

  inline void CreateIndexBuffer() {
    VkDeviceSize bufferSize =
      sizeof(block->indices[0])
      * block->indices.size();

    indiceBuff = new BufferInput(bufferSize, INDICE_BUFF, block->indices.data());
  }; //CreateIndexBuffer

  inline void CreateUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    UniformBufferObject ubo{};

    auto cam = currentScene->GetCamera();
    ubo.model = cam->GetWorldPosMat();
    ubo.proj = cam->GetPerspectiveProj();
    ubo.view = cam->GetViewMatrix();

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      uniformBuffers[i] = new BufferInput(bufferSize, UNIFORM_BUFF, &ubo);
    }
  }; //CreateUniformBuffer

  inline void CreateDescriptorPool() {
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(vkInterface->GetGPUSoftInterface(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor pool!");
    } //ICreateDescriptorPool = VK_SUCCESS
  } //CreateDescriptorPool


  void CreateDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(vkInterface->GetGPUSoftInterface(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      VkDescriptorBufferInfo bufferInfo{};
      bufferInfo.buffer = uniformBuffers[i]->GetBuffer();
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UniformBufferObject);

      VkDescriptorImageInfo imageInfo{};
      imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageInfo.imageView = textureImageView;
      imageInfo.sampler = textureSampler;

      std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

      descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[0].dstSet = descriptorSets[i];
      descriptorWrites[0].dstBinding = 0;
      descriptorWrites[0].dstArrayElement = 0;
      descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrites[0].descriptorCount = 1;
      descriptorWrites[0].pBufferInfo = &bufferInfo;

      descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrites[1].dstSet = descriptorSets[i];
      descriptorWrites[1].dstBinding = 1;
      descriptorWrites[1].dstArrayElement = 0;
      descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      descriptorWrites[1].descriptorCount = 1;
      descriptorWrites[1].pImageInfo = &imageInfo;

      vkUpdateDescriptorSets(vkInterface->GetGPUSoftInterface(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
  } //CreateDescriptorSets

  void CreateCommandBuffers() {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT); //Ignore

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(vkInterface->GetGPUSoftInterface(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate command buffers!");
    } //If CommandBufferAllocate
  } //Create Command Buffers


  inline void CreateSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      if (vkCreateSemaphore(vkInterface->GetGPUSoftInterface(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(vkInterface->GetGPUSoftInterface(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(vkInterface->GetGPUSoftInterface(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create synchronization objects for a frame!");
      }
    }
  }


  //Sec. Loop
  void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapChainExtent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    VkBuffer vertexBuffers[] = { vertBuff->GetBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indiceBuff->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);


    pushConst pC{};
    //pC.model = glm::vec3({ 1.0f, 0.0f, 1.0f });

    //vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pushConst), &pC);

    //vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(block->indices.size()), 1, 0, 0, 0);

    pC.model = glm::vec3(block->GetWorldPosVec());

    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(pushConst), &pC);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(block->indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }
  }

  void UpdateUniformBuffer(uint32_t currentImage) {
    UniformBufferObject ubo{};

    auto cam = currentScene->GetCamera();

    ubo.model = cam->GetWorldPosMat();
    ubo.proj = cam->SetPerspectiveProj(swapChainExtent.width, swapChainExtent.width);
    ubo.view = cam->GetViewMatrix();

    uniformBuffers[currentImage]->UpdateBuffer(&ubo);
  } //UpdateUniformBuffer

  void CleanupSwapChain() {
    vkDestroyImageView(vkInterface->GetGPUSoftInterface(), depthImageView, nullptr);
    vkDestroyImage(vkInterface->GetGPUSoftInterface(), depthImage, nullptr);
    vkFreeMemory(vkInterface->GetGPUSoftInterface(), depthImageMemory, nullptr);

    for (auto framebuffer : swapChainFramebuffers) {
      vkDestroyFramebuffer(vkInterface->GetGPUSoftInterface(), framebuffer, nullptr);
    }

    for (auto imageView : swapChainImageViews) {
      vkDestroyImageView(vkInterface->GetGPUSoftInterface(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(vkInterface->GetGPUSoftInterface(), swapChain, nullptr);
  } //CleanupSwapChain



  void RecreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(vkInterface->glfw->win, &width, &height);
    while (width == 0 || height == 0) {
      glfwGetFramebufferSize(vkInterface->glfw->win, &width, &height);
      glfwWaitEvents();
    }

    vkDeviceWaitIdle(vkInterface->GetGPUSoftInterface());

    CleanupSwapChain();

    CreateSwapChain();
    CreateImageViews();
    CreateDepthResources();
    CreateFramebuffers();
  } //RecreateSwapChain

  inline void RenderFrame() {
    vkWaitForFences(vkInterface->GetGPUSoftInterface(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    UINT32 imageIndex;
    VkResult result = vkAcquireNextImageKHR(vkInterface->GetGPUSoftInterface(), swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      RecreateSwapChain();
      return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image!");
    }

    UpdateUniformBuffer(currentFrame);

    vkResetFences(vkInterface->GetGPUSoftInterface(), 1, &inFlightFences[currentFrame]);

    vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    RecordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    //Note for future, this is clearly a semaphore used as a mutex.
    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(vkInterface->GetCmndQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
      throw std::runtime_error("failed to submit draw command buffer!");
    } //vkQueueSubmit

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(vkInterface->GetImageQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
      framebufferResized = false;
      RecreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
  }; //Render Frame


  void Loop() {
    while (!glfwWindowShouldClose(vkInterface->glfw->win)) {
      glfwPollEvents();
      RenderFrame(); //Move into Main Function?
    }

    vkDeviceWaitIdle(vkInterface->GetGPUSoftInterface());
  } //Loop



  PRIVATEPB::Vulkan13::Vulkan13(
    pb::Config::Render* rendConf,
    PRIVATEPB::Features* F,
    PRIVATEPB::Controls* C
  ) { //Vulkan13
    //These need to be separate due to the limits of the Pimpl Idiom
    auto featureSet = new FeatureSet();
    auto controlSet = new ControlSet();

    auto fHash = F->GetCameraVector();
    auto cHash = C->GetInputVector();
    featureSet->BuildCameraHash(fHash, rendConf);
    controlSet->BuildCameraHash(cHash, rendConf);
    
    delete F;
    
    //Ditch PbInterface, Replace with WorldSpace
    //pbInterface -> vkInterface -> GFXPipeline? -> Loop
    auto pbInterface = new PbInterface(rendConf, featureSet, controlSet);
    vkInterface = new VkInterface(rendConf);

    auto cam = currentScene->GetCamera();
    block = new Block(CUBE);

    //Debug Was Moved to the Wrong Spot, Move to New Class
    //Not necessary now, but add a scene system in the feature object using file loading

    //Sec. Pipe
    CreateSwapChain(); //SwapChain
    CreateImageViews(); //SwapChain
    CreateRenderPass(); //Send to RenderPass
    CreateDescriptorSetLayout(); //Send to DescriptorSet
    CreateGraphicsPipeline(); //Send to Pipeline and Shader

    //Sec. Cmnd
    CreateCommandPool(); //Relegate to PipeLine
    CreateDepthResources(); //SwapChain
    CreateFramebuffers(); //SwapChain
    CreateTextureImage(); //Move to Texture - split to BufferInput as appropriate
    CreateTextureImageView(); //Move to Texture
    CreateTextureSampler(); //Move to Texture
    CreateUniformBuffers(); //Send to Buffers
    CreateDescriptorPool(); //Move to DescriptorSets
    CreateDescriptorSets(); //Move to DescriptorSets
    CreateCommandBuffers(); //Relegate to BufferOutput
    CreateSyncObjects(); //Pipeline

    //Eventually Move these here 
    block->SetWorldPos(0, 0, 0);
    block->SetBlockColor({ 0.0f,0.0f,0.0f });
    block->Activate();

    CreateVertexBuffer();
    CreateIndexBuffer();
    //Sec. Loop
    Loop();

  }; //Vulkan



  PRIVATEPB::Vulkan13::~Vulkan13() {
    CleanupSwapChain();

    vkDestroyPipeline(vkInterface->GetGPUSoftInterface(), graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(vkInterface->GetGPUSoftInterface(), pipelineLayout, nullptr);
    vkDestroyRenderPass(vkInterface->GetGPUSoftInterface(), renderPass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      delete uniformBuffers[i];
    } //For MAX_FRAMES

    vkDestroyDescriptorPool(vkInterface->GetGPUSoftInterface(), descriptorPool, nullptr);

    vkDestroySampler(vkInterface->GetGPUSoftInterface(), textureSampler, nullptr);
    vkDestroyImageView(vkInterface->GetGPUSoftInterface(), textureImageView, nullptr);

    vkDestroyImage(vkInterface->GetGPUSoftInterface(), textureImage, nullptr);
    vkFreeMemory(vkInterface->GetGPUSoftInterface(), textureImageMemory, nullptr);

    vkDestroyDescriptorSetLayout(vkInterface->GetGPUSoftInterface(), descriptorSetLayout, nullptr);

    //vkDestroyBuffer(vkInterface->GetGPUSoftInterface(), indexBuffer, nullptr);
    //vkFreeMemory(vkInterface->GetGPUSoftInterface(), indexBufferMemory, nullptr);

    //vkDestroyBuffer(vkInterface->GetGPUSoftInterface(), vertexBuffer, nullptr);
    //vkFreeMemory(vkInterface->GetGPUSoftInterface(), vertexBufferMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      vkDestroySemaphore(vkInterface->GetGPUSoftInterface(), renderFinishedSemaphores[i], nullptr);
      vkDestroySemaphore(vkInterface->GetGPUSoftInterface(), imageAvailableSemaphores[i], nullptr);
      vkDestroyFence(vkInterface->GetGPUSoftInterface(), inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(vkInterface->GetGPUSoftInterface(), commandPool, nullptr);

    vkDestroyDevice(vkInterface->GetGPUSoftInterface(), nullptr);

    if (DEBUG) {
      DestroyDebugUtilsMessengerEXT(vkInterface->vulkan, vkInterface->debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(vkInterface->vulkan, vkInterface->windowSurface, nullptr);
    vkDestroyInstance(vkInterface->vulkan, nullptr);

    glfwDestroyWindow(vkInterface->glfw->win);

    glfwTerminate();
  }; //~Vulkan