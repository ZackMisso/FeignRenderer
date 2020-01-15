/**
 * Author:    Zackary Misso
 * Version:   0.1.1
 *
 * Anyone has permission to use the following code as long as proper
 * acknowledgement is provided to the original author(s).
 **/

#pragma once

#include <string>

enum LogType
{
    LT_Warning,
    LT_Debug
};

class Logger
{
private:
    static void logWarning(std::string logg);
    static void logDebug(std::string logg);
public:
    static void log(LogType type, std::string logg);
    static void log(std::string logg);
};
