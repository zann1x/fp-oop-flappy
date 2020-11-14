#include "input_manager.h"

#include "player.h"

InputManager InputManager::_instance;

InputManager::InputManager(void)
    : pressed_keys{ false } {
    EventBus::get_instance().attach(EventType::WINDOW_KEY_PRESS_EVENT, std::bind(&InputManager::handle_event, this, std::placeholders::_1));
    EventBus::get_instance().attach(EventType::WINDOW_KEY_RELEASE_EVENT, std::bind(&InputManager::handle_event, this, std::placeholders::_1));
}

InputManager::~InputManager(void) {}

InputManager& InputManager::get_instance(void) {
    return _instance;
}

void InputManager::set_rise_com(RiseComFun fun) {
    _rise_com = fun;
}

void InputManager::handle_event(const Event& event) {
    if (event.event_type() == EventType::WINDOW_KEY_PRESS_EVENT) {
        const KeyPressEvent& key_event = dynamic_cast<const KeyPressEvent&>(event);
        pressed_keys[static_cast<int>(key_event.key_code())] = true;
        std::cout << key_event << std::endl;
    } else if (event.event_type() == EventType::WINDOW_KEY_RELEASE_EVENT) {
        const KeyReleaseEvent& key_event = dynamic_cast<const KeyReleaseEvent&>(event);
        pressed_keys[static_cast<int>(key_event.key_code())] = false;
        std::cout << key_event << std::endl;
    }
}

void InputManager::dispatch_commands(void) const {
    if (pressed_keys[static_cast<int>(KeyCode::SPACE)]) {
        _rise_com();
    }
}
