#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>

#include "functions.h"
#include "keycode.h"
#include "event.h"
#include "player.h"
#include "input_manager.h"
#include "layer.h"
#include "window.h"

/*
====================
Application
====================
*/
class Application {
public:
    Application(void)
        : _window{ Window::Data{ "F(la)P(py) OOP", 1280, 720, std::bind(&EventBus::dispatch, &EventBus::get_instance(), std::placeholders::_1) } },
          _running{ false } {
        EventBus::get_instance().attach(EventType::WINDOW_CLOSE_EVENT, on_event_by_source(EventSource::WINDOW));
        EventBus::get_instance().attach(EventType::WINDOW_RESIZE_EVENT, on_event_by_source(EventSource::WINDOW));

        EventBus::get_instance().attach(EventType::GAME_LOSS_EVENT, on_event_by_source(EventSource::GAME));

        EventBus::get_instance().attach(EventType::WINDOW_MOUSE_MOVE_EVENT, std::bind(&Application::on_mouse, this, std::placeholders::_1));
        EventBus::get_instance().attach(EventType::WINDOW_MOUSE_PRESS_EVENT, std::bind(&Application::on_mouse, this, std::placeholders::_1));
        EventBus::get_instance().attach(EventType::WINDOW_MOUSE_RELEASE_EVENT, std::bind(&Application::on_mouse, this, std::placeholders::_1));

        GameLayer* game_layer = new GameLayer();
        UILayer* ui_layer = new UILayer();
        _layers.push_back(game_layer);
        _layers.push_back(ui_layer);
    }

    ~Application(void) {
        Functions::for_each(_layers, [](Layer* layer)->void {
            delete layer;
        });
    }

    void run(void) {
        _running = true;

        double ms_per_update = 1.0 / 60.0;
        double last_time = glfwGetTime();
        double timer = last_time;
        double lag = 0;
        double delta_time = 0;
        double now_time = 0;
        uint32_t frames = 0;
        uint32_t updates = 0;

        while (_running) {
            now_time = glfwGetTime();
            delta_time = now_time - last_time;
            lag += delta_time;
            last_time = now_time;

            _window.poll_events();

            // Fixed timestep update
            while (lag >= ms_per_update) {
                updates++;
                lag -= ms_per_update;

                InputManager::get_instance().dispatch_commands();
                Functions::for_each(_layers, [](Layer* layer) {
                    layer->on_update();
                });
            }

            ++frames;
            Functions::for_each(_layers, [](Layer* layer) {
                layer->on_render();
            });

            _window.swap_buffers();
            if (glfwGetTime() - timer > 1.0) {
                timer++;
                _window.update_render_stats(std::to_string(frames) + " fps, " + std::to_string(updates) + " updates, " + std::to_string(1000.0 / frames) + " ms/frame");
                updates = 0;
                frames = 0;
            }
        }
    }

    Window::EventCallbackFunction on_event_by_source(const EventSource event_source) {
        return [=](const Event& event) {
            switch (event_source) {
                case EventSource::WINDOW: {
                    if (event.event_type() == EventType::WINDOW_CLOSE_EVENT) {
                        LOG_TRACE("Closing window");
                        _running = false;
                    }
                    break;
                }
                case EventSource::GAME: {
                    if (event.event_type() == EventType::GAME_LOSS_EVENT) {
                        _running = false;
                    }
                    break;
                }
            }
        };
    }

    void on_mouse(const Event& event) {
        if (event.event_type() == EventType::WINDOW_MOUSE_MOVE_EVENT) {
            const MouseMoveEvent& move_event = dynamic_cast<const MouseMoveEvent&>(event);
            std::cout << move_event << std::endl;
        }

        if (event.event_type() == EventType::WINDOW_MOUSE_PRESS_EVENT) {
            const MousePressEvent& press_event = dynamic_cast<const MousePressEvent&>(event);
            std::cout << press_event << std::endl;
        } else if (event.event_type() == EventType::WINDOW_MOUSE_RELEASE_EVENT) {
            const MouseReleaseEvent& release_event = dynamic_cast<const MouseReleaseEvent&>(event);
            std::cout << release_event << std::endl;
        }
    }

private:
    Window _window;
    bool _running;

    std::vector<Layer*> _layers;
};

/*
====================
main
====================
*/
int main(void) {
    Application app;
    app.run();

    return 0;
}
