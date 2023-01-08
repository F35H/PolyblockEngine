#pragma once
#include <PBLCPB.h>

//#ifdef  PRVTPB
class PRIVATEPB {
  struct Config;
  struct Client;
  struct Utils;
  struct ClientVector;

  static std::shared_ptr<ClientVector> Client_ptr;

  PRIVATEPB();
  ~PRIVATEPB() {};

  //InClass
  friend struct Config;
  friend struct Client;
  friend struct Utils;
  friend struct ClientVector;


  //Pb::Class
  friend struct pb::Utils::Output;


  //pb::Functions
  friend void pb::Config::AddConfig(pb::Config::Render* R);
  friend void pb::Config::AddConfig(pb::Config::Utils* U);

  friend void pb::Config::AddConfig(std::shared_ptr<pb::Config::Utils> U);
  friend void pb::Config::AddConfig(std::shared_ptr<pb::Config::Render> R);

  friend void pb::Config::CheckConfigs();
  friend void pb::Config::ConfirmConfigs();

  friend void pb::Client::ConfirmClients();
  friend void pb::RunRender();

}; //PRIVATEPB

//#endif