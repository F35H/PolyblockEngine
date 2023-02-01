#include <PRVTPB.h>

#define PRVTPB //PIMPLE


//CONFIG
struct PRIVATEPB::Config {
  Config() :
    Confirmed(0b00),
    WrotetoUtil(0b00),
    WrotetoRender(0b00),
    errBuffer(
      new std::vector < const char*>())
  {}; //Config

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

struct PRIVATEPB::Utils {
  std::ofstream* logFile;
  pb::Config::Utils* utilConf;

  Utils(pb::Config::Utils* U) {
    std::string fileName = "gameLog";
    fileName += ".txt";

    logFile = new std::ofstream(
      fileName, std::ios::ate | std::ios::out);

    int i = 0;
    auto cout = U->GetLogBuffer();
    const char* errMsg;

    utilConf = U;

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

  void InternalLog(const char* macro, const char* meso, const char* micro) noexcept {
    if (utilConf->IsTimed()) {
      pb::Utils::Output::WritetoTimedLog(macro, meso, micro);
    }
    else {
      pb::Utils::Output::WritetoLog(macro, meso, micro);
    }
  }; //InternalLog

  ~Utils() {
    delete logFile;
  }; //Utils

}; //Utils

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


void pb::Utils::Output::WritetoLog(const std::string str) noexcept {
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  cout->write(str.c_str(), str.length());
  logfile->write(str.c_str(), str.length());

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoLog


void pb::Utils::Output::WritetoLog(const char* str) noexcept {
  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;

  cout->write(str, strlen(str));
  logfile->write(str, strlen(str));

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoLog


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


//void pb::Utils::Output::WritetoTimedLog(std::string macro, std::string meso, std::string micro) noexcept {
//  using namespace std::chrono;
//  
//  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
//  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;
//
//  macro = (macro.length() > 64) ? "String Length > 64 Chars" : macro;
//  meso = (meso.length() > 64) ? "String Length > 64 Chars" : meso;
//  micro = (micro.length() > 64) ? "String Length > 64 Chars" : micro;
//
//  cout->write("\n", 2);
//  logfile->write("\n", 2);
//}; //WriteToLog
//
//
//void pb::Utils::Output::WritetoTimedLog(const char* macro, const char* meso, const char* micro) noexcept {
//  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
//  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;
//
//  char rtrn[256];
//
//  if (strlen(macro) > 64) {
//    strncpy(rtrn,
//      "String Length > 64 Chars",
//      strlen("String Length > 64 Chars"));
//  }
//  else {
//    strncpy(rtrn, macro, strlen(macro));
//  }; //if Macro
//
//  strncat(rtrn, " | ", 3);
//
//  if (strlen(meso) > 64) {
//    strncat(rtrn,
//      "String Length > 64 Chars",
//      strlen("String Length > 64 Chars"));
//  }
//  else {
//    strncat(rtrn, meso, strlen(meso));
//  }; //if Meso
//
//  strncat(rtrn, " | ", 3);
//
//  if (strlen(micro) > 64) {
//    strncat(rtrn, micro, strlen(micro));
//  }
//  else {
//    strncat(rtrn,
//      "String Length > 64 Chars",
//      strlen("String Length > 64 Chars"));
//  }; //if Micro
//
//  cout->write(rtrn, strlen(rtrn));
//  logfile->write(rtrn, strlen(rtrn));
//
//  cout->write("\n", 2);
//  logfile->write("\n", 2);
//}; //WritetoLog
//
//
//void pb::Utils::Output::WritetoLog(std::string macro, std::string meso, std::string micro) noexcept {
//  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
//  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;
//
//  macro = (macro.length() > 64) ? "String Length > 64 Chars" : macro;
//  meso = (meso.length() > 64) ? "String Length > 64 Chars" : meso;
//  micro = (micro.length() > 64) ? "String Length > 64 Chars" : micro;
//
//  auto rtrnStr = std::move(macro);
//  rtrnStr += " | ";
//  rtrnStr += std::move(meso);
//  rtrnStr += " | ";
//  rtrnStr += std::move(micro);
//
//  cout->write(rtrnStr.c_str(), rtrnStr.length());
//  logfile->write(rtrnStr.c_str(), rtrnStr.length());
//
//  cout->write("\n", 2);
//  logfile->write("\n", 2);
//}; //WriteToLog
//
//
//void pb::Utils::Output::WritetoLog(const char* macro, const char* meso, const char* micro) noexcept {
//  auto cout = PRIVATEPB::Client_ptr->GetCurrentConfig()->GetUtils()->GetLogBuffer();
//  auto logfile = PRIVATEPB::Client_ptr->GetCurrentClient()->Utils->logFile;
//
//  char rtrn[256]; 
//
//  if (strlen(macro) > 64) {
//    strncpy(rtrn, 
//      "String Length > 64 Chars", 
//      strlen("String Length > 64 Chars"));
//  }
//  else {
//    strncpy(rtrn, macro, strlen(macro));
//  }; //if Macro
//
//  strncat(rtrn, " | ", 3);
//
//  if (strlen(meso) > 64) {
//    strncat(rtrn,
//      "String Length > 64 Chars",
//      strlen("String Length > 64 Chars"));
//  }
//  else {
//    strncat(rtrn, meso, strlen(meso));
//  }; //if Meso
//
//  strncat(rtrn, " | ", 3);
//
//  if (strlen(micro) > 64) {
//    strncat(rtrn, micro, strlen(micro));
//  }
//  else {
//    strncat(rtrn,
//      "String Length > 64 Chars",
//      strlen("String Length > 64 Chars"));
//  }; //if Micro
//
//  cout->write(rtrn, strlen(rtrn));
//  logfile->write(rtrn, strlen(rtrn));
//
//  cout->write("\n", 2);
//  logfile->write("\n", 2);
//}; //WritetoLog


pb::Utils::Input::Texture* pb::Utils::Input::TextureFromFile(const char* filename) {
  auto t = new Texture();
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
  FIBITMAP* dib = 0;

  char buffer[256];
  strcpy(buffer, "Gathering Texture ( ");
  strcat(buffer, filename);
  strcat(buffer, " ) | Gathering Filetype | Filetype Ungatherable");

  fif = FreeImage_GetFileType(filename, 0);
  if (fif == FIF_UNKNOWN)
    fif = FreeImage_GetFIFFromFilename(filename);
  if (fif == FIF_UNKNOWN)
    throw std::runtime_error(buffer);

  strcpy(buffer, "Gathering Texture ( ");
  strcat(buffer, filename);
  strcat(buffer, " ) | Checking Filetype | Wrong Filetype");

  if (FreeImage_FIFSupportsReading(fif))
    dib = FreeImage_Load(fif, filename);
  if (!dib)
    throw std::runtime_error(buffer);

  strcpy(buffer, "Gathering Texture ( ");
  strcat(buffer, filename);
  strcat(buffer, " ) | Checking Upload | Upload Failed");

  FreeImage_ConvertTo32Bits(dib);

  t->bits = FreeImage_GetBits(dib);
  t->width = FreeImage_GetWidth(dib);
  t->height = FreeImage_GetHeight(dib);
  if ((t->bits == 0) || (t->width == 0) || (t->height == 0))
    throw std::runtime_error(buffer);

  FreeImage_Unload(dib);

  return t;
}; //TextureFromFile


bool pb::Config::Utils::IsTimed() noexcept {
  return Timed;
}; //GetLogBuffer


std::ostream* pb::Config::Utils::GetLogBuffer() noexcept{
  return logBuffer;
}; //GetLogBuffer


void pb::Config::Utils::SetTimed(bool timed) noexcept {
  Timed = timed;
}; //GetLogBuffer


void pb::Config::Utils::SetLogBuffer(std::ostream* strm) noexcept {
  logBuffer = strm;
}; //SetLogBuffer
