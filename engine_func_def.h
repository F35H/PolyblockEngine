<<<<<<< HEAD

//FUNCTION POINTERS
//Engine_Collect
#define eInitCallBackFunctions (*initCallBackFunctions)(const std::map<std::string,SI> &genBakeVar)
#define eGameCallBackFunctions (*gameCallBackFunctions)(const std::map<std::string,SI> &genBakeVar)
#define eGenCallBackFunctions (*genCallBackFunctions)(const std::map<std::string,SI> &genBakeVar)
#define eDisplay  (*display) ()
//Bake
#define eGetScreenSize  (*getScreenSize)(PSI &psi,SI &si)
#define eBakeConfigure (*bakeConfigure)(std::map<std::string,SI> &genBakeVar, PSI &psi)
//Timers
#define eTimerUpdate  (*timerUpdate)(int value)
//GameIO
#define eErrorReturn  (*errorReturn)(const char *fmt, va_list ap)
#define eOutLog  (*outLog)(std::string throwString, std::ofstream &out)
#define eFailedOutLog  (*failedOutLog)()
//FUNCTIONS
//GameIO
#define ePrintLog2  printLog(std::string &throwString, time_t &time)
#define ePrintLog3  printLog(std::string &throwString,const std::string st, time_t &time)
=======

//FUNCTION POINTERS
//Engine_Collect
#define eInitCallBackFunctions (*initCallBackFunctions)(const std::map<std::string,SI> &genBakeVar)
#define eGameCallBackFunctions (*gameCallBackFunctions)(const std::map<std::string,SI> &genBakeVar)
#define eGenCallBackFunctions (*genCallBackFunctions)(const std::map<std::string,SI> &genBakeVar)
#define eDisplay  (*display) ()
//Bake
#define eGetScreenSize  (*getScreenSize)(PSI &psi,SI &si)
#define eBakeConfigure (*bakeConfigure)(std::map<std::string,SI> &genBakeVar, PSI &psi)
//Timers
#define eTimerUpdate  (*timerUpdate)(int value)
//GameIO
#define eErrorReturn  (*errorReturn)(const char *fmt, va_list ap)
#define eOutLog  (*outLog)(std::string throwString, std::ofstream &out)
#define eFailedOutLog  (*failedOutLog)()
//FUNCTIONS
//GameIO
#define ePrintLog2  printLog(std::string &throwString, time_t &time)
#define ePrintLog3  printLog(std::string &throwString,const std::string st, time_t &time)
>>>>>>> ddcba64 (MakeUpdate)
