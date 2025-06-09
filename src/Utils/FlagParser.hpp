#ifndef GAME_FLAG_PARSER_HPP
#define GAME_FLAG_PARSER_HPP

#include "Setup.hpp"

#include "Utils/String.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"



namespace game
{
// Class to parse flags
//
// Values being supported in format of (flag)(delimetr)(value)
// Value can be with spaces if surrounded by quotes '"'
// Can use quotes inside quotes if back slash '\' is present before quotation mark
class Flags
{
public:
  using MapType = std::unordered_multimap<std::string, std::string>;
  using RangeType = std::pair<MapType::const_iterator, MapType::const_iterator>;
  using ConstArgvType = const char * const *;

  Flags(const int argc, ConstArgvType argv) noexcept : begin_(argv), end_(argv + argc) { Parse(); }

  // Parse flags with custom delimetr
  // default flag_delim = "="
  // It's delimetr between flag and value (Example: -flag=value   "=" is flag_delim here)
  Flags(const int argc, ConstArgvType argv, const std::string &flag_delim) noexcept : begin_(argv), end_(argv + argc), delim_(flag_delim) { GAME_ASSERT(!flag_delim.empty()); Parse(); }


  auto Count(const std::string &flag) const noexcept -> int { return flags_.count(flag); }
  auto Contains(const std::string &flag) const noexcept -> bool { return flags_.find(flag) != flags_.end(); }
  // If flag doesn't contain a value or don't exists empty string is returned
  // return string with flag value, if multiple are present any of them can be returned
  auto Get(const std::string &flag) const noexcept -> std::string { const auto it = flags_.find(flag); return (it == flags_.end() ? "" : it->second); }
  // If flag doesn't exist past the end element is returned for both iterators
  auto GetRange(const std::string &flag) const noexcept -> RangeType { return flags_.equal_range(flag); }
  
  auto begin() const noexcept -> MapType::const_iterator { return flags_.cbegin(); }
  auto end() const noexcept -> MapType::const_iterator { return flags_.cend(); }

  // return a string containing all flags separated by delim
  auto GetArgsAsString(const std::string &delim = " ") const noexcept -> std::string { return ConcatRawStringArray(begin_, end_, delim); }


  // Default delimetr between flag and value
  static constexpr inline const char *kDefDelim = "=";

private:
  const ConstArgvType begin_;
  const ConstArgvType end_;
  const std::string delim_ = kDefDelim;

  MapType flags_;

  static auto ParseFlag(ConstArgvType flag_begin, const ConstArgvType possible_flag_end, std::size_t *strings_used = nullptr, const std::string &delim = kDefDelim) noexcept -> MapType::value_type;
  void Parse() noexcept;
};
} // game

#endif // GAME_FLAG_PARSER_HPP