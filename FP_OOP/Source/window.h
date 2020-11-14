#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "event.h"

#include <string>

/*
====================
Window
====================
*/
class Window {
public:
    using EventCallbackFunction = std::function<void(const Event&)>;

    struct Data {
        std::string name;
        int width;
        int height;

        EventCallbackFunction callback_function;
    };

public:
    Window(const Data& window_data)
        : _data{ window_data } {
        if (!glfwInit()) {
            throw std::runtime_error("Could not initialize GLFW");
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        _handle = glfwCreateWindow(_data.width, _data.height, _data.name.c_str(), nullptr, nullptr);
        if (!_handle) {
            throw std::runtime_error("Could not create window");
        }

        glfwMakeContextCurrent(_handle);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Could not load Glad");
        }
        glfwSwapInterval(0);
        glfwSetWindowUserPointer(_handle, &_data);

        glfwSetWindowCloseCallback(_handle, [](GLFWwindow* window)->void {
            WindowCloseEvent window_event;
            const Data* data = (Data*)glfwGetWindowUserPointer(window);
            data->callback_function(window_event);
        });
        glfwSetWindowSizeCallback(_handle, [](GLFWwindow* window, int width, int height)->void {
            Data* data = (Data*)glfwGetWindowUserPointer(window);
            WindowResizeEvent event(width, height);
            data->width = width;
            data->height = height;
            data->callback_function(event);
        });
        glfwSetFramebufferSizeCallback(_handle, [](GLFWwindow* window, int width, int height)->void {
            glViewport(0, 0, width, height);
        });
        glfwSetKeyCallback(_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)->void {
            const Data* data = (Data*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressEvent press_event(static_cast<KeyCode>(key));
                    data->callback_function(press_event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleaseEvent release_event(static_cast<KeyCode>(key));
                    data->callback_function(release_event);
                    break;
                }
            }
        });
        glfwSetMouseButtonCallback(_handle, [](GLFWwindow* window, int button, int action, int mods)->void {
            const Data* data = (Data*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    MousePressEvent press_event(button);
                    data->callback_function(press_event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseReleaseEvent release_event(button);
                    data->callback_function(release_event);
                    break;
                }
            }
        });
        glfwSetCursorPosCallback(_handle, [](GLFWwindow* window, double xpos, double ypos)->void {
            MouseMoveEvent move_event(xpos, ypos);
            const Data* data = (Data*)glfwGetWindowUserPointer(window);
            data->callback_function(move_event);
        });

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glEnable(GL_DEPTH_TEST);
    }

    ~Window() {
        glfwTerminate();
    }

    void update_render_stats(const std::string& stats) const {
        glfwSetWindowTitle(_handle, (_data.name + " - " + stats).c_str());
    }

    void poll_events(void) const {
        glfwPollEvents();
    }

    void swap_buffers(void) const {
        glfwSwapBuffers(_handle);
    }

private:
    GLFWwindow* _handle;
    Data _data;
};
