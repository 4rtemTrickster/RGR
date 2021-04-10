#pragma once

#include "../Help.h"

#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


class Log
{
public:

    static void Init();

    static std::shared_ptr<spdlog::logger>& GetLogger() { return m_Logger;}

private:
   static std::shared_ptr<spdlog::logger> m_Logger;
};


//Logging macros
#define LOG_TRACE(...)  Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)   Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)   Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)  Log::GetLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)  Log::GetLogger()->fatal(__VA_ARGS__)