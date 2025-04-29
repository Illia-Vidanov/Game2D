#ifndef GAME_TRANSFORM_COMPONENT_HPP
#define GAME_TRANSFORM_COMPONENT_HPP

#include "Setup.hpp"

#include "Utils/MathConstants.hpp"

namespace game
{
class TransformComponent : public Eigen::Transform<DefFloatType, 2, 2>
{
public:
  TransformComponent() : Eigen::Transform<DefFloatType, 2, 2>{Eigen::Transform<DefFloatType, 2, 2>::Identity()} {}
};
} // game

#endif // GAME_TRANSFORM_COMPONENT_HPP