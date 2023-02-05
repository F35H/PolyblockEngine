#pragma once
#include <PBLCPB.h>

//Internal Log
void IL(const char* A, const char* B, const char* C) noexcept;
void IR(const char* A, const char* B, const char* C) noexcept;


class PRIVATEPB {
  struct Config;
  struct Client;
  struct Utils;
  struct GFX;

  struct Vulkan;

  struct ClientVector;

  static ClientVector* Client_ptr;

  static void runNewVulkan();
  static void runOldVulkan();
  void runDirectX();
  void runOpenGL();

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

  //pb::Functions
  friend void pb::Config::AddConfig(pb::Config::Render* R);
  friend void pb::Config::AddConfig(pb::Config::Utils* U);

  friend void pb::Config::ConfirmConfigs();

  friend void pb::Client::ConfirmClients();
  friend void pb::RunRender();

  //PRVTPB
  friend void IL(const char* A, const char* B, const char* C);
  friend void IR(const char* A, const char* B, const char* C);

}; //PRIVATEPB