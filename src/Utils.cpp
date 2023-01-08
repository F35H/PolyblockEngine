#include <PRVTPB.h>

#define PRVTPB //PIMPLE


//CONFIG
struct PRIVATEPB::Config {
  Config() :
    Confirmed(0b00),
    WrotetoUtil(0b00),
    WrotetoRender(0b00)
  {}; //Config


  //Getters
  std::shared_ptr<pb::Config::Utils> GetUtils() { return U; };
  std::shared_ptr<pb::Config::Render> GetRender() { return R; };
  bool GetConfirmed() { return Confirmed; };
  bool GetWrotetoUtil() { return WrotetoUtil; };
  bool GetWrotetoRender() { return WrotetoRender; };


  //Setters
  void SetUtils(std::shared_ptr<pb::Config::Utils> u) { U = u; };
  void SetRender(std::shared_ptr<pb::Config::Render> r) { R = r; };
  void SetConfirmed(bool b) { Confirmed = b; };
  void SetWrotetoUtil(bool b) { WrotetoUtil = b; };
  void SetWrotetoRender(bool b) { WrotetoRender = b; };


private:
  bool Confirmed : 1;
  bool WrotetoUtil : 1;
  bool WrotetoRender : 1;

  std::shared_ptr<pb::Config::Utils> U; //Utils
  std::shared_ptr<pb::Config::Render> R; //Render
}; //Config


//CLIENT
struct PRIVATEPB::Client {
  std::shared_ptr<PRIVATEPB::Utils> Utils;

  std::shared_ptr<PRIVATEPB::Config> Conf =
    std::make_shared<PRIVATEPB::Config>();

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
      std::move(std::make_shared<PRIVATEPB::Utils>());
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



  //Set Items
  std::shared_ptr<PRIVATEPB::Utils> SetLatestUtils(
    std::shared_ptr<PRIVATEPB::Utils> U) {

    return vector->operator[](innerIndice)->Utils = U;
  }; //GetClient


  std::shared_ptr<PRIVATEPB::Config> SetLatestConfig(
    std::shared_ptr<PRIVATEPB::Config> C) {

    return vector->operator[](innerIndice)->Conf = C;
  }; //GetClient


private:
  UINT currentIndice = 0;
  UINT innerIndice = 0;
  UINT outerIndice = 1;

  std::shared_ptr <
    std::vector<
    std::shared_ptr<PRIVATEPB::Client>
    >>  vector;

}; //ClientVector


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



//Output & Config
void pb::Utils::Output::FlushtoLog() {
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto& logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  logfile->flush();
  cout->flush();
}; //Output


void pb::Utils::Output::WritetoLog(const std::string str) {
  auto cout = PRIVATEPB::Client_ptr->GetLatestConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  cout->write(str.c_str(), str.length());
  logfile->write(str.c_str(), str.length());

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoLog


void pb::Utils::Output::WritetoLog(const char* str) {
  auto cout = PRIVATEPB::Client_ptr->GetLatestConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  cout->write(str, strlen(str));
  logfile->write(str, strlen(str));

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoLog


void pb::Utils::Output::WritetoTimedLog(const char* str) {
  auto cout = PRIVATEPB::Client_ptr->GetLatestConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  cout->write(str, strlen(str));
  logfile->write(str, strlen(str));

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoTimedLog


void pb::Utils::Output::WritetoTimedLog(const std::string str) {
  auto cout = PRIVATEPB::Client_ptr->GetLatestConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  cout->write(str.c_str(), str.length());
  logfile->write(str.c_str(), str.length());

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WriteToTimedLog


std::ostream* pb::Config::Utils::GetLogBuffer() {
  return logBuffer;
}; //GetLogBuffer


void pb::Config::Utils::SetLogBuffer(std::ostream* strm) {
  logBuffer = strm;
}; //SetLogBuffer
