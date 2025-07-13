#include "FileReader.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <stdint.h>

#if USE_STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif
#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace tolik
{
std::string FileReader::Txt(const std::string &path) noexcept
{
  #ifdef TRACY_ENABLE
	ZoneScopedC(0xDD008C);
	#endif
  GAME_ASSERT(std::filesystem::exists(path)) << "File \'" << path << "\' doesn't exist!";

  std::ifstream file{path};
	static std::stringstream buffer;
  buffer.str("");
	buffer << file.rdbuf();
  return std::string{buffer.str()};
}

#if USE_STB_IMAGE
auto FileReader::Image(const std::string &path) noexcept -> ImageData
{
  #ifdef TRACY_ENABLE
	ZoneScopedC(0xDD008C);
	#endif
  GAME_ASSERT(std::filesystem::exists(path)) << "File \'" << path << "\' doesn't exist!";

  ImageData image_data;
  if(!(image_data.data = stbi_load(path.c_str(), &image_data.width, &image_data.height, &image_data.channel_number, 0)))
  {
    GAME_LOG(LogType::kError) << "Unable to load image \'" << path << '\'';
    image_data.data = nullptr;
    return image_data;
  }
  
  return image_data;
}

void ImageData::Free() noexcept
{
  stbi_image_free(data);
}
#endif
} // tolik