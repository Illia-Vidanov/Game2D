#include "UI.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Core/EventSystem.hpp"
#include "Utils/Logger.hpp"
#include "Rendering/OutlineComponent.hpp"
#include "Physics/TransformComponent.hpp"
#include "Physics/RigidbodyComponent.hpp"


namespace game
{
UI::UI(Game &game) noexcept
  : game_{game}
  , event_cleaner_{game_.GetEventSystem()}
  , global_scale_{static_cast<DefaultFloatType>(game_.GetWindow().kUnitsPerScreenX) / static_cast<DefaultFloatType>(game_.GetWindow().GetWidth()), static_cast<DefaultFloatType>(game_.GetWindow().kUnitsPerScreenY) / static_cast<DefaultFloatType>(game_.GetWindow().GetHeight())}
  , scale_multiplier_{global_scale_}
{
  ZoneScopedC(0xa5bfc7);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(game_.GetWindow().GetSDLWindow(), game_.GetRenderSystem().GetSDLGLContext());
  ImGui_ImplOpenGL3_Init("#version 460");

  game_.GetEventSystem().AddListener(event_cleaner_, EventType::kWindowResize, this, [](const Event &event, void *ui) -> bool { return reinterpret_cast<UI*>(ui)->WindowResizeEvent(event); });
}

UI::~UI() noexcept
{
  ZoneScopedC(0xa5bfc7);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void UI::Update() noexcept
{
  ZoneScopedC(0xa5bfc7);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  if(game_.GetDebug().GetActive())
    DrawDebugWindow();

  //ImGui::ShowDemoWindow();
  

  window_resized_ = false;

  ImGui::Render();
}

auto UI::WindowResizeEvent(const Event &event) noexcept -> bool
{
  ZoneScopedC(0xa5bfc7);

  window_resized_ = true;
  Vector2 last_global_scale = global_scale_;
  global_scale_ = Vector2{static_cast<DefaultFloatType>(game_.GetWindow().kUnitsPerScreenX) / static_cast<DefaultFloatType>(game_.GetWindow().GetWidth()), static_cast<DefaultFloatType>(game_.GetWindow().kUnitsPerScreenY) / static_cast<DefaultFloatType>(game_.GetWindow().GetHeight())};
  scale_multiplier_ = Vector2{last_global_scale.x() / global_scale_.x(), last_global_scale.y() / global_scale_.y()};
  
  return false;
}

void UI::UpdateWindow() const noexcept
{
  ZoneScopedC(0xa5bfc7);

  ImVec2 position = ImGui::GetWindowPos();
  ImGui::SetWindowPos(ImVec2{position.x * scale_multiplier_.x(), position.y * scale_multiplier_.y()});
  ImVec2 size = ImGui::GetWindowSize();
  ImGui::SetWindowSize(ImVec2{size.x * scale_multiplier_.x(), size.y * scale_multiplier_.y()});
}

void UI::DrawDebugWindow() const noexcept
{
  if(ImGui::Begin("Debug"))
  {
    ImGui::SetWindowPos(ImVec2{10 * scale_multiplier_.x(), 10 * scale_multiplier_.y()}, ImGuiCond_Once);
    ImGui::SetWindowSize(ImVec2{20 * scale_multiplier_.x(), 10 * scale_multiplier_.y()}, ImGuiCond_Once);

    if(window_resized_)
      UpdateWindow();

    static bool select_mode = game_.GetDebug().GetSelectMode();
    if(ImGui::Checkbox("Select Mode", &select_mode))
      game_.GetDebug().SetSelectMode(select_mode);
      
    if(!game_.GetDebug().GetSelectedEntities().empty())
    {
      bool first = true;
      for(Entity *entity : game_.GetDebug().GetSelectedEntities())
      {
        if(!ImGui::CollapsingHeader("Entity", first ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
          continue;
        first = false;

        TransformComponent *transform = entity->TryGetComponent<TransformComponent>();

        if(transform && ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
          Vector2 position = transform->GetPosition();
          if(ImGui::InputFloat2("Position", position.data()))
          {
            transform->SetPosition(position);
            game_.GetPhysicsSystem().Updateb2Transform(entity);
          }
        }
      }
    }
  }
  ImGui::End();
}
} // game