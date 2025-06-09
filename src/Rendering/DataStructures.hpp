#ifndef GAME_DATA_STRUCTURES_HPP
#define GAME_DATA_STRUCTURES_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"


namespace game
{
struct Vertex
{
  DefaultFloatType position_x = 0;
  DefaultFloatType position_y = 0;
};

using Index = uint32_t;
} // game

#endif // GAME_DATA_STRUCTURES_HPP