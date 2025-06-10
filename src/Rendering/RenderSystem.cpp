#include "RenderSystem.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Core/Window.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Utils/Math.hpp"
#include "Rendering/Utils.hpp"
#include "Rendering/SpriteComponents.hpp"
#include "Physics/TransformComponent.hpp"


namespace game
{
RenderSystem::RenderSystem(Game &game) noexcept
  : game_{game}
  , event_cleaner_{game_.GetEventSystem()}
{
  ZoneScopedC(0x07dbd4);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  context_ = SDL_GL_CreateContext(game_.GetWindow().GetSDLWindow());
  GAME_ASSERT(context_ != nullptr) << "Couldn't initialize opengl context: " << SDL_GetError();
  if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    GAME_ASSERT(false) << "Failed to load GL loader";

  if(SDL_GL_MakeCurrent(game_.GetWindow().GetSDLWindow(), context_) < 0)
    GAME_ASSERT(false) << "Failed to associate GL context with current window";

  GAME_VLOG(1, LogType::kInfo) << "GL Vendor:           " << glGetString(GL_VENDOR)                   << '\n'
                               << "GL RenderSystem:     " << glGetString(GL_RENDERER)                 << '\n'
                               << "GL Version:          " << glGetString(GL_VERSION)                  << '\n'
                               << "GL Shading Lnaguage: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

  GL_CALL(glEnable(GL_SCISSOR_TEST));
  GL_CALL(glEnable(GL_BLEND));
  GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  
  GL_CALL(glClearColor(0.224f, 0.298f, 0.302f, 1.0f));

  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kWindowResize, this, [](const Event &event, void *render_system) -> bool { return reinterpret_cast<RenderSystem*>(render_system)->RenderAreaResizeEvent(event); });
}

RenderSystem::~RenderSystem() noexcept
{
  ZoneScopedC(0x07dbd4);

  SDL_GL_DeleteContext(context_);
}

void RenderSystem::StartFrame() noexcept
{
  ZoneScopedC(0x07dbd4);

  GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
}

void RenderSystem::EndFrame() noexcept
{
  ZoneScopedC(0x07dbd4);

  game_.GetResourceManager().BindSpriteVAO();
  entt::view<entt::get_t<DefaultSpriteComponent>> default_sprites = game_.GetRegistry().view<DefaultSpriteComponent>();
  for(entt::entity entity : default_sprites)
  {
    static const Shader &kDefaultSpriteShader = game_.GetResourceManager().GetShader(ShaderType::kDefaultSprite);
    
    kDefaultSpriteShader.Use();
    Matrix3 transformation_matrix = game_.GetResourceManager().GetOrthographicProjection() * game_.GetCamera().GetViewMatrix().inverse() * game_.GetRegistry().get<TransformComponent>(entity).matrix();
    kDefaultSpriteShader.SetUniformMatrix3("mvp", 1, false, transformation_matrix.data());
    
    GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));
  }

  entt::view<entt::get_t<TexturedSpriteComponent>> textured_sprites = game_.GetRegistry().view<TexturedSpriteComponent>();
  for(entt::entity entity : textured_sprites)
  {
    const TexturedSpriteComponent &sprite = textured_sprites.get<TexturedSpriteComponent>(entity);
    
    GL_CALL(glActiveTexture(GL_TEXTURE0));
    sprite.GetTexture().Bind();

    sprite.GetShader().Use();
    Matrix3 transformation_matrix = game_.GetResourceManager().GetOrthographicProjection() * game_.GetCamera().GetViewMatrix().inverse() * game_.GetRegistry().get<TransformComponent>(entity).matrix();
    sprite.GetShader().SetUniformMatrix3("mvp", 1, false, transformation_matrix.data());

    GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));
  }
  
  entt::view<entt::get_t<AnimatedSpriteComponent>> animated_sprites = game_.GetRegistry().view<AnimatedSpriteComponent>();
  for(entt::entity entity : animated_sprites)
  {
    const AnimatedSpriteComponent &sprite = animated_sprites.get<AnimatedSpriteComponent>(entity);
    
    GL_CALL(glActiveTexture(GL_TEXTURE0));
    sprite.GetTexture().Bind();

    sprite.GetShader().Use();
    sprite.GetShader().SetUniform("sprite_index", sprite.GetSpriteIndex().x(), sprite.GetSpriteIndex().y());
    sprite.GetShader().SetUniform("atlas_step", sprite.GetAtlasStep());
    Matrix3 transformation_matrix = game_.GetResourceManager().GetOrthographicProjection() * game_.GetCamera().GetViewMatrix().inverse() * game_.GetRegistry().get<TransformComponent>(entity).matrix();
    sprite.GetShader().SetUniformMatrix3("mvp", 1, false, transformation_matrix.data());

    GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));
  }

  SDL_GL_SwapWindow(game_.GetWindow().GetSDLWindow());
}
auto RenderSystem::RenderAreaResizeEvent(const Event &event) const noexcept -> bool
{
  const int lowest_left_point_x = (event.GetNewResolutionX() - game_.GetWindow().GetRenderWidth()) / 2;
  const int lowest_left_point_y = (event.GetNewResolutionY() - game_.GetWindow().GetRenderHeight()) / 2;

  GL_CALL(glViewport(
    lowest_left_point_x,
    lowest_left_point_y,
    game_.GetWindow().GetRenderWidth(),
    game_.GetWindow().GetRenderHeight()
  ));

  //GL_CALL(glScissor(0, 0, game_.GetWindow().GetWidth(), game_.GetWindow().GetHeight()));

  // glClearColor are temporary before we figure out how to make background
  GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

  GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
  SDL_GL_SwapWindow(game_.GetWindow().GetSDLWindow());
  GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

  GL_CALL(glScissor(
    lowest_left_point_x,
    lowest_left_point_y,
    game_.GetWindow().GetRenderWidth(),
    game_.GetWindow().GetRenderHeight()
  ));

  GL_CALL(glClearColor(0.224f, 0.298f, 0.302f, 1.0f));

  return false;
}
} // game