#include "RenderSystem.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Core/Window.hpp"
#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Utils/Math.hpp"
#include "Rendering/Utils.hpp"
#include "Rendering/SpriteComponents.hpp"
#include "Rendering/OutlineComponent.hpp"
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
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
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
  
  GAME_GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GAME_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

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

  // Clear even out of scope of scissor because of the use of ImGUI
  GAME_GL_CALL(glDisable(GL_SCISSOR_TEST));
  GAME_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
  GAME_GL_CALL(glEnable(GL_SCISSOR_TEST));
  
  // Temporary used as background color
  GAME_GL_CALL(glClearColor(0.224f, 0.298f, 0.302f, 1.0f));
  GAME_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
  GAME_GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
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

  game_.GetRegistry().sort<SpriteComponent>([](const SpriteComponent &lhs, const SpriteComponent &rhs) -> bool {
    return lhs.GetLayer() < rhs.GetLayer();
  });

  const Shader &kColoredSpriteShader = game_.GetResourceManager().GetShader(ShaderType::kColoredSprite);
  const Matrix3 view_projection_matrix = game_.GetResourceManager().GetOrthographicProjection() * game_.GetCamera().GetViewMatrix().inverse();

  entt::view<entt::get_t<SpriteComponent>> sprites = game_.GetRegistry().view<SpriteComponent>();
  for(entt::entity entity : sprites)
  {
    SpriteComponent &sprite = sprites.get<SpriteComponent>(entity);
    
    switch(sprite.GetType())
    {
    case SpriteType::kDefault:
    {
      static const Shader &kDefaultSpriteShader = game_.GetResourceManager().GetShader(ShaderType::kDefaultSprite);
      
      TransformComponent &transform = game_.GetRegistry().get<TransformComponent>(entity);
      OutlineComponent *outline = game_.GetRegistry().try_get<OutlineComponent>(entity);

      if(outline && outline->GetWidth() > 0.0f)
      {
        GAME_GL_CALL(glEnable(GL_STENCIL_TEST));
        GAME_GL_CALL(glClearStencil(0));
        GAME_GL_CALL(glClear(GL_STENCIL_BUFFER_BIT));
        GAME_GL_CALL(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GAME_GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GAME_GL_CALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
        
        kDefaultSpriteShader.Use();
        transform.SetScale(Vector2(transform.GetScale().x() + outline->GetWidth() * 2, transform.GetScale().y() + outline->GetWidth() * 2));
        kDefaultSpriteShader.SetUniformMatrix3("mvp", 1, false, Matrix3{view_projection_matrix * transform.matrix()}.data());

        game_.GetResourceManager().BindSpriteVAO();
        GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));

        GAME_GL_CALL(glStencilFunc(GL_EQUAL, 1, 0xFF));
        GAME_GL_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
        
        kColoredSpriteShader.Use();
        kColoredSpriteShader.SetUniform4v("u_color", 1, outline->GetColor().data());
        kColoredSpriteShader.SetUniformMatrix3("mvp", 1, false, Matrix3{Matrix3::Identity()}.data());

        game_.GetResourceManager().BindScreenPlaneVAO();
        GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));

        GAME_GL_CALL(glDisable(GL_STENCIL_TEST));

        transform.SetScale(Vector2(transform.GetScale().x() - outline->GetWidth() * 2, transform.GetScale().y() - outline->GetWidth() * 2));
      }

      kDefaultSpriteShader.Use();
      kDefaultSpriteShader.SetUniformMatrix3("mvp", 1, false, Matrix3{view_projection_matrix * transform.matrix()}.data());

      game_.GetResourceManager().BindSpriteVAO();
      GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));
      break;
    }


    case SpriteType::kTextured:
    {
      TexturedSpriteData *sprite_data = dynamic_cast<TexturedSpriteData*>(sprite.GetData());

      GAME_GL_CALL(glActiveTexture(GL_TEXTURE0));
      sprite_data->GetTexture().Bind();

      TransformComponent &transform = game_.GetRegistry().get<TransformComponent>(entity);
      OutlineComponent *outline = game_.GetRegistry().try_get<OutlineComponent>(entity);

      if(outline && outline->GetWidth() > 0.0f)
      {
        GAME_GL_CALL(glEnable(GL_STENCIL_TEST));
        GAME_GL_CALL(glClearStencil(0));
        GAME_GL_CALL(glClear(GL_STENCIL_BUFFER_BIT));
        GAME_GL_CALL(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GAME_GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GAME_GL_CALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
        
        sprite_data->GetShader().Use();
        transform.SetScale(Vector2(transform.GetScale().x() + outline->GetWidth() * 2, transform.GetScale().y() + outline->GetWidth() * 2));
        sprite_data->GetShader().SetUniformMatrix3("mvp", 1, false, Matrix3{view_projection_matrix * transform.matrix()}.data());

        game_.GetResourceManager().BindSpriteVAO();
        GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));

        GAME_GL_CALL(glStencilFunc(GL_EQUAL, 1, 0xFF));
        GAME_GL_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
        
        kColoredSpriteShader.Use();
        kColoredSpriteShader.SetUniform4v("u_color", 1, outline->GetColor().data());
        kColoredSpriteShader.SetUniformMatrix3("mvp", 1, false, Matrix3{Matrix3::Identity()}.data());

        game_.GetResourceManager().BindScreenPlaneVAO();
        GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));

        GAME_GL_CALL(glDisable(GL_STENCIL_TEST));

        transform.SetScale(Vector2(transform.GetScale().x() - outline->GetWidth() * 2, transform.GetScale().y() - outline->GetWidth() * 2));
      }
      
      sprite_data->GetShader().Use();
      sprite_data->GetShader().SetUniformMatrix3("mvp", 1, false, Matrix3{view_projection_matrix * transform.matrix()}.data());

      game_.GetResourceManager().BindSpriteVAO();
      GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));

      break;
    }


    case SpriteType::kAnimated:
    {
      AnimatedSpriteData *sprite_data = dynamic_cast<AnimatedSpriteData*>(sprite.GetData());

      GAME_GL_CALL(glActiveTexture(GL_TEXTURE0));
      sprite_data->GetTexture().Bind();

      TransformComponent &transform = game_.GetRegistry().get<TransformComponent>(entity);
      OutlineComponent *outline = game_.GetRegistry().try_get<OutlineComponent>(entity);

      if(outline && outline->GetWidth() > 0.0f)
      {
        GAME_GL_CALL(glEnable(GL_STENCIL_TEST));
        GAME_GL_CALL(glClearStencil(0));
        GAME_GL_CALL(glClear(GL_STENCIL_BUFFER_BIT));
        GAME_GL_CALL(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GAME_GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GAME_GL_CALL(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
        
        sprite_data->GetShader().Use();
        transform.SetScale(Vector2(transform.GetScale().x() + outline->GetWidth() * 2, transform.GetScale().y() + outline->GetWidth() * 2));
        sprite_data->GetShader().SetUniformMatrix3("mvp", 1, false, Matrix3{view_projection_matrix * transform.matrix()}.data());
        sprite_data->GetShader().SetUniform("sprite_index", sprite_data->GetSpriteIndex().x(), sprite_data->GetSpriteIndex().y());
        sprite_data->GetShader().SetUniform("atlas_step", sprite_data->GetAtlasStep());

        game_.GetResourceManager().BindSpriteVAO();
        GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));

        GAME_GL_CALL(glStencilFunc(GL_EQUAL, 1, 0xFF));
        GAME_GL_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
        
        kColoredSpriteShader.Use();
        kColoredSpriteShader.SetUniform4v("u_color", 1, outline->GetColor().data());
        kColoredSpriteShader.SetUniformMatrix3("mvp", 1, false, Matrix3{Matrix3::Identity()}.data());

        game_.GetResourceManager().BindScreenPlaneVAO();
        GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));

        GAME_GL_CALL(glDisable(GL_STENCIL_TEST));

        transform.SetScale(Vector2(transform.GetScale().x() - outline->GetWidth() * 2, transform.GetScale().y() - outline->GetWidth() * 2));
      }

      sprite_data->GetShader().Use();
      sprite_data->GetShader().SetUniformMatrix3("mvp", 1, false, Matrix3{view_projection_matrix * transform.matrix()}.data());

      game_.GetResourceManager().BindSpriteVAO();
      GAME_GL_CALL(glDrawElements(GL_TRIANGLES, game_.GetResourceManager().GetSpriteIndexCount(), GL_UNSIGNED_INT, 0));

      break;
    }

    default:
      GAME_ASSERT(false) << "Unhandled SpriteType";
    }
  }
}

void RenderSystem::DrawUI() noexcept
{
  ZoneScopedC(0x07dbd4);

  ImDrawData *draw_data = ImGui::GetDrawData();
  ImGui_ImplOpenGL3_RenderDrawData(draw_data);
}
} // game