#include "Logger.hpp"

#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <stdint.h>

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "Utils/Enum.hpp"
#include "Utils/String.hpp"
#include "Utils/MathConstants.hpp"


namespace tolik
{
Logger::Logger() noexcept
{
  #ifdef TRACY_ENABLE
	ZoneScopedC(0xbaed00);
	#endif

  OpenFile();
}

Logger::~Logger() noexcept
{
  #ifdef TRACY_ENABLE
	ZoneScopedC(0xbaed00);
	#endif

  file_.close();
}

void Logger::OpenFile() noexcept
{
  #ifdef TRACY_ENABLE
	ZoneScopedC(0xbaed00);
	#endif

  std::filesystem::create_directories(std::filesystem::path{file_path_}.parent_path());
  file_.open(file_path_, kOpenMode);
  GAME_ASSERT(file_.good()) << "Couldn't creaate file: " << file_path_ << " when initialized LogStream: " << this;
}

LogStream::LogStream(const LogData &log_data) noexcept
  : fatal_(log_data.type == LogType::kFatal)
{
  Output(Logger::Get().GetFormatter()(log_data));
}

LogStream::~LogStream() noexcept
{
  #ifdef TRACY_ENABLE
	ZoneScopedNC("Log whole", 0xbaed00);
	#endif

  string_builder_ << '\n';

  Output(string_builder_.GetString());

  if(fatal_)
    std::abort();
}

void LogStream::Output(const std::string &string) noexcept
{
  Logger::Get().GetStream() << string;
  if(Logger::Get().GetFile().is_open())
  {
	  Logger::Get().GetFile() << string;
	  Logger::Get().GetFile().flush();
  }
}
} // tolik