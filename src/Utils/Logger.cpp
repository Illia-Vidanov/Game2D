#include "Logger.hpp"

#include "Setup.hpp"

#include "Utils/MathConstants.hpp"


namespace game
{
Logger::Logger() noexcept
{
  ZoneScopedC(0xbaed00);

  OpenFile();
}

Logger::~Logger() noexcept
{
  ZoneScopedC(0xbaed00);

  file_.close();
}

void Logger::OpenFile() noexcept
{
  ZoneScopedC(0xbaed00);

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
  ZoneScopedNC("Log whole", 0xbaed00);

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
} // game