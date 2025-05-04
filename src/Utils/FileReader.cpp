#include "FileReader.hpp"

#include "Setup.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>




namespace game
{
std::string FileReader::Txt(const std::string &path) noexcept
{
  ZoneScopedC(0xDD008C);
  GAME_ASSERT(std::filesystem::exists(path)) << "File \'" << path << "\' doesn't exist!";

  std::ifstream file{path};
	static std::stringstream buffer;
  buffer.str("");
	buffer << file.rdbuf();
  return std::string{buffer.str()};
}

auto FileReader::Image(const std::string &path) noexcept -> ImageData
{
  ZoneScopedC(0xDD008C);
  GAME_ASSERT(std::filesystem::exists(path)) << "File \'" << path << "\' doesn't exist!";

  ImageData image_data;
  if(!(image_data.data = stbi_load(path.c_str(), &image_data.width, &image_data.height, &image_data.channel_number, 0)))
  {
    GAME_LOG(LogType::kError) << "Wasn't able to load image \'" << path << '\'';
    image_data.data = nullptr;
    return image_data;
  }
  
  return image_data;
}

void ImageData::Free() noexcept
{
  stbi_image_free(data);
}
} // game