#include "RenderSystem.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Core/Window.hpp"
#include "Utils/Logger.hpp"
#include "Utils/Math.hpp"
#include "Rendering/Utils.hpp"
#include "Rendering/SpriteComponent.hpp"
#include "Physics/TransformComponent.hpp"


namespace game
{
RenderSystem::RenderSystem(Game &game) noexcept
  : game_{game}
  , sprites_{game_.GetRegistry().group<const SpriteComponent>(entt::get<TransformComponent>)}
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

  PrintDebugInfo();

  GL_CALL(glEnable(GL_SCISSOR_TEST));
  
  GL_CALL(glClearColor(0.224f, 0.298f, 0.302f, 1.0f));
}

void RenderSystem::PrintDebugInfo() const noexcept
{
  GAME_VLOG(1, LogType::kInfo) << "GL Vendor:           " << glGetString(GL_VENDOR)                   << '\n'
                               << "GL RenderSystem:         " << glGetString(GL_RENDERER)                 << '\n'
                               << "GL Version:          " << glGetString(GL_VERSION)                  << '\n'
                               << "GL Shading Lnaguage: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
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
  for(entt::entity entity : sprites_)
  {
    ZoneScopedNC("Render sprite", 0x07dbd4);

    GL_CALL(glActiveTexture(GL_TEXTURE0));
    const SpriteComponent &sprite = sprites_.get<SpriteComponent>(entity);
    sprite.GetTexture().Bind();
    sprite.GetShader().Use();
    sprite.GetShader().SetUniformMatrix4("model", 1, false, TransformTo4x4TransformationMatrix(sprites_.get<TransformComponent>(entity)).data());
    GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));
  }

  SDL_GL_SwapWindow(game_.GetWindow().GetSDLWindow());
}

void RenderSystem::Exit() noexcept
{
  ZoneScopedC(0x07dbd4);

  SDL_GL_DeleteContext(context_);
}
} // game