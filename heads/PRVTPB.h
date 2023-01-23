#pragma once
#include <PBLCPB.h>

class PRIVATEPB {
  struct Config;
  struct Client;
  struct Utils;
  struct GFX;

  struct ClientVector;

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


  //pb::Functions
  friend void pb::Config::AddConfig(pb::Config::Render* R);
  friend void pb::Config::AddConfig(pb::Config::Utils* U);

  friend void pb::Config::ConfirmConfigs();

  friend void pb::Client::ConfirmClients();
  friend void pb::RunRender();

}; //PRIVATEPB