#pragma once
#include <PBLCPB.h>

//#ifdef  PRVTPB
class PRIVATEPB {
  struct Config;
  struct Utils;

  std::unique_ptr<Config> Config_ptr;
  std::unique_ptr<Utils> Utils_ptr;

public:
  struct Client;

  std::unique_ptr<Client> Client_ptr;

  PRIVATEPB() :
    Config_ptr(std::make_unique<Config>()),
    Client_ptr(std::make_unique<Client>()),
    Utils_ptr(std::make_unique<Utils>())
  {}; //PRIVATEPB

}; //PRIVATEPB

  static std::shared_ptr<PRIVATEPB> PB = std::make_shared<PRIVATEPB>();

//#endif