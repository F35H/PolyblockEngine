#pragma once

#include <pbMacros.h>

namespace pbUtils {
  //LogFile
    static std::ofstream logfile;

    void LogtoFlush();

    void WritetoLog(const std::string str);
    void WritetoLog(const char* str);
    void WritetoTimedLog(const std::string str);
    void WritetoTimedLog(const char* str);
}