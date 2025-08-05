#include "Scene.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Core/Entity.hpp"
#include "Core/AllComponents.hpp"
#include "Rendering/ResourceManager.hpp"


namespace game
{
Scene1::Scene1(Game &game) noexcept
  : Scene{game}
{
  // Level loading
  Entity &player = game_.CreateEntity("player");
  TransformComponent &player_transform = player.AddComponent<TransformComponent>();
  player_transform.SetScale(Vector2{10, 10});

  SpriteComponent &player_sprite = player.AddComponent<SpriteComponent>();
  AnimatedSpriteData *player_sprite_data = new AnimatedSpriteData();
  player_sprite_data->SetShader(&game_.GetResourceManager().GetShader(ShaderType::kAnimatedSprite));
  player_sprite_data->SetTexture(&game_.GetResourceManager().GetTexture(TextureType::kPlayer));
  player_sprite_data->SetAtlasStep(0.5f);
  player_sprite.SetType(SpriteType::kAnimated);
  player_sprite.SetData(player_sprite_data);
  player_sprite.SetLayer(-1);

  b2BodyDef body_definition = b2DefaultBodyDef();
  body_definition.type = b2_dynamicBody;
  RigidbodyComponent &player_rigidbody = player.AddComponent<RigidbodyComponent>(body_definition);
  
  b2ShapeDef shape_definition = b2DefaultShapeDef();
  shape_definition.material.friction = 0.0f;
  RectangleColliderComponent &player_collider = player.AddComponent<RectangleColliderComponent>(shape_definition);

  PlayerComponent &player_component = player.AddComponent<PlayerComponent>();


  Entity &box = game_.CreateEntity("floor");
  TransformComponent &box_transform = box.AddComponent<TransformComponent>();
  box_transform.SetPosition(Vector2{0, -50});
  box_transform.SetScale(Vector2{200, 10});

  SpriteComponent &box_sprite = box.AddComponent<SpriteComponent>();
  TexturedSpriteData *box_sprite_data = new TexturedSpriteData();
  box_sprite_data->SetShader(&game_.GetResourceManager().GetShader(ShaderType::kTexturedSprite));
  box_sprite_data->SetTexture(&game_.GetResourceManager().GetTexture(TextureType::kNoTexture64));
  box_sprite.SetType(SpriteType::kTextured);
  box_sprite.SetData(box_sprite_data);

  RectangleColliderComponent &box_collider = box.AddComponent<RectangleColliderComponent>();
}
}

void game::Scene1::Update() noexcept
{

}
