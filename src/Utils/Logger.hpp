#ifndef GAME_LOGGER_HPP
#define GAME_LOGGER_HPP

#include "Setup.hpp"

#include "Utils/Enum.hpp"
#include "Utils/String.hpp"
#include "Utils/MathConstants.hpp"

/* Todo:
*  Color
*/

#if defined(NDEBUG) && !defined(TRACY_ENABLE)
  #define GAME_DLOG(type) if constexpr (false) game::LogStream{LogData{__LINE__, __FILE__, LogType::kNone, std::chrono::system_clock::now(), #condition, static_cast<Logger::VerboseLevelType>{-1}}}
  #define GAME_VDLOG(level, type) GAME_DLOG(void)
  #define GAME_ASSERT(condition) GAME_DLOG(void)
  #define GAME_DLOG_IF(condition, type) GAME_DLOG(void)
  #define GAME_VDLOG_IF(condition, level, type) GAME_DLOG(void)
#else
  // Log message only in when NDEBUG is 0
  #define GAME_DLOG(type) GAME_LOG(type)
  // DLOG if verbose level is smaller than current Logger's verbose level
  #define GAME_VDLOG(level, type) GAME_VLOG(level, type)
  #define GAME_ASSERT(condition) if(GAME_IS_UNLIKELY(!(static_cast<bool>(condition)))) game::LogStream{game::LogData{__LINE__, __FILE__, game::LogType::kFatal, std::chrono::system_clock::now(), #condition, static_cast<game::Logger::VerboseLevelType>(-1)}}
  // DLOG if condition is true
  #define GAME_DLOG_IF(condition, type) GAME_LOG_IF(condition, type)
  // DLOG if verbose level is smaller than current Logger's verbose level and condition is true
  #define GAME_VDLOG_IF(condition, level, type) GAME_VLOG_IF(condition, level, type)
#endif

#define GAME_LOG(type) game::LogStream{game::LogData{__LINE__, __FILE__, type, std::chrono::system_clock::now(), "", static_cast<game::Logger::VerboseLevelType>(-1)}}
// LOG if verbose level is smaller than current Logger's verbose level
#define GAME_VLOG(level, type) if(level < game::Logger::Get().GetVerboseLevel()) game::LogStream{game::LogData{__LINE__, __FILE__, type, std::chrono::system_clock::now(), "", static_cast<game::Logger::VerboseLevelType>(level)}}
// LOG if condition is true
#define GAME_LOG_IF(condition, type) if(static_cast<bool>(condition)) game::LogStream{game::LogData{__LINE__, __FILE__, type, std::chrono::system_clock::now(), #condition, static_cast<game::Logger::VerboseLevelType>(-1)}}
// LOG if verbose level is smaller than current Logger's verbose level and condition is true
#define GAME_VLOG_IF(condition, level, type) if(static_cast<bool>(condition)) game::LogStream{game::LogData{__LINE__, __FILE__, type, std::chrono::system_clock::now(), #condition, static_cast<game::Logger::VerboseLevelType>(level)}}

namespace game
{
enum class LogType : uint8_t
{
  kNone, kFatal, kError, kWarning, kInfo
};

const std::string kLogTypeNameMap[] =
{
  "None", "Fatal", "Error", "Warning", "Info"
};


struct LogData
{
  using VerboseLevelType = int;

  int line;
  const char *file;
  LogType type;
  std::chrono::time_point<std::chrono::system_clock> time;
  const char *condition;
  VerboseLevelType verbose_level;
};


class Logger
{
public:
  Logger() noexcept;
  ~Logger() noexcept;

	using StreamCharT = char;
  using VerboseLevelType = LogData::VerboseLevelType;
  using FormatterType = std::function<std::string(const LogData &)>; // Wanted to use plain function pointer, but in getter "function can't return function"

  // Global instance of logger
  [[nodiscard]] static auto Get() -> Logger& { static Logger instance; return instance; }

  [[nodiscard]] constexpr auto GetFile() noexcept -> std::basic_ofstream<StreamCharT>& { return file_; }
  [[nodiscard]] constexpr auto GetFilePath() const noexcept -> const std::string& { return file_path_; }
  void SetFilePath(std::string file_path) noexcept { file_path_ = file_path; file_.close(); OpenFile(); }
  
  [[nodiscard]] constexpr auto GetVerboseLevel() const noexcept -> int { return verbose_level_; }
  constexpr void SetVerboseLevel(VerboseLevelType verbose_level) noexcept { verbose_level_ = verbose_level; }
  [[nodiscard]] constexpr auto GetStream() noexcept -> std::basic_ostream<StreamCharT>& { return stream_; }
  // Set buffer of underlying stream
  // Set to:
  // std::cerr.rdbuf() to use std::cerr
  // std::cout.rdbuf() to use std::cout
  // etc.
  void SetStream(std::basic_ostream<StreamCharT> &stream) noexcept { stream_.rdbuf(stream.rdbuf()); }

  auto GetFormatter() const noexcept -> const FormatterType& { return formatter_; }
  void SetFormatter(const FormatterType &formatter) { formatter_ = formatter; }
  

  [[nodiscard]] static auto GetLogTypeName(LogType type) noexcept -> std::string { return kLogTypeNameMap[ToUnderlying(type)]; }

private:
  void OpenFile() noexcept;  

  std::basic_ostream<StreamCharT> stream_{std::cout.rdbuf()};
  VerboseLevelType verbose_level_ = 0;
  std::string file_path_{"./logs/last_log.txt"};
	std::basic_ofstream<StreamCharT> file_;
  FormatterType formatter_ = [](const LogData &log_data) -> std::string { return (StringBuilder{} << GetLogTypeName(log_data.type) << "] ").GetString(); };

  static inline constexpr std::ios_base::openmode kOpenMode = std::ios_base::out | std::ios_base::trunc;
};


class LogStream
{
public:
  LogStream(const LogData &log_data) noexcept;
  ~LogStream() noexcept;

  template<typename T>
  auto operator<<(T &&t) noexcept -> LogStream& { string_builder_ << t; return *this; }

private:
  void Output(const std::string &string) noexcept;

  const bool fatal_ = false;
  StringBuilder<Logger::StreamCharT> string_builder_;
};
} // game

#endif // GAME_LOGGER_HPP