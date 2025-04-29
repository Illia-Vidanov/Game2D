#ifndef GAME_EVENT_HANDLER_HPP
#define GAME_EVENT_HANDLER_HPP

#include "Setup.hpp"

#include "Utils/Logger.hpp"
#include "Utils/Enum.hpp"


namespace game
{
enum class EventType : uint32_t
{
  kNone = 0,
  kKeyDown,
  kKeyUp,
  kWindowResize,
  kRenderAreaResize,
  kQuit
};

union Event
{
public:
  struct Common
  {
    friend Event;
  public:
    explicit constexpr inline Common(EventType new_type) noexcept
    : type{new_type}
    {}

  // private here is used as const to make it immutable by users
  private:
    EventType type;
  };


private:
  struct Keyboard
  {
    explicit constexpr inline Keyboard(int new_keycode, int new_scancode, uint16_t new_mod_keys) noexcept
    : keycode{new_keycode}
    , scancode{new_scancode}
    , mod_keys{new_mod_keys}
    {}
    int keycode;
    int scancode;
    uint16_t mod_keys;
  };
  struct CommonKeyboard : Common, Keyboard
  {};


public:
  struct KeyDown : Common, Keyboard
  {
    explicit constexpr inline KeyDown(int new_keycode, int new_scancode, uint16_t new_mod_keys) noexcept
    : Common{EventType::kKeyDown}
    , Keyboard{new_keycode, new_scancode, new_mod_keys}
    {}
  };
  constexpr inline Event(const KeyDown &key_down) noexcept
  : key_down_{key_down}
  {}


  struct KeyUp : Common, Keyboard
  {
    explicit constexpr inline KeyUp(int new_keycode, int new_scancode, uint16_t new_mod_keys) noexcept
    : Common{EventType::kKeyUp}
    , Keyboard{new_keycode, new_scancode, new_mod_keys}
    {}
  };
  constexpr inline Event(const KeyUp &key_up) noexcept
  : key_up_{key_up}
  {}


  struct Quit : Common
  {
    explicit constexpr inline Quit() noexcept
      : Common{EventType::kQuit}
    {}
  };
  inline Event(const Quit &quit) noexcept
    : quit_{quit}
  {}


private:
  struct AreaResize
  {
  explicit constexpr inline AreaResize(int new_old_resolution_x, int new_old_resolution_y, int new_new_resolution_x, int new_new_resolution_y) noexcept
    : old_resolution_x{new_old_resolution_x}
    , old_resolution_y{new_old_resolution_y}
    , new_resolution_x{new_new_resolution_x}
    , new_resolution_y{new_new_resolution_y}
    {}

    int old_resolution_x;
    int old_resolution_y;
    int new_resolution_x;
    int new_resolution_y;
  };
  struct CommonAreaResize : Common, AreaResize
  {};


public:
  struct WindowResize : Common, AreaResize
  {
    explicit constexpr inline WindowResize(int new_old_resolution_x, int new_old_resolution_y, int new_new_resolution_x, int new_new_resolution_y) noexcept
      : Common{EventType::kWindowResize}
      , AreaResize{new_old_resolution_x, new_old_resolution_y, new_new_resolution_x, new_new_resolution_y}
    {}
  };
  inline Event(const WindowResize &window_resize) noexcept
    : window_resize_{window_resize}
  {}


  struct RenderAreaResize : Common, AreaResize
  {
    explicit constexpr inline RenderAreaResize(int new_old_resolution_x, int new_old_resolution_y, int new_new_resolution_x, int new_new_resolution_y) noexcept
      : Common{EventType::kRenderAreaResize}
      , AreaResize{new_old_resolution_x, new_old_resolution_y, new_new_resolution_x, new_new_resolution_y}
    {}
  };
  inline Event(const RenderAreaResize &render_area_resize) noexcept
    : render_area_resize_{render_area_resize}
  {}


  // Custom type should specify it's type for listeners and it's last bit should be 1 or contain just use kCustomTypeBitMask
  struct Custom : Common
  {
    explicit constexpr inline Custom(EventType custom_type, void *new_data) noexcept
    : Common{custom_type}
    , data{new_data}
    {
      GAME_ASSERT_STD(custom_type & kCustomTypeBitMask, "Custom type's should contain kCustomTypeBitMask");
    }
    void *data;
  };
  inline Event(const Custom &custom) noexcept
  : custom_{custom}
  {}

public:
  static constexpr inline std::underlying_type_t<EventType> kCustomTypeBitMask = std::underlying_type_t<EventType>{1} << (sizeof(EventType) * CHAR_BIT - 1);

  
  [[nodiscard]] constexpr inline auto GetType() const noexcept -> EventType { return common_.type; }
  // Get name of event according to type
  // If type is custom "Other: (type value)" is returned
  // Mainly debuging feature
  [[nodiscard]] auto GetName() const noexcept -> std::string;
	
	[[nodiscard]] inline auto GetKeycode() const noexcept -> int
  {
    GAME_ASSERT_STD(common_.type == EventType::kKeyDown
                 || common_.type == EventType::kKeyUp
                 , "Acces data from wrong event type. Expected: kKeyDown/kKeyUp/kKeyPressed");
    return keyboard_.keycode;
  }
	[[nodiscard]] inline auto GetScancode() const noexcept -> int
  {
    GAME_ASSERT_STD(common_.type == EventType::kKeyDown
                 || common_.type == EventType::kKeyUp
                 , "Acces data from wrong event type. Expected: kKeyDown/kKeyUp/kKeyPressed");
    return keyboard_.scancode;
  }
	[[nodiscard]] inline auto GetModKeys() const noexcept -> uint16_t
  {
    GAME_ASSERT_STD(common_.type == EventType::kKeyDown
                 || common_.type == EventType::kKeyUp
                 , "Acces data from wrong event type. Expected: kKeyDown/kKeyUp/kKeyPressed");
    return keyboard_.mod_keys;
  }
	[[nodiscard]] inline auto GetCustomData() const noexcept -> void*
  {
    GAME_ASSERT_STD(GetType() & kCustomTypeBitMask, "Acces data from wrong event type. Expected: to contain kCustomTypeBitMask");
    return custom_.data;
  }
  [[nodiscard]] inline auto GetOldResolutionX() const noexcept -> int
  {
    GAME_ASSERT_STD(common_.type == EventType::kWindowResize
                 || common_.type == EventType::kRenderAreaResize, "Acces data from wrong event type. Expected: kWindowResize or kRenderAreaResize");
    return area_resize_.old_resolution_x;
  }
  [[nodiscard]] inline auto GetOldResolutionY() const noexcept -> int
  {
    GAME_ASSERT_STD(common_.type == EventType::kWindowResize
                 || common_.type == EventType::kRenderAreaResize, "Acces data from wrong event type. Expected: kWindowResize or kRenderAreaResize");
    return area_resize_.old_resolution_y;
  }
  [[nodiscard]] inline auto GetNewResolutionX() const noexcept -> int
  {
    GAME_ASSERT_STD(common_.type == EventType::kWindowResize
                 || common_.type == EventType::kRenderAreaResize, "Acces data from wrong event type. Expected: kWindowResize or kRenderAreaResize");
    return area_resize_.new_resolution_x;
  }
  [[nodiscard]] inline auto GetNewResolutionY() const noexcept -> int
  {
    GAME_ASSERT_STD(common_.type == EventType::kWindowResize
                 || common_.type == EventType::kRenderAreaResize, "Acces data from wrong event type. Expected: kWindowResize or kRenderAreaResize");
    return area_resize_.new_resolution_y;
  }


// private here is used as const to make it immutable by users
private:
  Common common_;
  CommonKeyboard keyboard_;
	KeyDown key_down_;
	KeyUp key_up_;
	Quit quit_;
  CommonAreaResize area_resize_;
  WindowResize window_resize_;
  RenderAreaResize render_area_resize_;
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
  using EventMapType = std::unordered_multimap<EventType, std::size_t>;
  using QueueType = std::queue<Event>;


  // Add to type event listener that will be trigered when event fires
  // First in is first to be called when event happens
  // UID is returned that can be used to remvoe listener
  inline auto AddListener(EventCleaner &cleaner, EventType type, void *data, CallbackType callback) noexcept -> std::size_t;
  inline void RemoveListener(EventCleaner &cleaner, std::size_t uid) noexcept;
  // Remove all listeners of specified type
  inline void ClearListeners(EventType type) noexcept;
  // Clear all listeners
  inline void ClearListeners() noexcept { listeners_.clear(); events_.clear(); }
 
  // Instantly dispatch event
  inline void DispatchEvent(const Event &event) noexcept;
  inline void EnqueEvent(const Event &event) noexcept { queue_.push(event); }
  inline void DispatchEnquedEvents() noexcept { ZoneScopedC(0xe8bb25); while(queue_.size()) { DispatchEvent(queue_.front()); queue_.pop(); } }


private:
  // Used in event cleaner to directly remove listener when EventCleaner is destroyed
  inline void RemoveListener(std::size_t uid) noexcept { listeners_.erase(uid); }

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
  inline EventCleaner(EventSystem &event_handler) noexcept : event_handler_{event_handler} {}
  inline ~EventCleaner() noexcept { ZoneScopedC(0xe8bb25); for(auto uid : uids_) event_handler_.RemoveListener(uid); }

  // Add position (you can get it when adding listener in EventSystem) to EventClener that will be removed from EventSystem on the destruction
  inline void AddUid(std::size_t uid) noexcept { uids_.push_back(uid); }
  // Remove position (you can get it when adding listener in EventSystem) from EventCleaner to not remove it on the destruction of a cleaner
  inline void RemoveUid(std::size_t uid) noexcept { std::swap(uids_.back(), *std::find(uids_.begin(), uids_.end(), uid)); uids_.pop_back(); }
  // Remove all positions from cleaner so they won't be removed on destruction of EventCleaner
  inline void ClearUids() noexcept { uids_.clear(); }

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
  ZoneScopedC(0xe8bb25);

  GAME_ASSERT(this == &cleaner.event_handler_) << "Same cleaner used for multiple EventSystems:\n"
                                                  "This handler: " << this
                                               << "\nHandler which EventCleaner is bound to: " << &cleaner.event_handler_;
  ++last_uid;
  listeners_.emplace(last_uid, ListenerMapValueType(data, callback));
  events_.emplace(type, last_uid);
  cleaner.AddUid(last_uid);
  return last_uid;
}
} // game

#endif // GAME_EVENT_HANDLER_HPP