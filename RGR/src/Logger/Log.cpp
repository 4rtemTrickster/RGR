#include "Log.h"

std::shared_ptr<spdlog::logger> Log::m_Logger;

void Log::Init()
{
    spdlog::set_pattern("%^[%T][%n][%l]: %v%$");

    m_Logger = spdlog::stdout_color_mt("Log");
    m_Logger->set_level(spdlog::level::trace);
}
