#include <PRVTPB.h>

//PIMPLE IMPLEMENTATION 

//CONFIG
struct PRIVATEPB::Config {
  Config() {
    Confirmed = 0x00; 
    WrotetoUtil = 0x00; 
    WrotetoRender = 0x00; 
    errBuffer = new std::vector < const char*>();
  }; //Config

  //Getters
  pb::Config::Utils* GetUtils() { return U; };
  pb::Config::Render* GetRender() { return R; };
  bool GetConfirmed() { return Confirmed; };
  bool GetWrotetoUtil() { return WrotetoUtil; };
  bool GetWrotetoRender() { return WrotetoRender; };
  std::vector<const char*> GetErrBuff() { return *errBuffer; };


  //Setters
  void SetUtils(pb::Config::Utils* u) { U = u; };
  void SetRender(pb::Config::Render* r) { R = r; };
  void SetConfirmed(bool b) { Confirmed = b; };
  void SetWrotetoUtil(bool b) { WrotetoUtil = b; };
  void SetWrotetoRender(bool b) { WrotetoRender = b; };

  void ExtndErrBuff(const char* str) {
    errBuffer->emplace_back(str);
  }; //ExtndErrBuffer

  ~Config() {
    delete U;
    delete R;
    
    for (auto err : *errBuffer) {
      delete err;
    }; //For

    delete errBuffer;
  }; //Config

private:
  std::vector<const char*>* errBuffer;
  pb::Config::Utils* U; //Utils
  pb::Config::Render* R; //Render

  bool Confirmed : 1;
  bool WrotetoUtil : 1;
  bool WrotetoRender : 1;

}; //Config


//CLIENT
struct PRIVATEPB::Client {
  PRIVATEPB::Utils* Utils;

  PRIVATEPB::Config* Conf =
    new PRIVATEPB::Config();

  void SetConfirmed(bool b) { Confirmed = b; };
  bool GetConfirmed(bool b) { return Confirmed; };

  ~Client() {
    delete Utils;
    delete Conf;
  }; //Client

private:
  bool Confirmed;

}; //Client

struct PRIVATEPB::ClientVector {
  ClientVector() {
    vector = new std::vector<PRIVATEPB::Client*>();

    innerIndice -= 1;
    outerIndice -= 1;

    NewClient();
  }; //ClientVector


  //New
  void NewClient() {
    innerIndice += 1;
    outerIndice += 1;

    vector->emplace_back(
      new PRIVATEPB::Client());
  }; //AddClient


  PRIVATEPB::Utils* NewUtils() {
    return vector->operator[](innerIndice)->Utils =
      new PRIVATEPB::Utils(
        GetLatestConfig()
        ->GetUtils());
  }; //NewUtils


  PRIVATEPB::Config* NewConfig() {
    return vector->operator[](innerIndice)->Conf =
      new PRIVATEPB::Config();
  }; //NewConfig



  //Get Items
  PRIVATEPB::Client* GetClient(UINT indice) {
    return vector->operator[](indice);
  }; //GetClient


  PRIVATEPB::Client* GetLatestClient() {
    return vector->operator[](innerIndice);
  }; //GetClient


  PRIVATEPB::Client* GetCurrentClient() {
    return vector->operator[](currentIndice);
  }; //GetClient


  PRIVATEPB::Config* GetCurrentConfig() {
    return vector->operator[](currentIndice)->Conf;
  }; //GetClient

  PRIVATEPB::Config* GetLatestConfig() {
    return vector->operator[](innerIndice)->Conf;
  }; //GetClient


  PRIVATEPB::Utils* GetLatestUtils() {
    return vector->operator[](innerIndice)->Utils;
  }; //GetClient


  std::vector
    <PRIVATEPB::Client*> GetClientVector() {
    return *vector;
  }; //GetClient


  //Set Items
  PRIVATEPB::Utils* SetLatestUtils(
    PRIVATEPB::Utils* U) {

    return vector->operator[](innerIndice)->Utils = U;
  }; //GetClient


  PRIVATEPB::Config* SetLatestConfig(
    PRIVATEPB::Config* C) {

    return vector->operator[](innerIndice)->Conf = C;
  }; //GetClient


  ~ClientVector() {
    for (auto c : *vector) {
      delete c;
    };

    delete vector;
  }; //ClientVector


private:
  UINT currentIndice = 0;
  UINT innerIndice = 0;
  UINT outerIndice = 1;

  std::vector<PRIVATEPB::Client*>* vector;

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



void pb::Config::ConfirmConfigs() {
  PRIVATEPB::Client_ptr->GetLatestConfig()
    ->SetConfirmed(true);

  auto errBuff = PRIVATEPB::Client_ptr
    ->GetLatestConfig()
    ->GetErrBuff();

  PRIVATEPB::Client_ptr
    ->NewUtils();

  for (auto& str : errBuff) {
    IR("Writing Configurations", "Assigning Conf to Client", str);
  }; //For

  pb::Utils::Output::FlushtoLog();
}; //ConfirmConfigs


void pb::Client::ConfirmClients() {
  IR("Attempting Client Vector Confirmation", "Enabeling Client Lock", "Attempting Once");
  
  auto vector =
    PRIVATEPB::Client_ptr
    ->GetClientVector();

  for (auto& item : vector) {
    item->SetConfirmed(true);
  }; //For

  pb::Utils::Output::FlushtoLog();
}; //ConfirmConfigs

