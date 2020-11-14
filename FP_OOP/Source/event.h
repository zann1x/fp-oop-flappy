#pragma once

#include <functional>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "functions.h"
#include "keycode.h"

/*
====================
EventType
====================
*/
enum class EventType {
    GAME_LOSS_EVENT,

    WINDOW_CLOSE_EVENT,
    WINDOW_RESIZE_EVENT,
    WINDOW_KEY_PRESS_EVENT,
    WINDOW_KEY_RELEASE_EVENT,
    WINDOW_MOUSE_PRESS_EVENT,
    WINDOW_MOUSE_RELEASE_EVENT,
    WINDOW_MOUSE_MOVE_EVENT
};

/*
====================
EventSource
====================
*/
enum class EventSource {
    WINDOW,
    RENDERER,
    GAME,
};

/*
====================
Event
====================
*/
class Event {
public:
    virtual EventType event_type(void) const = 0;
};


#define EVENT_CLASS_TYPE(type) \
    static EventType static_type(void) { return type; } \
    virtual EventType event_type(void) const override { return type; }

/*
====================
EventBus
====================
*/
class EventBus {
public:
    using EventUpdateFunction = std::function<void(const Event&)>;

    static EventBus& get_instance(void) {
        static EventBus _instance;
        return _instance;
    }

    void attach(EventType type, EventUpdateFunction fun) {
        update_handler[type].push_back(fun);
    }

    void dispatch(const Event& event) {
        auto type_handler = update_handler[event.event_type()];
        Functions::for_each(type_handler, [&event](EventUpdateFunction& fun) {
            fun(event);
        });
    }

private:
    std::unordered_map<EventType, std::vector<EventUpdateFunction>> update_handler;
};

/*
====================
GameLossEvent
====================
*/
class GameLossEvent : public Event {
public:
    GameLossEvent(void) { }

    EVENT_CLASS_TYPE(EventType::GAME_LOSS_EVENT)

    friend std::ostream& operator<<(std::ostream& os, const GameLossEvent& ev) {
        return (os << "Game lost");
    }
};

/*
====================
KeyPressEvent
====================
*/
class KeyPressEvent : public Event {
public:
    KeyPressEvent(KeyCode key_code)
        : key{ key_code } {
    }

    EVENT_CLASS_TYPE(EventType::WINDOW_KEY_PRESS_EVENT)

    KeyCode key_code() const { return key; }

    friend std::ostream& operator<<(std::ostream& os, const KeyPressEvent& ev) {
        return (os << "Key pressed: " << (int)ev.key_code());
    }

private:
    KeyCode key;
};

/*
====================
KeyReleaseEvent
====================
*/
class KeyReleaseEvent : public Event {
public:
    KeyReleaseEvent(KeyCode key_code)
        : key{ key_code } {
    }

    EVENT_CLASS_TYPE(EventType::WINDOW_KEY_RELEASE_EVENT)

    KeyCode key_code() const { return key; }

    friend std::ostream& operator<<(std::ostream& os, const KeyReleaseEvent& ev) {
        return (os << "Key released: " << (int)ev.key_code());
    }

private:
    KeyCode key;
};

/*
====================
WindowCloseEvent
====================
*/
class WindowCloseEvent : public Event {
public:
    WindowCloseEvent(void) {}

    EVENT_CLASS_TYPE(EventType::WINDOW_CLOSE_EVENT)
};

/*
====================
WindowResizeEvent
====================
*/
class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(int width, int height) 
        : _width{ width }, _height{ height } {
    }

    EVENT_CLASS_TYPE(EventType::WINDOW_RESIZE_EVENT)

    const std::pair<int, int> size(void) const { return std::make_pair(_width, _height); }

    friend std::ostream& operator<<(std::ostream& os, const WindowResizeEvent& ev) {
        const auto [width, height] = ev.size();
        return (os << "Window resize: (" << width << "," << height << ")");
    }

private:
    int _width;
    int _height;
};

/*
====================
MouseMoveEvent
====================
*/
class MouseMoveEvent : public Event {
public:
    MouseMoveEvent(double xpos, double ypos)
        : xpos{ xpos }, ypos{ ypos } { }

    EVENT_CLASS_TYPE(EventType::WINDOW_MOUSE_MOVE_EVENT)

    double x() const { return xpos; }
    double y() const { return ypos; }

    friend std::ostream& operator<<(std::ostream& os, const MouseMoveEvent& ev) {
        return (os << "Mouse position: (" << ev.x() << "," << ev.y() << ")");
    }

private:
    double xpos;
    double ypos;
};

/*
====================
MousePressEvent
====================
*/
class MousePressEvent : public Event {
public:
    MousePressEvent(int button)
        : mouse_button{ button } { }

    EVENT_CLASS_TYPE(EventType::WINDOW_MOUSE_PRESS_EVENT)

    double button() const { return mouse_button; }

    friend std::ostream& operator<<(std::ostream& os, const MousePressEvent& ev) {
        return (os << "Mouse button pressed: (" << ev.button() << ")");
    }

private:
    int mouse_button;
};

/*
====================
MouseReleaseEvent
====================
*/
class MouseReleaseEvent : public Event {
public:
    MouseReleaseEvent(int button)
        : mouse_button{ button } {
    }

    EVENT_CLASS_TYPE(EventType::WINDOW_MOUSE_RELEASE_EVENT)

    double button() const { return mouse_button; }

    friend std::ostream& operator<<(std::ostream& os, const MouseReleaseEvent& ev) {
        return (os << "Mouse button released: (" << ev.button() << ")");
    }

private:
    int mouse_button;
};
