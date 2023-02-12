#pragma once
#include <PBLCPB.h>

void InternalLog(const char* macro, const char* meso, const char* micro);
void InternalReport(const char* macro, const char* meso, const char* micro);

class PRIVATEPB {
  struct Config {
    Config(); 
    ~Config(); 
    pb::Config::Utils* GetUtils();
    pb::Config::Render* GetRender();
    bool GetConfirmed();
    bool GetWrotetoUtil();
    bool GetWrotetoRender();
    std::vector<const char*> GetErrBuff();

    void SetUtils(pb::Config::Utils* u);
    void SetRender(pb::Config::Render* r);
    void SetConfirmed(bool b);
    void SetWrotetoUtil(bool b);
    void SetWrotetoRender(bool b);
    void ExtndErrBuff(const char* str);
  }; //Config

  struct Utils { 
    std::ofstream* logFile;
    pb::Config::Utils* utilConf;
    
    void InternalLog(const char* macro, const char* meso, const char* micro);
    void InternalReport(const char* macro, const char* meso, const char* micro);

    Utils(pb::Config::Utils* U); 
  ~Utils(); };
  
  struct Client { 
    Config* Conf;
    Utils* Utils;

    void SetConfirmed(bool b);
    bool GetConfirmed(bool b);

    Client(); 
    ~Client(); };
  
  struct GFX { GFX(); ~GFX(); };
  struct GLFW; //Defined in Vulkan/OpenGL
  struct Vulkan { Vulkan(pb::Config::Render* R); ~Vulkan(); };
  struct DirectX { DirectX(pb::Config::Render* R); ~DirectX(); };
  struct OpenGL { OpenGL(pb::Config::Render* R); ~OpenGL(); };

  struct ClientVector {
    ClientVector();
    void NewClient();
  
    PRIVATEPB::Config* GetCurrentConfig();
    PRIVATEPB::Config* GetLatestConfig();

    PRIVATEPB::Utils* NewUtils();
    PRIVATEPB::Utils* GetClientVector();

    PRIVATEPB::Client* GetCurrentClient();
  }; //ClientVector

  static ClientVector* Client_ptr;

  PRIVATEPB() {};
  ~PRIVATEPB() {
    delete Client_ptr;
  }; //PrivatePB

  //InClass
  friend struct Config;
  friend struct Client;
  friend struct Utils;
  friend struct GFX;
  friend struct ClientVector;

  //Pb::Class
  friend struct pb::Utils::Output;
  friend struct pb::Utils::Input;

  //pb::Functions
  friend void pb::Config::AddConfig(pb::Config::Render* R);
  friend void pb::Config::AddConfig(pb::Config::Utils* U);

  friend void pb::Config::ConfirmConfigs();

  friend void pb::Client::ConfirmClients();
  friend void pb::RunRender();

  //PRVTPB
  friend void InternalLog(const char* A, const char* B, const char* C);
  friend void InternalReport(const char* A, const char* B, const char* C);

}; //PRIVATEPB