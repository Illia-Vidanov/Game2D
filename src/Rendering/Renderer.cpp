#include "Renderer.hpp"

#include "Setup.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <tracy/Tracy.hpp>

#include "Core/Game.hpp"
#include "Core/Window.hpp"
#include "Utils/Logger.hpp"
#include "Rendering/Utils.hpp"
#include "Rendering/Sprite.hpp"



namespace game
{
Renderer::Renderer(Game &game) noexcept
  : game_(game)
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

  GL_CALL(glClearColor(0.224f, 0.298f, 0.302f, 1.0f));
}

void Renderer::PrintDebugInfo() const noexcept
{
  GAME_VLOG(1, LogType::kInfo) << "GL Vendor:           " << glGetString(GL_VENDOR)                   << '\n'
                               << "GL Renderer:         " << glGetString(GL_RENDERER)                 << '\n'
                               << "GL Version:          " << glGetString(GL_VERSION)                  << '\n'
                               << "GL Shading Lnaguage: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
}


void Renderer::Render(const Sprite &sprite) noexcept
{
  ZoneScopedC(0x07dbd4);

  GL_CALL(glActiveTexture(GL_TEXTURE0));
  sprite.GetTexture().Bind();
  sprite.GetShader().Use();
  sprite.GetShader().SetUniformMatrix4("model", 1, false, sprite.GetTransform().data());
  GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));
}

void Renderer::StartFrame() noexcept
{
  ZoneScopedC(0x07dbd4);

  GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::EndFrame() noexcept
{
  ZoneScopedC(0x07dbd4);

  game_.GetResourceManager().BindSpriteVAO();

  while(sprite_queue.size())
  {
    ZoneScopedNC("Render sprite", 0x07dbd4);

    Render(*sprite_queue.front());
    sprite_queue.pop();
  }

  SDL_GL_SwapWindow(game_.GetWindow().GetSDLWindow());
}

void Renderer::Exit() noexcept
{
  ZoneScopedC(0x07dbd4);

  SDL_GL_DeleteContext(context_);
}
} // game