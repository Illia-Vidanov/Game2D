#ifndef FILE_STRING_UTILS_HPP
#define FILE_STRING_UTILS_HPP

#include "Setup.hpp"

#include <string>
#include <stdint.h>
#include <algorithm>

#include "Utils/MathConstants.hpp"


namespace tolik
{
namespace detail
{
  template<typename CharT = char>
  using ConstRawStrArray = const CharT * const *;
}


template<typename CharT = char>
class StringBuilder
{
public:
  [[nodiscard]] auto GetString() noexcept -> std::string { return string_stream_.str(); }

  template<typename T>
	StringBuilder &operator<<(T &&t) noexcept;

private:
  std::basic_stringstream<CharT> string_stream_;
};

template<typename CharT>
template<typename T>
StringBuilder<CharT> &StringBuilder<CharT>::operator<<(T &&t) noexcept
{
  #ifdef TRACY_ENABLE
	ZoneScopedC(0xbaed00);
	#endif
  
  string_stream_ << std::forward<T>(t);
	return *this;
}


// return current path + '/' + str
template<typename CharT>
auto AddCurrentPathToString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>
{ return std::filesystem::current_path().generic_string<CharT>() + std::basic_string<CharT>(1, static_cast<CharT>('/')) + str; }

template<typename CharT>
auto RemoveCurrentPathFromString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>;

// Split string by delimetr in vector of strings
// default delim = " "
template<typename CharT>
auto SplitString(const std::basic_string<CharT> &str, const std::basic_string<CharT> &delim = " ") noexcept -> std::vector<std::basic_string<CharT>>;
// Split string by delimetr in vector of strings
// default delim = " "
template<typename CharT>
auto SplitString(const std::basic_string<CharT> &str, CharT delim) noexcept -> std::vector<std::basic_string<CharT>>
{ return SplitString(str, std::basic_string<CharT>(1, delim)); }

// Check if string starts with certain prefix
// Doesn't perform bounds checking so if prefix.size() > string.size() behaviour is undefined
template<typename CharT = char>
auto StringStartsWith(const std::basic_string<CharT> &prefix, const std::basic_string<CharT> &str) noexcept -> bool;

template<typename CharT>
auto SubstringCount(const std::basic_string<CharT> &str, const std::basic_string<CharT> &substr) noexcept -> std::size_t;

// Get charachter count of all strings in array
template<typename CharT>
constexpr auto RawStringArrayLength(detail::ConstRawStrArray<CharT> first, const detail::ConstRawStrArray<CharT> last) noexcept -> std::size_t; 

// Connect raw string array into single string with delimetr
// default delim = ""
// return string with elements from first to last separated by delim 
template<typename CharT>
auto ConcatRawStringArray(detail::ConstRawStrArray<CharT> first, const detail::ConstRawStrArray<CharT> last, const std::basic_string<CharT> &delim = "") noexcept -> std::basic_string<CharT>;

// Connect raw string array into single raw string with delimetr
// default delim = ""
// return raw string with elements from first to last separated by delim 
template<typename CharT>
auto ConcatRawStringArrayToRaw(detail::ConstRawStrArray<CharT> first, const detail::ConstRawStrArray<CharT> last, const CharT * const delim = "") noexcept -> Owner<CharT*>;

// Connect raw string array into single raw string with delimetr
// default delim = ""
// return raw string with elements from first to last separated by delim 
template<typename CharT>
auto ConcatRawStringArrayToRaw(std::initializer_list<const CharT*> raw_strings, const CharT * const delim = "") noexcept -> Owner<CharT*> { return ConcatRawStringArrayToRaw(&*raw_strings.begin(), &*raw_strings.end(), delim); }



template<typename CharT>
auto RemoveCurrentPathFromString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>
{
  #ifdef TRACY_ENABLE
	ZoneScopedC(0x3e2ed1);
	#endif

  const std::basic_string<CharT> &path = std::filesystem::current_path().generic_string<CharT>();
  if(str.size() == path.size())
    return std::basic_string<CharT>{""};
  if(!StringStartsWith(path, str))
    return str;
  return std::basic_string<CharT>(str).replace(0, path.size() + 1, ""); // + 1 for slash
}

template<typename CharT>
auto SplitString(const std::basic_string<CharT> &str, const std::basic_string<CharT> &delim) noexcept -> std::vector<std::basic_string<CharT>>
{
  GAME_ASSERT_STD(delim.size() != 0);

  #ifdef TRACY_ENABLE
	ZoneScopedC(0x3e2ed1);
	#endif

	std::vector<std::basic_string<CharT>> result(SubstringCount(str, delim) + 1);
	
	std::size_t index = 0;
	std::size_t fromPos = 0;
	for(std::size_t delimPos = str.find(delim); delimPos != std::basic_string<CharT>::npos; delimPos = str.find(delim, delimPos + 1))
	{
		result[index] = str.substr(fromPos, delimPos - fromPos);
		if(!result[index].empty())
			index++;
		fromPos = delimPos + delim.size();
	}
	
	result[index] = str.substr(fromPos);
	if(!result[index].empty())
		index++;

  result.resize(index);
  // In some cases we allocate much more memory than we actuall use, but usually this line can be commented out, depends on demands
  //result.shrink_to_fit();

	return result;
}

template<typename CharT>
auto StringStartsWith(const std::basic_string<CharT> &prefix, const std::basic_string<CharT> &str) noexcept -> bool
{
  GAME_ASSERT_STD(str.size() == 0);

  #ifdef TRACY_ENABLE
	ZoneScopedC(0x3e2ed1);
	#endif

    for(std::size_t i = 0; i < prefix.size(); i++)
    {
      if(str[i] != prefix[i])
        return false;
    }

    return true;
}

template<typename CharT>
auto SubstringCount(const std::basic_string<CharT> &str, const std::basic_string<CharT> &substr) noexcept -> std::size_t
{
  if(str.size() == 0 || substr.size() == 0)
    return 0;

  #ifdef TRACY_ENABLE
	ZoneScopedC(0x3e2ed1);
	#endif

	std::size_t count = 0;
	std::size_t substrPos = str.find(substr);
  while(substrPos != std::basic_string<CharT>::npos)
	{
		count++;
		substrPos = str.find(substr, substrPos + substr.size());
	}

	return count;
}

template<typename CharT>
constexpr auto RawStringArrayLength(detail::ConstRawStrArray<CharT> first, const detail::ConstRawStrArray<CharT> last) noexcept -> std::size_t
{
  GAME_ASSERT_STD(first != nullptr && last != nullptr && first < last);

  std::size_t result = 0;
	for(; first != last; first++)
		result += std::char_traits<CharT>::length(*first);

	return result;
}

template<typename CharT>
auto ConcatRawStringArray(detail::ConstRawStrArray<CharT> first, const detail::ConstRawStrArray<CharT> last, const std::basic_string<CharT> &delim) noexcept -> std::basic_string<CharT>
{
  GAME_ASSERT_STD(first != nullptr && last != nullptr && first < last);

  #ifdef TRACY_ENABLE
	ZoneScopedC(0x3e2ed1);
	#endif

	std::string result;
	result.reserve(RawStringArrayLength(first, last) + (last - first - 1) * delim.size());

  for(; first != last; first++)
  {
		result += *first;
		result += delim;
	}

	return result;
}

template<typename CharT>
auto ConcatRawStringArrayToRaw(detail::ConstRawStrArray<CharT> first, const detail::ConstRawStrArray<CharT> last, const CharT * const delim) noexcept -> Owner<CharT*>
{
  GAME_ASSERT_STD(first != nullptr && last != nullptr && first < last);

  #ifdef TRACY_ENABLE
	ZoneScopedC(0x3e2ed1);
	#endif

  const std::size_t delim_size = std::char_traits<CharT>::length(delim);
  char *result = new char[RawStringArrayLength(first, last) + (last - first - 1) * delim_size];
  const CharT *delim_end = delim + delim_size;

  for(; first != last; ++first)
  {
    const std::size_t string_length = std::char_traits<CharT>::length(*first);
    result = std::copy(*first, *first + string_length, result);
    result = std::copy(delim, delim_end, result);
  }

  return result;
}
} // tolik

#endif // FILE_STRING_UTILS_HPP