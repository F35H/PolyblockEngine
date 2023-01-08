#include <pbMacros.h>
#include <PRVTPB.h>

#define PRVTPB //PIMPLE

//BEGIN CLASS MACRO



//CONFIG
struct PRIVATEPB::Config {
  Config() :
    Confirmed(0b00),
    WrotetoUtil(0b00),
    WrotetoRender(0b00),
    errBuffer(
      std::make_shared<
      std::vector<std::string>>())
  {}; //Config


  ~Config() {};


  //Getters
  std::shared_ptr<pb::Config::Utils> GetUtils() { return U; };
  std::shared_ptr<pb::Config::Render> GetRender() { return R; };
  bool GetConfirmed() { return Confirmed; };
  bool GetWrotetoUtil() { return WrotetoUtil; };
  bool GetWrotetoRender() { return WrotetoRender; };
  std::vector<std::string> GetErrBuff() { return *errBuffer.get(); };


  //Setters
  void SetUtils(std::shared_ptr<pb::Config::Utils> u) { U = u; };
  void SetRender(std::shared_ptr<pb::Config::Render> r) { R = r; };
  void SetConfirmed(bool b) { Confirmed = b; };
  void SetWrotetoUtil(bool b) { WrotetoUtil = b; };
  void SetWrotetoRender(bool b) { WrotetoRender = b; };

  void ExtndErrBuff(std::string str) {
    errBuffer->emplace_back(str);
  }; //ExtndErrBuffer

private:
  std::shared_ptr<std::vector<std::string>> errBuffer;
  std::shared_ptr<pb::Config::Utils> U; //Utils
  std::shared_ptr<pb::Config::Render> R; //Render


  bool Confirmed : 1;
  bool WrotetoUtil : 1;
  bool WrotetoRender : 1;



}; //Config


//CLIENT
struct PRIVATEPB::Client {
  std::shared_ptr<PRIVATEPB::Utils> Utils;

  std::shared_ptr<PRIVATEPB::Config> Conf =
    std::make_shared<PRIVATEPB::Config>();

  ~Client() {};

  void SetConfirmed(bool b) { Confirmed = b; };
  bool GetConfirmed(bool b) { return Confirmed; };

private:
  bool Confirmed;

}; //Client


struct PRIVATEPB::ClientVector {
  ClientVector() {
    vector = std::make_shared< std::vector
      <std::shared_ptr<PRIVATEPB::Client>>>();

    innerIndice -= 1;
    outerIndice -= 1;

    NewClient();
  }; //ClientVector


  //New
  void NewClient() {
    innerIndice += 1;
    outerIndice += 1;

    vector->emplace_back(
      std::make_shared<PRIVATEPB::Client>());
  }; //AddClient


  std::shared_ptr<PRIVATEPB::Utils> NewUtils() {
    return vector->operator[](innerIndice)->Utils =
      std::make_shared<PRIVATEPB::Utils>();
  }; //NewUtils


  std::shared_ptr<PRIVATEPB::Config> NewConfig() {
    return vector->operator[](innerIndice)->Conf =
      std::make_shared<PRIVATEPB::Config>();
  }; //NewConfig



  //Get Items
  std::shared_ptr<PRIVATEPB::Client> GetClient(UINT indice) {
    return vector->operator[](indice);
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Client> GetLatestClient() {
    return vector->operator[](innerIndice);
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Client> GetCurrentClient() {
    return vector->operator[](currentIndice);
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Config> GetCurrentConfig() {
    return vector->operator[](currentIndice)->Conf;
  }; //GetClient

  std::shared_ptr<PRIVATEPB::Config> GetLatestConfig() {
    return vector->operator[](innerIndice)->Conf;
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Utils> GetLatestUtils() {
    return vector->operator[](innerIndice)->Utils;
  }; //GetClient


  std::vector
    <std::shared_ptr<PRIVATEPB::Client>> GetClientVector() {
    return *vector.get();
  }; //GetClient


  //Set Items
  std::shared_ptr<PRIVATEPB::Utils> SetLatestUtils(
    std::shared_ptr<PRIVATEPB::Utils> U) {

    return vector->operator[](innerIndice)->Utils = U;
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Config> SetLatestConfig(
    std::shared_ptr<PRIVATEPB::Config> C) {

    return vector->operator[](innerIndice)->Conf = C;
  }; //GetClient


  ~ClientVector() {};


private:
  UINT currentIndice = 0;
  UINT innerIndice = 0;
  UINT outerIndice = 1;

  std::shared_ptr <
    std::vector<
    std::shared_ptr<PRIVATEPB::Client>
    >>  vector;

}; //ClientVector


std::shared_ptr<PRIVATEPB::ClientVector> PRIVATEPB::Client_ptr = std::shared_ptr<ClientVector>(new ClientVector());


struct PRIVATEPB::Utils {
  std::shared_ptr<std::ofstream> logFile;

  Utils() {
    std::string uuid = sole::uuid1().str();

    std::string fileName = "logs";
    fileName += "/log-";
    fileName += uuid;
    fileName += ".txt";

    logFile = std::move(
      std::shared_ptr<std::ofstream>(new std::ofstream(
        "filename", std::ios::ate | std::ios::out)));

    int i = 0;
    auto cout = PRIVATEPB::Client_ptr->GetLatestConfig()->GetUtils()->GetLogBuffer();
    const char* errMsg;

    do { //Create Log File
      switch (i) {
      default:
        errMsg = "Attempting Log File Creation | File Creation Imminent | Attempting Once";
        cout->write(errMsg, sizeof(errMsg));
        continue;

      case 0b01:
        errMsg = "Log File Creation Failed | File Creation Primary Attempt | Attempting Twice";
        cout->write(errMsg, sizeof(errMsg));
        continue;

      case 0b10:
        errMsg = "Log File Creation Failed | File Creation Secondary Attempt | Attempting Trice";
        cout->write(errMsg, sizeof(errMsg));
        continue;

      case 0b11:
        errMsg = "Log File Creation Failed | File Creation Tertiary Attempt | Termination Imminent";
        cout->write(errMsg, sizeof(errMsg));
        abort();

      }; //Switch

      ++i;
    } while (!logFile->is_open());

  }; //UTILSCONSTRUCTOR

}; //Utils


class Control {
}; //Control


class Features {
}; //Features


void pb::Config::AddConfig(pb::Config::Utils* U) {
  if (!PRIVATEPB::Client_ptr->GetLatestConfig()->GetWrotetoUtil()) {

    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->SetUtils(
        std::move(std::shared_ptr
          <pb::Config::Utils>(U))
      ); //SetUtils

    PRIVATEPB::Client_ptr->GetLatestConfig()->SetWrotetoUtil(true);
  } //IF
  else {
    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->ExtndErrBuff("Util Already Wrriten to in Config \n");
  }; //ELSE

}; //AddConfig


void pb::Config::AddConfig(pb::Config::Render* R) {
  if (!PRIVATEPB::Client_ptr->GetLatestConfig()
    ->GetWrotetoRender()) {

    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->SetRender(
        std::move(std::shared_ptr
          <pb::Config::Render>(R))
      ); //Set Render


    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->SetWrotetoRender(true);
  } //IF
  else {
    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->ExtndErrBuff("Util Already Wrriten to in Config \n");
  }; //ELSE
}; //Add Config


void pb::Config::AddConfig(std::shared_ptr<pb::Config::Utils> U) {
  if (!PRIVATEPB::Client_ptr->GetLatestConfig()
    ->GetWrotetoUtil()) {

    PRIVATEPB::Client_ptr->GetLatestConfig()->
      SetUtils(U);

    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->SetWrotetoUtil(true);
  } //IF
  else {
    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->ExtndErrBuff("Util Already Wrriten to in Config \n");
  }; //ELSE
}; //AddConfig


void pb::Config::AddConfig(std::shared_ptr<pb::Config::Render> R) {
  if (!PRIVATEPB::Client_ptr->GetLatestConfig()
    ->GetWrotetoRender()) {

    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->SetRender(R);

    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->SetWrotetoRender(true);

  } //IF
  else {
    PRIVATEPB::Client_ptr->GetLatestConfig()
      ->ExtndErrBuff("Util Already Wrriten to in Config \n");
  }; //ELSE
}; //AddConfig



void pb::Config::ConfirmConfigs() {
  PRIVATEPB::Client_ptr->GetLatestConfig()
    ->SetConfirmed(true);

  auto errBuff = PRIVATEPB::Client_ptr->GetLatestConfig()
    ->GetErrBuff();

  PRIVATEPB::Client_ptr->NewUtils();

  for (auto& str : errBuff) {
    pb::Utils::Output::WritetoTimedLog(str);
  }; //For

  pb::Utils::Output::FlushtoLog();
}; //ConfirmConfigs


void pb::Client::ConfirmClients() {
  auto vector =
    PRIVATEPB::Client_ptr
    ->GetClientVector();

  for (auto& item : vector) {
    item->SetConfirmed(true);
  }; //For

  auto errBuff = PRIVATEPB::Client_ptr->GetLatestConfig()
    ->GetErrBuff();

  PRIVATEPB::Client_ptr->NewUtils();

  for (auto& str : errBuff) {
    pb::Utils::Output::WritetoTimedLog(str);
  }; //For

  pb::Utils::Output::FlushtoLog();
}; //ConfirmConfigs
