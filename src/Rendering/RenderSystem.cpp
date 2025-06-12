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
#include "UI/UI.hpp"


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
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


  sdl_gl_context_ = SDL_GL_CreateContext(game_.GetWindow().GetSDLWindow());
  GAME_ASSERT(sdl_gl_context_ != nullptr) << "Couldn't initialize opengl context: " << SDL_GetError();
  if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    GAME_ASSERT(false) << "Failed to load GL loader";

  if(SDL_GL_MakeCurrent(game_.GetWindow().GetSDLWindow(), sdl_gl_context_) < 0)
    GAME_ASSERT(false) << "Failed to associate GL context with current window";

  GAME_VLOG(1, LogType::kInfo) << "GL Vendor:           " << glGetString(GL_VENDOR)                   << '\n'
                               << "GL RenderSystem:     " << glGetString(GL_RENDERER)                 << '\n'
                               << "GL Version:          " << glGetString(GL_VERSION)                  << '\n'
                               << "GL Shading Lnaguage: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

  GAME_GL_CALL(glEnable(GL_SCISSOR_TEST));
  GAME_GL_CALL(glEnable(GL_BLEND));
  GAME_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  
  GAME_GL_CALL(glClearColor(0.224f, 0.298f, 0.302f, 1.0f));

  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kWindowResize, this, [](const Event &event, void *render_system) -> bool { return reinterpret_cast<RenderSystem*>(render_system)->WindowResizeEvent(event); });
}

RenderSystem::~RenderSystem() noexcept
{
  ZoneScopedC(0x07dbd4);

  SDL_GL_DeleteContext(sdl_gl_context_);
}

void RenderSystem::StartFrame() noexcept
{
  ZoneScopedC(0x07dbd4);

  GAME_GL_CALL(glDisable(GL_SCISSOR_TEST));
  GAME_GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GAME_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

  GAME_GL_CALL(glEnable(GL_SCISSOR_TEST));

  // Temporary used as background color
  GAME_GL_CALL(glClearColor(0.224f, 0.298f, 0.302f, 1.0f));
  GAME_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
}

void RenderSystem::EndFrame() noexcept
{
  ZoneScopedC(0x07dbd4);

  DrawSprites();
  DrawUI();

  SDL_GL_SwapWindow(game_.GetWindow().GetSDLWindow());
}
auto RenderSystem::WindowResizeEvent(const Event &event) const noexcept -> bool
{
  ZoneScopedC(0x07dbd4);

  GAME_GL_CALL(glViewport(
    game_.GetWindow().GetRenderPivotX(),
    game_.GetWindow().GetRenderPivotY(),
    game_.GetWindow().GetRenderWidth(),
    game_.GetWindow().GetRenderHeight()
  ));

  GAME_GL_CALL(glScissor(
    game_.GetWindow().GetRenderPivotX(),
    game_.GetWindow().GetRenderPivotY(),
    game_.GetWindow().GetRenderWidth(),
    game_.GetWindow().GetRenderHeight()
  ));

  return false;
}

void RenderSystem::DrawSprites() noexcept
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
    
    GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));
  }

  entt::view<entt::get_t<TexturedSpriteComponent>> textured_sprites = game_.GetRegistry().view<TexturedSpriteComponent>();
  for(entt::entity entity : textured_sprites)
  {
    const TexturedSpriteComponent &sprite = textured_sprites.get<TexturedSpriteComponent>(entity);
    
    GAME_GL_CALL(glActiveTexture(GL_TEXTURE0));
    sprite.GetTexture().Bind();

    sprite.GetShader().Use();
    Matrix3 transformation_matrix = game_.GetResourceManager().GetOrthographicProjection() * game_.GetCamera().GetViewMatrix().inverse() * game_.GetRegistry().get<TransformComponent>(entity).matrix();
    sprite.GetShader().SetUniformMatrix3("mvp", 1, false, transformation_matrix.data());

    GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));
  }
  
  entt::view<entt::get_t<AnimatedSpriteComponent>> animated_sprites = game_.GetRegistry().view<AnimatedSpriteComponent>();
  for(entt::entity entity : animated_sprites)
  {
    const AnimatedSpriteComponent &sprite = animated_sprites.get<AnimatedSpriteComponent>(entity);
    
    GAME_GL_CALL(glActiveTexture(GL_TEXTURE0));
    sprite.GetTexture().Bind();

    sprite.GetShader().Use();
    sprite.GetShader().SetUniform("sprite_index", sprite.GetSpriteIndex().x(), sprite.GetSpriteIndex().y());
    sprite.GetShader().SetUniform("atlas_step", sprite.GetAtlasStep());
    Matrix3 transformation_matrix = game_.GetResourceManager().GetOrthographicProjection() * game_.GetCamera().GetViewMatrix().inverse() * game_.GetRegistry().get<TransformComponent>(entity).matrix();
    sprite.GetShader().SetUniformMatrix3("mvp", 1, false, transformation_matrix.data());

    GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));
  }
}

void RenderSystem::DrawUI() noexcept
{
  ZoneScopedC(0x07dbd4);

  ImDrawData *draw_data = ImGui::GetDrawData();
  ImGui_ImplOpenGL3_RenderDrawData(draw_data);
}
} // game