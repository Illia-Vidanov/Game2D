#ifndef GAME_DATA_STRUCTURES_HPP
#define GAME_DATA_STRUCTURES_HPP

#include "Utils/MathConstants.hpp"

namespace game
{
struct Vertex
{
  DefFloatType position_x = 0;
  DefFloatType position_y = 0;
  DefFloatType texture_position_x = 0;
  DefFloatType texture_position_y = 0;
};

using Index = uint32_t;
} // game

#endif