#ifndef FILE_FILE_READER_HPP
#define FILE_FILE_READER_HPP

#include "Setup.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <stdint.h>

#include "stb_image.h"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace tolik
{
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

class FileReader
{
public:
  FileReader() = delete;

  // Read txt file
  // Path can be either relative to executable or whole
  static auto Txt(const std::string &path) noexcept -> std::string;
  static auto Image(const std::string &path) noexcept -> ImageData;

private:
};
} // tolik

#endif // FILE_FILE_READER_HPP