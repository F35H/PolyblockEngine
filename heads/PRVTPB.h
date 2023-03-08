#pragma once
#include <PBLCPB.h>

void getBlock();
void getChunk();
void getBlock();

void InternalLog(const char* macro, const char* meso, const char* micro) noexcept;
void InternalReport(const char* macro, const char* meso, const char* micro) noexcept;

class PRIVATEPB {
  struct Config;
  struct Utils;
  struct Client;
  struct ClientVector;

  struct GFX { GFX(pb::Config::Render* R); ~GFX(); };
  struct Vulkan { Vulkan(pb::Config::Render* R); ~Vulkan(); };
  struct DirectX { DirectX(pb::Config::Render* R); ~DirectX(); };
  struct OpenGL { OpenGL(pb::Config::Render* R); ~OpenGL(); };

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
  friend void InternalLog(const char* A, const char* B, const char* C) noexcept;
  friend void InternalReport(const char* A, const char* B, const char* C) noexcept;

}; //PRIVATEPB