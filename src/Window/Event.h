#pragma once

namespace raytracing {

// EVENT UTILITY
enum class EventType {
    NONE = 0,
    KEY_PRESS,
    KEY_RELEASE,
    KEY_REPEAT,
    MOUSE_PRESS,
    MOUSE_RELEASE,
    MOUSE_POSITION,
    MOUSE_SCROLL,
    WINDOW_RESIZE
};

class Event {
   private:
    EventType m_EventType = EventType::NONE;
    bool m_Handled = false;
    /* Avoiding implementing any kind of dependency graph. Dirty flag triggers RenderDevice dirtying if set. */
    bool m_Dirty = false;

   public:
    Event(EventType type) : m_EventType(type) {}

    EventType GetEventType() const { return m_EventType; }

    void Handle() { m_Handled = true; }
    bool IsHandled() { return m_Handled; }

    void SetDirty(bool dirty) { m_Dirty = dirty; }
    bool IsDirty() { return m_Dirty; }
};

class EventDispatcher {
   private:
    Event& m_Event;

   public:
    EventDispatcher(Event& event)
        : m_Event(event) {}

    template <typename T, typename F>
    bool Dispatch(EventType type, const F& func, bool dirty = true) {
        /* Note: If user mismatches T and type it will compile time error from static_cast. */
        if (m_Event.GetEventType() == type) {
            T& event = static_cast<T&>(m_Event);
            func(event);

            m_Event.Handle();
            m_Event.SetDirty(dirty);
            return true;
        }
        return false;
    }
};
//

// EVENT TYPES
class KeyEvent : public Event {
   private:
    int m_Key;

   public:
    KeyEvent(EventType type, int key) : Event(type), m_Key(key) {}

    int GetKey() { return m_Key; }
};

class MouseButtonEvent : public Event {
   private:
    int m_Button;

   public:
    MouseButtonEvent(EventType type, int button)
        : Event(type), m_Button(button) {}

    int GetButton() { return m_Button; }
};

class MousePositionEvent : public Event {
   private:
    double m_XPos, m_YPos;

   public:
    MousePositionEvent(double xpos, double ypos)
        : Event(EventType::MOUSE_POSITION), m_XPos(xpos), m_YPos(ypos) {}

    double GetXPos() { return m_XPos; }
    double GetYPos() { return m_YPos; }
};

class MouseScrollEvent : public Event {
   private:
    double m_XOffset, m_YOffset;

   public:
    MouseScrollEvent(double xoffset, double yoffset)
        : Event(EventType::MOUSE_SCROLL),
          m_XOffset(xoffset),
          m_YOffset(yoffset) {}

    double GetXOffset() { return m_XOffset; }
    double GetYOffset() { return m_YOffset; }
};

class WindowResizeEvent : public Event {
   private:
    uint32_t m_Width, m_Height;

   public:
    WindowResizeEvent(uint32_t width, uint32_t height)
        : Event(EventType::WINDOW_RESIZE), m_Width(width), m_Height(height) {}

    uint32_t GetWidth() { return m_Width; }
    uint32_t GetHeight() { return m_Height; }
};
//

}  // namespace raytracing