#ifndef GAME_EVENT_HANDLER_HPP
#define GAME_EVENT_HANDLER_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/MathConstants.hpp"
#include "Utils/Enum.hpp"


namespace game
{
enum class EventType : uint32_t
{
  kNone = 0,
  kKeyDown,
  kKeyUp,
  kMouseButtonDown,
  kMouseButtonUp,
  kMouseMotion,
  kWindowResize,
  kRenderAreaResize,
  kQuit
};

union Event
{
public:
  Event(EventType type) noexcept : common_{type} {}
  ~Event() noexcept;
  Event(const Event &other) noexcept { *this = other; }
  Event(Event &&other) noexcept { *this = other; }
  Event &operator=(const Event &other) noexcept;
  Event &operator=(Event &&other) noexcept;


  static constexpr std::underlying_type_t<EventType> kCustomTypeMask = static_cast<std::underlying_type_t<EventType>>(1) << (sizeof(EventType) * CHAR_BIT - 1);

  [[nodiscard]] constexpr auto GetType() const noexcept -> EventType { return common_.type; }
  [[nodiscard]] auto GetName() const noexcept -> std::string;

  [[nodiscard]] constexpr auto GetKeycode() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kKeyDown || common_.type == EventType::kKeyUp); return key_data_.keycode; }
  constexpr auto SetKeycode(int keycode) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kKeyDown || common_.type == EventType::kKeyUp); key_data_.keycode = keycode; return *this; }
  [[nodiscard]] constexpr auto GetScancode() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kKeyDown || common_.type == EventType::kKeyUp); return key_data_.scancode; }
  constexpr auto SetScancode(int scancode) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kKeyDown || common_.type == EventType::kKeyUp); key_data_.scancode = scancode; return *this; }
  [[nodiscard]] constexpr auto GetModKeys() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kKeyDown || common_.type == EventType::kKeyUp); return key_data_.mod_keys; }
  constexpr auto SetModKeys(uint16_t mod_keys) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kKeyDown || common_.type == EventType::kKeyUp); key_data_.mod_keys = mod_keys; return *this; }

  [[nodiscard]] constexpr auto GetMouseButton() const noexcept -> uint8_t { GAME_ASSERT(common_.type == EventType::kMouseButtonDown || common_.type == EventType::kMouseButtonUp); return mouse_button_.button; }
  constexpr auto SetMouseButton(uint8_t button) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kMouseButtonDown || common_.type == EventType::kMouseButtonUp); mouse_button_.button = button; return *this; }
  
  [[nodiscard]] constexpr auto GetNewWindowWidth() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kWindowResize); return window_resize_.new_width; }
  constexpr auto SetNewWindowWidth(int new_width) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kWindowResize); window_resize_.new_width = new_width; return *this; }
  [[nodiscard]] constexpr auto GetNewWindowHeight() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kWindowResize); return window_resize_.new_height; }
  constexpr auto SetNewWindowHeight(int new_height) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kWindowResize); window_resize_.new_height = new_height; return *this; }
  [[nodiscard]] constexpr auto GetOldWindowWidth() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kWindowResize); return window_resize_.old_width; }
  constexpr auto SetOldWindowWidth(int old_width) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kWindowResize); window_resize_.old_width = old_width; return *this; }
  [[nodiscard]] constexpr auto GetOldWindowHeight() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kWindowResize); return window_resize_.old_height; }
  constexpr auto SetOldWindowHeight(int old_height) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kWindowResize); window_resize_.old_height = old_height; return *this; }
  [[nodiscard]] auto GetNewWindowResolution() const noexcept -> Vector2i { GAME_ASSERT(common_.type == EventType::kWindowResize); return Vector2i{window_resize_.new_width, window_resize_.new_height}; }
  auto SetNewWindowResolution(Vector2i new_window_resolution) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kWindowResize); window_resize_.new_width = new_window_resolution.x(); window_resize_.new_height = new_window_resolution.y(); return *this; }
  [[nodiscard]] auto GetOldWindowResolution() const noexcept -> Vector2i { GAME_ASSERT(common_.type == EventType::kWindowResize); return Vector2i{window_resize_.old_width, window_resize_.old_height}; }
  auto SetOldWindowResolution(Vector2i old_window_resolution) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kWindowResize); window_resize_.old_width = old_window_resolution.x(); window_resize_.old_height = old_window_resolution.y(); return *this; }

  [[nodiscard]] constexpr auto GetNewRenderAreaWidth() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); return render_area_resize_.new_width; }
  constexpr auto SetNewRenderAreaWidth(int new_width) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); render_area_resize_.new_width = new_width; return *this; }
  [[nodiscard]] constexpr auto GetOldRenderAreaWidth() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); return render_area_resize_.old_width; }
  constexpr auto SetOldRenderAreaWidth(int old_width) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); render_area_resize_.old_width = old_width; return *this; }
  [[nodiscard]] constexpr auto GetNewRenderAreaHeight() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); return render_area_resize_.new_height; }
  constexpr auto SetNewRenderAreaHeight(int new_height) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); render_area_resize_.new_height = new_height; return *this; }
  [[nodiscard]] constexpr auto GetOldRenderAreaHeight() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); return render_area_resize_.old_height; }
  constexpr auto SetOldRenderAreaHeight(int old_height) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); render_area_resize_.old_height = old_height; return *this; }
  [[nodiscard]] auto GetNewRenderAreaResolution() const noexcept -> Vector2i { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); return Vector2i{render_area_resize_.new_width, render_area_resize_.new_height}; }
  auto SetNewRenderAreaResolution(Vector2i new_render_area_resolution) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); render_area_resize_.new_width = new_render_area_resolution.x(); render_area_resize_.new_height = new_render_area_resolution.y(); return *this; }
  [[nodiscard]] auto GetOldRenderAreaResolution() const noexcept -> Vector2i { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); return Vector2i{render_area_resize_.old_width, render_area_resize_.old_height}; }
  auto SetOldRenderAreaResolution(Vector2i old_render_area_resolution) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kRenderAreaResize); render_area_resize_.old_width = old_render_area_resolution.x(); render_area_resize_.old_height = old_render_area_resolution.y(); return *this; }

  [[nodiscard]] constexpr auto GetNewMousePositionX() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kMouseMotion); return mouse_motion_.new_mouse_position_x; }
  constexpr auto SetNewMousePositionX(int new_mouse_position_x) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kMouseMotion); mouse_motion_.new_mouse_position_x = new_mouse_position_x; return *this; }
  [[nodiscard]] constexpr auto GetMouseDeltaX() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kMouseMotion); return mouse_motion_.mouse_delta_x; }
  constexpr auto SetMouseDeltaX(int mouse_delta_x) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kMouseMotion); mouse_motion_.mouse_delta_x = mouse_delta_x; return *this; }
  [[nodiscard]] constexpr auto GetNewMousePositionY() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kMouseMotion); return mouse_motion_.new_mouse_position_y; }
  constexpr auto SetNewMousePositionY(int new_mouse_position_y) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kMouseMotion); mouse_motion_.new_mouse_position_y = new_mouse_position_y; return *this; }
  [[nodiscard]] constexpr auto GetMouseDeltaY() const noexcept -> int { GAME_ASSERT(common_.type == EventType::kMouseMotion); return mouse_motion_.mouse_delta_y; }
  constexpr auto SetMouseDeltaY(int mouse_delta_y) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kMouseMotion); mouse_motion_.mouse_delta_y = mouse_delta_y; return *this; }
  [[nodiscard]] auto GetNewMousePosition() const noexcept -> Vector2i { GAME_ASSERT(common_.type == EventType::kMouseMotion); return Vector2i{mouse_motion_.new_mouse_position_x, mouse_motion_.new_mouse_position_y}; }
  auto SetNewMousePosition(Vector2i new_mouse_position) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kMouseMotion); mouse_motion_.new_mouse_position_x = new_mouse_position.x(); mouse_motion_.new_mouse_position_y = new_mouse_position.y(); return *this; }
  [[nodiscard]] auto GetMouseDelta() const noexcept -> Vector2i { GAME_ASSERT(common_.type == EventType::kMouseMotion); return Vector2i{mouse_motion_.mouse_delta_x, mouse_motion_.mouse_delta_y}; }
  auto SetMouseDelta(Vector2i mouse_delta) noexcept -> Event & { GAME_ASSERT(common_.type == EventType::kMouseMotion); mouse_motion_.mouse_delta_x = mouse_delta.x(); mouse_motion_.mouse_delta_y = mouse_delta.y(); return *this; }

  template<typename T>
  [[nodiscard]] auto GetCustomData() noexcept -> std::any & { GAME_ASSERT(common_.type & kCustomTypeMask); return custom_.data; }

private:
  struct Common
  {
    EventType type;
  };
  Common common_;

  struct KeyData : public Common
  {
    int keycode;
    int scancode;
    uint16_t mod_keys;
  };
  KeyData key_data_;

  struct MouseButton : public Common
  {
    uint8_t button;
  };
  MouseButton mouse_button_;

  struct MouseMotion : public Common
  {
    int new_mouse_position_x;
    int new_mouse_position_y;
    int mouse_delta_x;
    int mouse_delta_y;
  };
  MouseMotion mouse_motion_;

  struct WindowResize : public Common
  {
    int new_width;
    int new_height;
    int old_width;
    int old_height;
  };
  WindowResize window_resize_;

  struct RenderAreaResize : public Common
  {
    int new_width;
    int new_height;
    int old_width;
    int old_height;
  };
  RenderAreaResize render_area_resize_;

  struct Custom : public Common
  {
    std::any data;
  };
  Custom custom_;
};



class EventCleaner;
class Game;

// Add listener to an event and dispatch events with specific type
// If callback returns true then event is handeled and doesn't spread further
class EventSystem
{
  friend EventCleaner;
public:
  using CallbackType = bool(*)(const Event &, void *data);
  using ListenerMapValueType = std::pair<void*, CallbackType>;
  using ListenerMapType = std::unordered_map<std::size_t, ListenerMapValueType>;
  using EventMapType = std::multimap<EventType, std::size_t>;
  using QueueType = std::queue<Event>;

  EventSystem() noexcept = default;
  EventSystem(const EventSystem &) noexcept = delete;
  EventSystem(EventSystem &&) noexcept = delete;
  EventSystem &operator=(const EventSystem &) noexcept = delete;
  EventSystem &operator=(EventSystem &&) noexcept = delete;


  // Add to type event listener that will be trigered when event fires
  // First in is first to be called when event happens
  // UID is returned that can be used to remvoe listener
  auto AddListener(EventCleaner &cleaner, EventType type, void *data, CallbackType callback) noexcept -> std::size_t;
  void RemoveListener(EventCleaner &cleaner, std::size_t uid) noexcept;
  void ClearListeners(EventType type) noexcept;
  void ClearListeners() noexcept { listeners_.clear(); events_.clear(); }
 
  // Instantly dispatch event
  void DispatchEvent(const Event &event) noexcept;
  void EnqueEvent(const Event &event) noexcept { queue_.push(event); }
  void DispatchEnquedEvents() noexcept { ZoneScopedC(0xe8bb25); while(queue_.size()) { DispatchEvent(queue_.front()); queue_.pop(); } }


private:
  // Used in event cleaner to directly remove listener when EventCleaner is destroyed
  void RemoveListener(std::size_t uid) noexcept { listeners_.erase(uid); }

  QueueType queue_;
  ListenerMapType listeners_;
  EventMapType events_;
  

  std::size_t last_uid = 0;
};



// Class that manages lifetime of event listeners
// It removes listeners that were created using it from EventSystem on destruction
class EventCleaner
{
  friend EventSystem;
public:
  EventCleaner(EventSystem &event_handler) noexcept : event_handler_{event_handler} {}
  
  ~EventCleaner() noexcept { ZoneScopedC(0xe8bb25); for(auto uid : uids_) event_handler_.RemoveListener(uid); }

  // Add position (you can get it when adding listener in EventSystem) to EventClener that will be removed from EventSystem on the destruction
  void AddUid(std::size_t uid) noexcept { uids_.push_back(uid); }
  // Remove position (you can get it when adding listener in EventSystem) from EventCleaner to not remove it on the destruction of a cleaner
  void RemoveUid(std::size_t uid) noexcept { std::swap(uids_.back(), *std::find(uids_.begin(), uids_.end(), uid)); uids_.pop_back(); }
  // Remove all positions from cleaner so they won't be removed on destruction of EventCleaner
  void ClearUids() noexcept { uids_.clear(); }

private:
  EventSystem &event_handler_;
  std::vector<std::size_t> uids_;
};


inline void EventSystem::RemoveListener(EventCleaner &cleaner, std::size_t uid) noexcept
{
  listeners_.erase(uid);
  cleaner.RemoveUid(uid);
}

inline void EventSystem::DispatchEvent(const Event &event) noexcept
{
  ZoneScopedC(0xe8bb25);
  ZoneText(event.GetName().c_str(), event.GetName().size());

  std::pair<EventMapType::const_iterator, EventMapType::const_iterator> range = events_.equal_range(event.GetType());

  ZoneValue(std::distance(range.first, range.second));

  for(; range.first != range.second; ++range.first)
  {
    ListenerMapType::iterator it = listeners_.find(range.first->second);
    if(it == listeners_.end())
    {
      events_.erase(range.first);
      continue;
    }

    if(it->second.second(event, it->second.first))
      break;
  }
}

inline void EventSystem::ClearListeners(EventType type) noexcept
{
  ZoneScopedC(0xe8bb25);

  std::pair<EventMapType::const_iterator, EventMapType::const_iterator> range = events_.equal_range(type);
  for(; range.first != range.second; ++range.first)
    listeners_.erase(listeners_.find(range.first->second));
}

inline auto EventSystem::AddListener(EventCleaner &cleaner, EventType type, void *data, CallbackType callback) noexcept -> std::size_t
{
  GAME_ASSERT(this == &cleaner.event_handler_);
  ZoneScopedC(0xe8bb25);

  ++last_uid;
  listeners_.emplace(last_uid, ListenerMapValueType(data, callback));
  events_.emplace(type, last_uid);
  cleaner.AddUid(last_uid);
  return last_uid;
}
} // game

#endif // GAME_EVENT_HANDLER_HPP