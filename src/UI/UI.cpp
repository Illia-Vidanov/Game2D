#include "UI.hpp"

#include "Setup.hpp"

#include "Core/Game.hpp"
#include "Utils/Logger.hpp"
#include "Rendering/OutlineComponent.hpp"
#include "Physics/TransformComponent.hpp"
#include "Physics/RigidbodyComponent.hpp"


namespace game
{
UI::UI(Game &game) noexcept
  : game_{game}
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

void UI::WindowResizeEvent() noexcept
{
  ZoneScopedC(0xa5bfc7);

  window_resized_ = true;
  Vector2 last_global_scale = global_scale_;
  global_scale_ = Vector2{static_cast<DefaultFloatType>(game_.GetWindow().kUnitsPerScreenX) / static_cast<DefaultFloatType>(game_.GetWindow().GetWidth()), static_cast<DefaultFloatType>(game_.GetWindow().kUnitsPerScreenY) / static_cast<DefaultFloatType>(game_.GetWindow().GetHeight())};
  scale_multiplier_ = Vector2{last_global_scale.x() / global_scale_.x(), last_global_scale.y() / global_scale_.y()};
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
  ImGui::SetNextWindowPos(ImVec2{10 * scale_multiplier_.x(), 10 * scale_multiplier_.y()}, ImGuiCond_Once);
  ImGui::SetNextWindowSize(ImVec2{100 * scale_multiplier_.x(), 200 * scale_multiplier_.y()}, ImGuiCond_Once);
  
  if(ImGui::Begin("Debug"))
  {
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
        if(!ImGui::CollapsingHeader(entity->GetName().c_str(), first ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
          continue;

        bool entity_active = entity->GetActive();
        if(ImGui::Checkbox("Active", &entity_active))
          entity->SetActive(entity_active);

        if(!entity_active)
          ImGui::BeginDisabled();

        first = false;

        TransformComponent *transform = entity->TryGetComponent<TransformComponent>();

        if(transform && ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
          DrawTransformComponent(transform, entity);
          ImGui::TreePop();
        }

        if(!entity_active)
          ImGui::EndDisabled();
      }
    }
  }
  ImGui::End();
}

void UI::DrawTransformComponent(TransformComponent *transform, Entity *entity) const noexcept
{
  bool component_active = transform->GetActive();
  if(ImGui::Checkbox("Active", &component_active))
    transform->SetActive(component_active);
  
  if(!component_active)
    return;

  bool changed = false;
  Vector2 position = transform->GetPosition();
  ImGui::SetNextItemWidth(100);
  if(ImGui::DragFloat2("Position", position.data()))
  {
    transform->SetPosition(position);
    changed = true;
  }

  float rotation = transform->GetRotationDegrees();
  ImGui::SetNextItemWidth(100);
  if(ImGui::DragFloat("Rotation", &rotation))
  {
    transform->SetRotationDegrees(rotation);
    changed = true;
  }

  Vector2 scale = transform->GetScale();
  ImGui::SetNextItemWidth(100);
  if(ImGui::DragFloat2("Scale", scale.data()))
    transform->SetScale(scale);
  
  if(changed)
    game_.GetPhysicsSystem().Updateb2Transform(entity);
}
} // game