#pragma once

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "background.h"
#include "event.h"
#include "pipe.h"
#include "shader.h"
#include "texture_2d.h"
#include "vertex_array.h"

/*
====================
Layer
====================
*/
class Layer {
public:
    virtual void on_update() = 0;
    virtual void on_render() = 0;
};

/*
====================
UILayer
====================
*/
class UILayer : public Layer {
public:
    UILayer() {}
    ~UILayer() {}

    virtual void on_update() override {}
    virtual void on_render() override {};
};

/*
====================
GameLayer
====================
*/
class GameLayer : public Layer {
public:
    GameLayer()
        : _xscroll{ 0 }, _pipe_index{ 0 } {
        srand(time(NULL));

        _player = std::make_unique<Player>(Vec2(0.0f, 0.0f));

        for (uint32_t i = 0; i < 3; i++) {
            _background_tiles.push_back(std::make_unique<Background>(Vec2(static_cast<float>(10 * i), 0.0f), Vec2(0.0f, 0.0f)));
        }
        for (uint32_t i = 0; i < 5 * 2; i += 2) {
            auto[bottom, top] = Pipe::generate_pipe_position(i);
            _pipes.push_back(std::make_unique<Pipe>(Vec2(bottom.x(), bottom.y())));
            _pipes.push_back(std::make_unique<Pipe>(Vec2(top.x(), top.y()), 180.0f));
            _pipe_index += 2;
        }
    }

    ~GameLayer() {}

    virtual void on_update() override {
        double start = 0.0;
        double end = 0.0;

        _xscroll--;

        int32_t background_scroll = _xscroll % 333;

        start = glfwGetTime();
#ifdef _RUN_FUNCTIONAL
        // time: 2.6e-05 ms
        std::transform(_background_tiles.begin(), _background_tiles.end(), _background_tiles.begin(),
            [background_scroll](const std::unique_ptr<Background>& bg)->std::unique_ptr<Background> {
                return bg->with_render_offset(Vec2(0.03003f * background_scroll, 0.0f));
            }
        );  
#else
        // time: 7e-07 ms
        for (size_t i = 0; i < _background_tiles.size(); i++) {
            _background_tiles[i]->set_render_offset(Vec2(0.03003f * background_scroll, 0.0f));
        }
#endif
        end = glfwGetTime();
        //std::cout << (end - start) << std::endl;

        float pipe_scroll = _xscroll * 0.05f;
        start = glfwGetTime();
#ifdef _RUN_FUNCTIONAL
        // time: 4.78e-05 ms
        if (_xscroll < -300 && _xscroll % 100 == 0) {
            auto [bottom, top] = Pipe::generate_pipe_position(_pipe_index);

            _pipes.push_back(_pipes[0]->with_position(bottom));
            _pipes.push_back(_pipes[1]->with_position(top));
            _pipes.erase(_pipes.begin(), _pipes.begin() + 2);

            _pipe_index += 2;
        }
        std::transform(_pipes.begin(), _pipes.end(), _pipes.begin(),
            [pipe_scroll](const std::unique_ptr<Pipe>& pipe)->std::unique_ptr<Pipe> {
                return pipe->with_render_offset(Vec2(pipe_scroll, 0.0f));
            }
        );
#else
        // time: 1.2e-06 ms
        if (_xscroll < -300 && _xscroll % 100 == 0) {
            auto [bottom, top] = Pipe::generate_pipe_position(_pipe_index);
            _pipes[_pipe_index % _pipes.size()]->move_to(bottom);
            _pipes[(_pipe_index + 1) % _pipes.size()]->move_to(top);
            _pipe_index += 2;
        }
        for (uint32_t i = 0; i < _pipes.size(); i += 2) {
            _pipes[i]->set_render_offset(Vec2(pipe_scroll, 0.0f));
            _pipes[i + 1]->set_render_offset(Vec2(pipe_scroll, 0.0f));
        }
#endif
        end = glfwGetTime();
        //std::cout << (end - start) << std::endl;

        Functions::for_each(_background_tiles, [](auto& bg)->void { bg->update(); });
        Functions::for_each(_pipes, [](auto& pipe)->void { pipe->update(); });

        start = glfwGetTime();
#ifdef _RUN_FUNCTIONAL
        // time: 2.1e-05 ms
        _player = _player->updated();
#else
        // time: 1.1e-05 ms
        _player->update();
#endif
        end = glfwGetTime();
        //std::cout << (end - start) << std::endl;
    }

    virtual void on_render() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Functions::for_each(_background_tiles, [](auto& bg)->void { bg->render(); });
        Functions::for_each(_pipes, [](auto& pipe)->void { pipe->render(); });
        _player->render();
    }

    std::unique_ptr<Player>& player(void) { return _player; }

private:
    std::unique_ptr<Player> _player;
    std::vector<std::unique_ptr<Background>> _background_tiles;
    std::vector<std::unique_ptr<Pipe>> _pipes;

    int32_t _xscroll;
    uint32_t _pipe_index;
};
