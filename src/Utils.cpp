#include <PRVTPB.h>


//CLIENT
struct PRIVATEPB::Client {
  std::vector<
    std::shared_ptr<Client>
  > ClientVector;

  std::shared_ptr<Config> Conf = std::make_shared<Config>();

  //CLIENT
  Client() {
    ClientVector = std::vector
      <std::shared_ptr<Client>>();
  }; //CLIENT

}; //Client



class Utils {
  Utils() {
    int i = 0;
    std::string uuid = sole::uuid1().str();

    auto cout = PB->Client_ptr->ClientVector;
    auto logfile = Config->Utils->LogFile;

    std::string fileName = "logs";
    fileName += "/log-";
    fileName += uuid;
    fileName += ".txt";

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

      }//Switch

      ++i;
    } while (!logfile->is_open());

  }; //UTILSCONSTRUCTOR

}; //Utils


void pb::Utils::Output::FlushtoLog() {
  auto cout = Config->Utils->LogBuffer;
  auto logfile = Config->Utils->LogFile;

  logfile->flush();
  cout->flush();
};

void pbUtils::Output::WritetoLog(const std::string str) {
  auto Config = GetConfig();

  auto cout = Config->Utils->LogBuffer;
  auto logfile = Config->Utils->LogFile;

  cout->write(str.c_str(), str.length());
  logfile->write(str.c_str(), str.length());

  cout->write("\n", 2);
  logfile->write("\n", 2);
};

void pbUtils::Output::WritetoLog(const char* str) {
  auto Config = GetConfig();

  auto cout = Config->Utils->LogBuffer;
  auto logfile = Config->Utils->LogFile;

  cout->write(str, strlen(str));
  logfile->write(str, strlen(str));

  cout->write("\n", 2);
  logfile->write("\n", 2);
};

void pbUtils::Output::WritetoTimedLog(const char* str) {
  auto Config = GetConfig();

  auto cout = Config->Utils->LogBuffer;
  auto logfile = Config->Utils->LogFile;

  cout->write(str, strlen(str));
  logfile->write(str, strlen(str));

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WritetoTimedLog


void pbUtils::Output::WritetoTimedLog(const std::string str) {
  auto Config = GetConfig();

  auto cout = Config->Utils->LogBuffer;
  auto logfile = Config->Utils->LogFile;

  cout->write(str.c_str(), str.length());
  logfile->write(str.c_str(), str.length());

  cout->write("\n", 2);
  logfile->write("\n", 2);
}; //WriteToTimedLog


 PRIVATEPB::Utils::Utils(std::vector<


 } //pbOutput 
