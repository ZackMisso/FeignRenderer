#include <feign/log/logger.h>
#include <iostream>

void Logger::logWarning(std::string logg)
{
    std::cout << "Warning: " << logg << std::endl;
}

void Logger::logDebug(std::string logg)
{
    std::cout << "Debug: " << logg << std::endl;
}

void Logger::log(LogType type, std::string logg)
{
    if (type == LT_Warning) logWarning(logg);
    else if (type == LT_Debug) logDebug(logg);
}

void Logger::log(std::string logg)
{
    logDebug(logg);
}
