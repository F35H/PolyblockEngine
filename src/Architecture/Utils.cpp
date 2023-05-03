#include <PRVTPB.h>

#include <csv.hpp>

#define PRVTPB //PIMPLE

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

//Output & Config
void pb::Utils::Output::FlushtoLog() {
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto& logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  logfile->flush();
  cout->flush();
}; //Output


void pb::Utils::Output::WritetoLog(const char* str) noexcept {
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  cout->write(str, strlen(str));
  logfile->write(str, strlen(str));

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoLog
void pb::Utils::Output::WritetoLog(const char* macro, const char* micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  char rtrnStr[0xFFF]; //4096

  char buffer[0x40];
  strcpy(buffer, macro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, micro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  cout->write(rtrnStr, strlen(rtrnStr));
  logfile->write(rtrnStr, strlen(rtrnStr));

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoTimedLog
void pb::Utils::Output::WritetoLog(const char* macro, const char* meso, const char* micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  char rtrnStr[0xFFF];

  char buffer[0x40];
  strcpy(buffer, macro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, meso);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, micro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  cout->write(rtrnStr, strlen(rtrnStr));
  logfile->write(rtrnStr, strlen(rtrnStr));

  cout->write("\n", 1);
  logfile->write("\n", 1);
}; //WritetoTimedLog

void pb::Utils::Output::WritetoTimedLog(const char* str) noexcept {
  using namespace std::chrono;
  
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  char rtrnStr[0xFFF]; //4096

  auto timeStr = ctime(&rtrnTime);

  timeStr[strlen(timeStr) - 1] = '\0';
  strcpy(rtrnStr, timeStr);
  strcat(rtrnStr, " | ");
  strcat(rtrnStr, str);

  cout->write(rtrnStr, strlen(rtrnStr));
  logfile->write(rtrnStr, strlen(rtrnStr));

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoTimedLog
void pb::Utils::Output::WritetoTimedLog(const char* macro, const char* micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  char rtrnStr[0xFFF]; //4096

  auto timeStr = ctime(&rtrnTime);

  timeStr[strlen(timeStr) - 1] = '\0';
  strcpy(rtrnStr, timeStr);

  char buffer[0x40];
  strcpy(buffer, macro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, micro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  cout->write(rtrnStr, strlen(rtrnStr));
  logfile->write(rtrnStr, strlen(rtrnStr));

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoTimedLog
void pb::Utils::Output::WritetoTimedLog(const char* macro, const char* meso, const char* micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  char rtrnStr[0xFFF]; //4096

  auto timeStr = ctime(&rtrnTime);

  timeStr[strlen(timeStr) - 1] = '\0';
  strcpy(rtrnStr, timeStr);

  char buffer[0x40];
  strcpy(buffer, macro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, meso);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, micro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  cout->write(rtrnStr, strlen(rtrnStr));
  logfile->write(rtrnStr, strlen(rtrnStr));

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoTimedLog

void pb::Utils::Output::WritetoLog(const std::string str) noexcept {
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  *cout << (str.c_str());
  *logfile << (str.c_str());

  *cout << "\n";
  *logfile << "\n";
}; //WritetoLog
void pb::Utils::Output::WritetoLog(std::string macro, std::string micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  macro = (macro.length() > 64) ? "String Length > 64 Chars" : macro;
  micro = (micro.length() > 64) ? "String Length > 64 Chars" : micro;

  if (macro.length() < 64) {
    for (FINT16 i = 0; (64 - macro.length()); ++i) {
      macro.append(" ");
    }; //for FINT
  }; //if macro

  if (micro.length() < 64) {
    for (FINT16 i = 0; (64 - micro.length()); ++i) {
      micro.append(" ");
    }; //for FINT
  }; //if micro

  auto rtrnStr = std::move(macro);
  rtrnStr += " | ";
  rtrnStr += std::move(micro);

  cout->write(rtrnStr.c_str(), rtrnStr.length());
  logfile->write(rtrnStr.c_str(), rtrnStr.length());

  cout->write("\n", 1);
  logfile->write("\n", 1);
}; //WriteToLog
void pb::Utils::Output::WritetoLog(std::string macro, std::string meso, std::string micro) noexcept {
  using namespace std::chrono;
  
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  macro = (macro.length() > 64) ? "String Length > 64 Chars" : macro;
  meso = (meso.length() > 64) ? "String Length > 64 Chars" : meso;
  micro = (micro.length() > 64) ? "String Length > 64 Chars" : micro;

  if (macro.length() < 64) {
    for (FINT16 i = 0; (64 - macro.length()); ++i) {
      macro.append(" ");
    }; //for FINT
  }; //if macro

  if (meso.length() < 64) {
    for (FINT16 i = 0; (64 - meso.length()); ++i) {
      meso.append(" ");
    }; //for FINT
  }; //if macro

  if (micro.length() < 64) {
    for (FINT16 i = 0; (64 - micro.length()); ++i) {
      micro.append(" ");
    }; //for FINT
  }; //if macro

  auto rtrnStr = std::move(macro);
  rtrnStr += " | ";
  rtrnStr += std::move(meso);
  rtrnStr += " | ";
  rtrnStr += std::move(micro);

  cout->write(rtrnStr.c_str(), rtrnStr.length());
  logfile->write(rtrnStr.c_str(), rtrnStr.length());

  cout->write("\n", 1);
  logfile->write("\n", 1);
}; //WriteToLog

void pb::Utils::Output::WritetoTimedLog(std::string str) noexcept {
  using namespace std::chrono;
  
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  std::string rtrnStr = ctime(&rtrnTime);
  rtrnStr.pop_back();
  rtrnStr += " | ";
  rtrnStr += str;

  cout->write(rtrnStr.c_str(), str.length());
  logfile->write(rtrnStr.c_str(), str.length());

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WriteToTimedLog
void pb::Utils::Output::WritetoTimedLog(std::string macro, std::string micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  macro = (macro.length() > 64) ? "String Length > 64 Chars" : macro;
  micro = (micro.length() > 64) ? "String Length > 64 Chars" : micro;

  if (macro.length() < 64) {
    for (FINT16 i = 0; (64 - macro.length()); ++i) {
      macro.append(" ");
    }; //for FINT
  }; //if macro

  if (micro.length() < 64) {
    for (FINT16 i = 0; (64 - micro.length()); ++i) {
      micro.append(" ");
    }; //for FINT
  }; //if macro

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  std::string rtrnStr = ctime(&rtrnTime);
  rtrnStr.pop_back();
  rtrnStr += " | ";

  rtrnStr = std::move(macro);
  rtrnStr += " | ";
  rtrnStr += std::move(micro);

  cout->write(rtrnStr.c_str(), rtrnStr.length());
  logfile->write(rtrnStr.c_str(), rtrnStr.length());

  cout->write("\n", 1);
  logfile->write("\n", 1);
}; //WriteToLog
void pb::Utils::Output::WritetoTimedLog(std::string macro, std::string meso, std::string micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  macro = (macro.length() > 64) ? "String Length > 64 Chars" : macro;
  meso = (meso.length() > 64) ? "String Length > 64 Chars" : meso;
  micro = (micro.length() > 64) ? "String Length > 64 Chars" : micro;

  if (macro.length() < 64) {
    for (FINT16 i = 0; (64 - macro.length()); ++i) {
      macro.append(" ");
    }; //for FINT
  }; //if macro

  if (meso.length() < 64) {
    for (FINT16 i = 0; (64 - meso.length()); ++i) {
      meso.append(" ");
    }; //for FINT
  }; //if macro

  if (micro.length() < 64) {
    for (FINT16 i = 0; (64 - micro.length()); ++i) {
      micro.append(" ");
    }; //for FINT
  }; //if macro

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  std::string rtrnStr = ctime(&rtrnTime);
  rtrnStr.pop_back();
  rtrnStr += " | ";

  rtrnStr += std::move(macro);
  rtrnStr += " | ";
  rtrnStr += std::move(meso);
  rtrnStr += " | ";
  rtrnStr += std::move(micro);

  cout->write(rtrnStr.c_str(), rtrnStr.length());
  logfile->write(rtrnStr.c_str(), rtrnStr.length());

  cout->write("\n", 1);
  logfile->write("\n", 1);
}; //WriteToLog

void pb::Utils::Output::ThrowError(const char* str) noexcept {
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  cout->write(str, strlen(str));
  logfile->write(str, strlen(str));

  throw std::runtime_error(str);
}; //WritetoLog
void pb::Utils::Output::ThrowError(const char* macro, const char* micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  char rtrnStr[0xFFF]; //4096

  char buffer[0x40];
  strcpy(buffer, macro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, micro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  throw std::runtime_error(rtrnStr);
}; //WritetoTimedLog
void pb::Utils::Output::ThrowError(const char* macro, const char* meso, const char* micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  char rtrnStr[0xFFF];

  char buffer[0x40];
  strcpy(buffer, macro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, meso);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, micro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  throw std::runtime_error(rtrnStr);
}; //WritetoTimedLog
void pb::Utils::Output::ThrowError(const std::string str) noexcept {
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  cout->write(str.c_str(), str.length());
  logfile->write(str.c_str(), str.length());

  throw std::runtime_error(str);
}; //WritetoLog
void pb::Utils::Output::ThrowError(std::string macro, std::string micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  macro = (macro.length() > 64) ? "String Length > 64 Chars" : macro;
  micro = (micro.length() > 64) ? "String Length > 64 Chars" : micro;

  if (macro.length() < 64) {
    for (FINT16 i = 0; (64 - macro.length()); ++i) {
      macro.append(" ");
    }; //for FINT
  }; //if macro

  if (micro.length() < 64) {
    for (FINT16 i = 0; (64 - micro.length()); ++i) {
      micro.append(" ");
    }; //for FINT
  }; //if micro

  auto rtrnStr = std::move(macro);
  rtrnStr += " | ";
  rtrnStr += std::move(micro);

  throw std::runtime_error(rtrnStr);
}; //WriteToLog
void pb::Utils::Output::ThrowError(std::string macro, std::string meso, std::string micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  macro = (macro.length() > 64) ? "String Length > 64 Chars" : macro;
  meso = (meso.length() > 64) ? "String Length > 64 Chars" : meso;
  micro = (micro.length() > 64) ? "String Length > 64 Chars" : micro;

  if (macro.length() < 64) {
    for (FINT16 i = 0; (64 - macro.length()); ++i) {
      macro.append(" ");
    }; //for FINT
  }; //if macro

  if (meso.length() < 64) {
    for (FINT16 i = 0; (64 - meso.length()); ++i) {
      meso.append(" ");
    }; //for FINT
  }; //if macro

  if (micro.length() < 64) {
    for (FINT16 i = 0; (64 - micro.length()); ++i) {
      micro.append(" ");
    }; //for FINT
  }; //if macro

  auto rtrnStr = std::move(macro);
  rtrnStr += " | ";
  rtrnStr += std::move(meso);
  rtrnStr += " | ";
  rtrnStr += std::move(micro);

  throw std::runtime_error(rtrnStr);
}; //WriteToLog

void pb::Utils::Output::ThrowTimedError(const char* str) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  char rtrnStr[0xFFF]; //4096

  auto timeStr = ctime(&rtrnTime);

  timeStr[strlen(timeStr) - 1] = '\0';
  strcpy(rtrnStr, timeStr);
  strcat(rtrnStr, " | ");
  strcat(rtrnStr, str);

  throw std::runtime_error(rtrnStr);
}; //WritetoTimedLog
void pb::Utils::Output::ThrowTimedError(const char* macro, const char* micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  char rtrnStr[0xFFF]; //4096

  auto timeStr = ctime(&rtrnTime);

  timeStr[strlen(timeStr) - 1] = '\0';
  strcpy(rtrnStr, timeStr);

  char buffer[0x40];
  strcpy(buffer, macro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, micro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  throw std::runtime_error(rtrnStr);
}; //WritetoTimedLog
void pb::Utils::Output::ThrowTimedError(const char* macro, const char* meso, const char* micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  char rtrnStr[0xFFF]; //4096

  auto timeStr = ctime(&rtrnTime);

  timeStr[strlen(timeStr) - 1] = '\0';
  strcpy(rtrnStr, timeStr);

  char buffer[0x40];
  strcpy(buffer, macro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, meso);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  memset(buffer, 0, 0x40);
  strcpy(buffer, micro);

  strcat(rtrnStr, " | ");
  strcat(rtrnStr, buffer);

  throw std::runtime_error(rtrnStr);
}; //WritetoTimedLog
void pb::Utils::Output::ThrowTimedError(std::string str) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  std::string rtrnStr = ctime(&rtrnTime);
  rtrnStr.pop_back();
  rtrnStr += " | ";
  rtrnStr += str;

  throw std::runtime_error(rtrnStr);
}; //WriteToTimedLog
void pb::Utils::Output::ThrowTimedError(std::string macro, std::string micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  macro = (macro.length() > 64) ? "String Length > 64 Chars" : macro;
  micro = (micro.length() > 64) ? "String Length > 64 Chars" : micro;

  if (macro.length() < 64) {
    for (FINT16 i = 0; (64 - macro.length()); ++i) {
      macro.append(" ");
    }; //for FINT
  }; //if macro

  if (micro.length() < 64) {
    for (FINT16 i = 0; (64 - micro.length()); ++i) {
      micro.append(" ");
    }; //for FINT
  }; //if macro

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  std::string rtrnStr = ctime(&rtrnTime);
  rtrnStr.pop_back();
  rtrnStr += " | ";

  rtrnStr += std::move(macro);
  rtrnStr += " | ";
  rtrnStr += std::move(micro);

  throw std::runtime_error(rtrnStr);
}; //ThrowTimedError
void pb::Utils::Output::ThrowTimedError(std::string macro, std::string meso, std::string micro) noexcept {
  using namespace std::chrono;

  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  macro = (macro.length() > 64) ? "String Length > 64 Chars" : macro;
  meso = (meso.length() > 64) ? "String Length > 64 Chars" : meso;
  micro = (micro.length() > 64) ? "String Length > 64 Chars" : micro;

  if (macro.length() < 64) {
    for (FINT16 i = 0; (64 - macro.length()); ++i) {
      macro.append(" ");
    }; //for FINT
  }; //if macro

  if (meso.length() < 64) {
    for (FINT16 i = 0; (64 - meso.length()); ++i) {
      meso.append(" ");
    }; //for FINT
  }; //if macro

  if (micro.length() < 64) {
    for (FINT16 i = 0; (64 - micro.length()); ++i) {
      micro.append(" ");
    }; //for FINT
  }; //if macro

  auto time = system_clock::now();
  auto rtrnTime = system_clock::to_time_t(time);
  std::string rtrnStr = ctime(&rtrnTime);
  rtrnStr.pop_back();
  rtrnStr += " | ";

  rtrnStr += std::move(macro);
  rtrnStr += " | ";
  rtrnStr += std::move(meso);
  rtrnStr += " | ";
  rtrnStr += std::move(micro);

  throw std::runtime_error(rtrnStr);
}; //ThrowTimedError


#undef RADIAN 0x00
#undef DEGREE 0x01
bool CSVTypeCheck(csv::CSVField csvValue, int i) {
  inline constexpr int null = 0;
  inline constexpr int integer = 1;
  inline constexpr int string = 2;
  inline constexpr int floating = 3;
  switch (i) {
  case null: 
    if (csvValue.is_null()) {
      return true;
    } //CSVValue is Null
    return false;
  case integer:
    if (csvValue.is_int()) {
      return true;
    } //CSVValue is int
    return false;
  case string:
    if (csvValue.is_str()) {
      return true;
    } //CSVValue is str
    return false;
  case floating:
    if (csvValue.is_float()) {
      return true;
    } //CSVValue is float
    return false;
  }; //Switch
}; //CSVTypeCheck

//Finish This
std::array<float, 3> CSVStringFormat(const char* c, int i) {
  inline constexpr int WorldPos = 0;
  inline constexpr int ViewMat = 1;
  inline constexpr int FOVUnit = 2;
  inline constexpr int charSizeLimit = 5;
  std::array<float, 3> rtrnArray;
  
  switch (i) {
  WorldPos:
  ViewMat: {
      int j = 2, charIndex = charSizeLimit;
      for (size_t i = strlen(c); strlen(c) > i; --i, --charIndex) {
        char charArray[charSizeLimit];
        
        float returnD;
        if (c[i] == ',') { 
          sscanf(charArray, "%d", &returnD); 
          rtrnArray[j] = returnD; 
          --j; charIndex = charSizeLimit;
          continue; 
        } //if (c[i] == ',')
        if (c[i] == ' ') { continue; }

        if (charIndex >= 0) {
          charArray[charIndex] = c[i];
        } // if (charSizeLimit < charIndex)
        else {
          InternalReport(
            "Parsing CSV",
            "Uploading Camera",
            "Failure: Improper Input for CharSize"
          ); //InternalReport
        }; //if charIndex > 0
      }; //For (size_t i = 0;)
    }; //Tri-Int Parse
    return rtrnArray;

  FOVUnit:
    if (c == "RADIAN") {
      rtrnArray[0] = 0x00;
    } //if (c == RADIAN)
    else if (c == "DEGREE") {
      rtrnArray[0] = 0x01;
    }; //Else IF DEGREE
    return rtrnArray;
  }; //Switch
}; //CSVStirngFormat

#define RADIAN 0x00
#define DEGREE 0x01

pb::Utils::Input::Texture* pb::Utils::Input::TextureFromFile(const char* filename) {
  auto t = new Texture();
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
  FIBITMAP* dib = 0;

  char buffer[256];
  strcpy(buffer, "Gathering Texture ( ");
  strcat(buffer, filename);
  strcat(buffer, " )");

  fif = FreeImage_GetFileType(filename, 0);
  if (fif == FIF_UNKNOWN)
    fif = FreeImage_GetFIFFromFilename(filename);
  if (fif == FIF_UNKNOWN)
    InternalReport(buffer, "Checking Filetype", "Unknown Filetype");

  if (FreeImage_FIFSupportsReading(fif))
    dib = FreeImage_Load(fif, filename);
  if (!dib)
    InternalReport(buffer, "Checking Filetype Support", "No Filetype Support Found");

  //FreeImage_ConvertTo32Bits(dib);

  t->bits = FreeImage_GetBits(dib);
  t->width = FreeImage_GetWidth(dib);
  t->height = FreeImage_GetHeight(dib);
  if ((t->bits == 0) || (t->width == 0) || (t->height == 0))
    InternalReport(buffer, "Checking Upload", "Upload Failed");

  //FreeImage_Unload(dib);

  return t;
}; //TextureFromFile
pb::Feature::Scene* pb::Utils::Input::SceneFromFiles(std::vector<const char*> filenames) {

}; //SceneFromFiles
pb::Feature::Camera* pb::Utils::Input::CamFromFile(const char* filename, const char* name) {
  csv::CSVFormat format;
  format.trim({ ' ', '\t' });
  
  csv::CSVReader csvRows = csv::CSVReader(filename, format);
  for (auto& rows : csvRows) {
    if (CSVTypeCheck(rows["Camera"], 0) 
      && CSVTypeCheck(rows["Camera"], 2));

    if (rows["Camera"] == std::string(name)) {
      auto newCam = new pb::Feature::Camera();
      
      int i = 0;
      for (auto& fields : rows) {
        switch (i) {
        case 0: //name
          if (!CSVTypeCheck(fields, 2)) {
            InternalReport(
              "Reading Files",
              "Uploading Camera",
              "Failure, Incompatible Format: Name"
            ); //InternalReport
          }; //CSVTypeCheck

          newCam->SetName(name);
          break;
        case 1: //TransitiveLayer
          if (!CSVTypeCheck(fields, 1)) {
            InternalReport(
              "Reading Files",
              "Uploading Camera",
              "Failure, Incompatible Format: Transitive Layer"
            ); //InternalReport
          }; //CSVTypeCheck

          //newCam->SetName(name); 
          break;
        case 2: //WorldPos
          if (!CSVTypeCheck(fields, 2)) {
            InternalReport(
              "Reading Files",
              "Uploading Camera",
              "Failure, Incompatible Format: WoldPos"
            ); //InternalReport
          }; //CSVTypeCheck
          
          auto array = CSVStringFormat(fields.get().c_str(), 0);

          newCam->SetWorldPos(array);
          break;
        case 3: //ViewDir
          if (!CSVTypeCheck(rows["Camera"], 2)) {
            InternalReport(
              "Reading Files",
              "Uploading Camera",
              "Failure, Incompatible Format: ViewDir"
            ); //InternalReport
          }; //CSVTypeCheck

          auto array = CSVStringFormat(fields.get().c_str(), 1);

          newCam->SetViewDirection(array);
          break;
        case 4: //FOV
          if (!CSVTypeCheck(fields, 1)
            && !CSVTypeCheck(fields, 3)) {
            InternalReport(
              "Reading Files",
              "Uploading Camera",
              "Failure, Incompatible Format: FOV"
            ); //InternalReport
          }; //CSVTypeCheck

          newCam->SetFOV(std::stod(fields.get()));
          break;
        case 5: //FOVUnit
          if (!CSVTypeCheck(fields, 2)) {
            InternalReport(
              "Reading Files",
              "Uploading Camera",
              "Failure, Incompatible Format: FOVUnit"
            ); //InternalReport
          }; //CSVTypeCheck

          auto array = CSVStringFormat(fields.get().c_str(), 2);

          newCam->SetFOVUnit(array[0]);
          break;
        case 6: //FarClip
          if (!CSVTypeCheck(fields, 3)) {
            InternalReport(
              "Reading Files",
              "Uploading Camera",
              "Failure, Incompatible Format: FarClip"
            ); //InternalReport
          }; //CSVTypeCheck

          newCam->SetFarClip(std::stoi(fields.get()));
          break;
        case 7: //NearClip
          if (!CSVTypeCheck(fields, 1)
            && !CSVTypeCheck(fields, 3)) {
            InternalReport(
              "Reading Files",
              "Uploading Camera",
              "Failure, Incompatible Format: NearClip"
            ); //InternalReport
          }; //CSVTypeCheck
          
          newCam->SetNearClip(std::stoi(fields.get()));
          break;
        }; //Field Switch
        ++i;
      }; //for auto& fields 
      return newCam;
    }; // Checking Cam Value
  } //csvRowReader
  InternalReport(
    "Reading Files",
    "Uploading Camera",
    "Failure, Incompatible Format: Camera"
  ); //InternalReport
}; //CamFromFile


bool pb::Config::Utils::IsLogTimed() noexcept {
  return Timed;
}; //GetLogBuffer


std::ostream* pb::Config::Utils::GetLogBuffer() noexcept{
  return LogBuffer;
}; //GetLogBuffer


INT pb::Config::Utils::GetLogSegments() noexcept {
  return Segments;
}; //GetLogBuffer


void pb::Config::Utils::SetLogDelimiter(std::string delimeter) noexcept {
  Delimeter = delimeter;
}; //GetLogBuffer

void pb::Config::Utils::SetLogTimed(bool timed) noexcept {
  Timed = timed;
}; //GetLogBuffer


void pb::Config::Utils::SetLogBuffer(std::ostream* strm) noexcept {
  LogBuffer = strm;
}; //SetLogBuffer


//Internal 
void InternalLog(const char* macro, const char* meso, const char* micro) noexcept {
  auto utilConf = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils();

  switch (utilConf->GetLogSegments()) {
  case 1:
    if (utilConf->IsLogTimed()) {
      pb::Utils::Output::WritetoTimedLog(micro);
    }
    else {
      pb::Utils::Output::WritetoLog(micro);
    }
    break;
  case 2:
    if (utilConf->IsLogTimed()) {
      pb::Utils::Output::WritetoTimedLog(macro, micro);
    }
    else {
      pb::Utils::Output::WritetoLog(macro, micro);
    }
    break;
  case 3:
    if (utilConf->IsLogTimed()) {
      pb::Utils::Output::WritetoTimedLog(macro, meso, micro);
    }
    else {
      pb::Utils::Output::WritetoLog(macro, meso, micro);
    }
    break;
  }
}; //InternalLog

void InternalReport(const char* macro, const char* meso, const char* micro) noexcept {
  auto utilConf = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils();

  switch (utilConf->GetLogSegments()) {
  case 1:
    if (utilConf->IsLogTimed()) {
      pb::Utils::Output::ThrowTimedError(micro);
    }
    else {
      pb::Utils::Output::ThrowError(micro);
    }
    break;
  case 2:
    if (utilConf->IsLogTimed()) {
      pb::Utils::Output::ThrowTimedError(macro, micro);
    }
    else {
      pb::Utils::Output::ThrowError(macro, micro);
    }
    break;
  case 3:
    if (utilConf->IsLogTimed()) {
      pb::Utils::Output::ThrowTimedError(macro, meso, micro);
    }
    else {
      pb::Utils::Output::ThrowError(macro, meso, micro);
    }
    break;
  }
}; //InternalLog