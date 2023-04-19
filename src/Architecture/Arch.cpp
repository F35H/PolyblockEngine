#include <PRVTPB.h>

//CONFIG
struct PRIVATEPB::Config {
  Config() {
    Confirmed = 0x00; 
    WrotetoUtil = 0x00; 
    WrotetoRender = 0x00; 
    errBuffer = std::vector <const char*>();
  }; //Config

  //Getters
  pb::Config::Utils* GetUtils() { return U; };
  pb::Config::Render* GetRender() { return R; };
  bool GetConfirmed() { return Confirmed; };
  bool GetWrotetoUtil() { return WrotetoUtil; };
  bool GetWrotetoRender() { return WrotetoRender; };
  std::vector<const char*> GetErrBuff() { return errBuffer; };


  //Setters
  void SetUtils(pb::Config::Utils* u) { U = u; };
  void SetRender(pb::Config::Render* r) { R = r; };
  void SetConfirmed(bool b) { Confirmed = b; };
  void SetWrotetoUtil(bool b) { WrotetoUtil = b; };
  void SetWrotetoRender(bool b) { WrotetoRender = b; };

  void ExtndErrBuff(const char* str) {
    errBuffer.emplace_back(str);
  }; //ExtndErrBuffer

  ~Config() {
    delete U;
    delete R;
    
  }; //Config

private:
  std::vector<const char*> errBuffer;
  pb::Config::Utils* U; //Utils
  pb::Config::Render* R; //Render

  bool Confirmed : 1;
  bool WrotetoUtil : 1;
  bool WrotetoRender : 1;

}; //Config

struct PRIVATEPB::Utils {
  std::ofstream* logFile;

  Utils(pb::Config::Utils* U) {
    std::string fileName = "gameLog";
    fileName += ".txt";

    logFile = new std::ofstream(
      fileName, std::ios::ate | std::ios::out);

    int i = 0;
    auto cout = U->GetLogBuffer();
    const char* errMsg;

    do { //Create Log File
      switch (i) {
      default:
        errMsg = "Attempting Log File Creation | File Creation Imminent | Attempting Once \n";
        cout->write(errMsg, strlen(errMsg));
        logFile->write(errMsg, strlen(errMsg));
        break;

      case 0b01:
        errMsg = "Log File Creation Failed | File Creation Primary Attempt | Attempting Twice \n";
        cout->write(errMsg, strlen(errMsg));
        break;

      case 0b10:
        errMsg = "Log File Creation Failed | File Creation Secondary Attempt | Attempting Trice \n";
        cout->write(errMsg, strlen(errMsg));
        break;

      case 0b11:
        errMsg = "Log File Creation Failed | File Creation Tertiary Attempt | Termination Imminent \n";
        cout->write(errMsg, strlen(errMsg));
        abort();

      }; //Switch

      ++i;
    } while (!logFile->is_open());

  }; //UTILSCONSTRUCTOR

  ~Utils() {
    delete logFile;
  }; //Utils

}; //Utils

//FEATURES
struct PRIVATEPB::Features {
  Features() :
    CameraHash({})
  {};

  //Getters
  std::unordered_map
    <const char*, pb::Feature::Camera> 
    GetCameraHash() 
  { return CameraHash; };
  
  bool GetConfirmed() { return Confirmed; };

  //Setters
  void SetConfirmed(bool b) { Confirmed = b; };

private:
  std::unordered_map<const char*, pb::Feature::Camera> CameraHash;

  bool Confirmed;
}; //FEATURES


//CLIENT
struct PRIVATEPB::Client {
  PRIVATEPB::Utils* Utils;

  PRIVATEPB::Config* Conf =
    new PRIVATEPB::Config();

  PRIVATEPB::Features* Features =
    new PRIVATEPB::Features();

  void SetConfirmed(bool b) { Confirmed = b; };
  bool GetConfirmed(bool b) { return Confirmed; };

  ~Client() {
    delete Features;
    delete Utils;
    delete Conf;
  }; //Client

private:
  bool Confirmed;

}; //Client

struct PRIVATEPB::ClientVector {
  ClientVector() {
    vector = std::vector<PRIVATEPB::Client*>();

    innerIndice -= 1;
    outerIndice -= 1;

    NewClient();
  }; //ClientVector


  //New
  void NewClient() {
    innerIndice += 1;
    outerIndice += 1;

    vector.emplace_back(
      new PRIVATEPB::Client());
  }; //AddClient


  PRIVATEPB::Utils* NewUtils() {
    return vector[innerIndice]->Utils =
      new PRIVATEPB::Utils(
        GetLatestConfig()
        ->GetUtils());
  }; //NewUtils


  PRIVATEPB::Config* NewConfig() {
    return vector[innerIndice]->Conf =
      new PRIVATEPB::Config();
  }; //NewConfig



  //Get Items
  PRIVATEPB::Client* GetClient(UINT indice) {
    return vector[indice];
  }; //GetClient


  PRIVATEPB::Client* GetLatestClient() {
    return vector[innerIndice];
  }; //GetClient


  PRIVATEPB::Client* GetCurrentClient() {
    return vector[currentIndice];
  }; //GetClient


  PRIVATEPB::Config* GetCurrentConfig() {
    return vector[currentIndice]->Conf;
  }; //GetClient

  PRIVATEPB::Config* GetLatestConfig() {
    return vector[innerIndice]->Conf;
  }; //GetClient


  PRIVATEPB::Features* GetCurrentFeatures() {
    return vector[currentIndice]->Features;
  }; //GetCurrentFeatures

  PRIVATEPB::Features* GetLatestFeatures() {
    return vector[innerIndice]->Features;
  }; //GetLatestFeatures

  PRIVATEPB::Utils* GetLatestUtils() {
    return vector[innerIndice]->Utils;
  }; //GetClient


  std::vector <PRIVATEPB::Client*> GetClientVector() {
    return vector;
  }; //GetClient


  //Set Items
  PRIVATEPB::Utils* SetLatestUtils(
    PRIVATEPB::Utils* U) {

    return vector[innerIndice]->Utils = U;
  }; //GetClient


  PRIVATEPB::Config* SetLatestConfig(
    PRIVATEPB::Config* C) {

    return vector[innerIndice]->Conf = C;
  }; //GetClient


  PRIVATEPB::Features* SetLatestFeatures(
    PRIVATEPB::Features* F) {

    return vector[innerIndice]->Features = F;
  }; //GetFeatures

  ~ClientVector() {
  }; //ClientVector


private:
  UINT currentIndice = 0;
  UINT innerIndice = 0;
  UINT outerIndice = 1;

  std::vector<PRIVATEPB::Client*> vector;

}; //ClientVector

PRIVATEPB::ClientVector* PRIVATEPB::Client_ptr = new ClientVector();


//PUBLIC IMPLEMENTATION 
void pb::Config::AddConfig(pb::Config::Utils* U) {
  if (!PRIVATEPB::Client_ptr->GetLatestConfig()->GetWrotetoUtil()) {

    PRIVATEPB::Client_ptr
      ->GetLatestConfig()
      ->SetUtils(U);

    PRIVATEPB::Client_ptr
      ->GetLatestConfig()
      ->SetWrotetoUtil(true);

  } //IF
  else {
    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->ExtndErrBuff(
        "Util Conf Assignment Failed: Client Aleready Written to!");
  }; //ELSE

}; //AddConfig


void pb::Config::AddConfig(pb::Config::Render* R) {
  if (!PRIVATEPB::Client_ptr
    ->GetLatestConfig()
    ->GetWrotetoRender()) {

    PRIVATEPB::Client_ptr
      ->GetLatestConfig()
      ->SetRender(R);


    PRIVATEPB::Client_ptr
      ->GetLatestConfig()
      ->SetWrotetoRender(true);

  } //IF
  else {
    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->ExtndErrBuff("Render Conf Assignment Failed: Client Aleready Written to!");
  }; //ELSE
}; //Add Config


void pb::Feature::AddFeature(pb::Feature::Camera* Cam) {
  auto Configs = PRIVATEPB::Client_ptr
    ->GetLatestConfig();

  auto Features = PRIVATEPB::Client_ptr
    ->GetLatestFeatures();
  
  std::string micro = Cam->GetName();

  if (!Features
    ->GetConfirmed()
    & Configs
    ->GetConfirmed()
    ) {

    Features
      ->GetCameraHash()
      .try_emplace(Cam->GetName(), Cam);
  } //if Confirmed
  else {
    micro += " : Failure, Confirmation Order";
    
    InternalReport(
      "Writing Features",
      "Adding Camera",
      micro.c_str()
    ); //InternalReport
  }; //Else

  std::string micro = Cam->GetName();
  micro += " : Success";

  InternalLog(
    "Writing Features",
    "Adding Camera",
    micro.c_str()
  ); //InternalReport
}; //AddFeature


void pb::Config::ConfirmConfigs() {
  PRIVATEPB::Client_ptr->GetLatestConfig()
    ->SetConfirmed(true);

  auto errBuff = PRIVATEPB::Client_ptr
    ->GetLatestConfig()
    ->GetErrBuff();

  PRIVATEPB::Client_ptr
    ->NewUtils();

  for (auto& str : errBuff) {
    InternalLog("Writing Configurations", "Assigning Conf to Client", str);
  }; //For

  pb::Utils::Output::FlushtoLog();
}; //ConfirmConfigs


void pb::Feature::ConfirmFeatures() {
  auto Configs = PRIVATEPB::Client_ptr
    ->GetLatestConfig();

  auto Features = PRIVATEPB::Client_ptr
    ->GetLatestFeatures();

  if (!Features
    ->GetConfirmed()
    & Configs
    ->GetConfirmed()
    ) {

    PRIVATEPB::Client_ptr->GetLatestFeatures()
      ->SetConfirmed(true);

    InternalLog("Writing Features", "Assigning Features to Client", "Confirming");

    pb::Utils::Output::FlushtoLog();
  } //IfConfirmed
  else {
    InternalReport(
      "Writing Features", 
      "Assigning Features to Client", 
      "Failure: Confirmation Order Incorrect"
    ); //InternalReport
  }; //If Confirmed
}; //AddFeature

void pb::Client::ConfirmClients() {
  InternalLog("Attempting Client Vector Confirmation", "Enabeling Client Lock", "Attempting Once");
  
  auto vector =
    PRIVATEPB::Client_ptr
    ->GetClientVector();

  for (auto& item : vector) {
    item->SetConfirmed(true);
  }; //For

  pb::Utils::Output::FlushtoLog();
}; //ConfirmConfigs


inline void pb::RunRender() {
  auto rendConf = PRIVATEPB::Client_ptr
    ->GetCurrentConfig()
    ->GetRender();

  auto g = new PRIVATEPB::GFX(rendConf);
  delete g;
}; //RunRender
