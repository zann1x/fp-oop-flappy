#pragma once

#include "keycode.h"
#include "event.h"

#include "log.h"

class Player;

class InputManager {
private:
    InputManager(void);
    ~InputManager(void);

public:
    using RiseComFun = std::function<void(void)>;

public:
    static InputManager& get_instance(void);
    void set_rise_com(RiseComFun fun);
    void handle_event(const Event& event);
    void dispatch_commands(void) const;

private:
    static InputManager _instance;

    bool pressed_keys[1024];

    RiseComFun _rise_com;
};
