#include "FileReader.hpp"

#include "Setup.hpp"

#include <string>
#include <fstream>
#include <sstream>
#if defined(GAME_WIN_OS)
#include <windows.h>
#endif

#include <tracy/Tracy.hpp>


namespace game
{
std::string FileReader::Txt(const std::string &path) noexcept
{
  ZoneScopedC(0xDD008C);

  /// Use platform specific, because it is usually much faster
  #if defined(GAME_WIN_OS)
    OFSTRUCT info;
		HANDLE f = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		std::string str(info.cBytes, '\0');
		ReadFile(f, str.data(), info.cBytes, NULL, NULL);
		CloseHandle(f);
    return str;
  #else
    std::ifstream file{path};
		static std::stringstream string;
    string.str("");
		string << file.rdbuf();
    return string.str();
  #endif
}
} // game