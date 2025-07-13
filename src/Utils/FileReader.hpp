#ifndef FILE_FILE_READER_HPP
#define FILE_FILE_READER_HPP

#define USE_STB_IMAGE true

#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <stdint.h>

#if USE_STB_IMAGE
#include "stb_image.h"
#endif

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace tolik
{
#if USE_STB_IMAGE
struct ImageData
{
public:
  using DataType = uint8_t;

  uint8_t *data = nullptr;
  int width = 0;
  int height = 0;
  int channel_number = 0;

  void Free() noexcept;
};
#endif

class FileReader
{
public:
  FileReader() = delete;

  // Read txt file
  // Path can be either relative to executable or whole
  static auto Txt(const std::string &path) noexcept -> std::string;
  #if USE_STB_IMAGE
  static auto Image(const std::string &path) noexcept -> ImageData;
  #endif

private:
};
} // tolik

#endif // FILE_FILE_READER_HPP