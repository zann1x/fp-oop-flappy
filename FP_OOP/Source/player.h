#pragma once

#include "camera.h"
#include "input_manager.h"
#include "render_manager.h"
#include "shader.h"
#include "shader_manager.h"
#include "texture_2d.h"
#include "texture_manager.h"
#include "vertex_array.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/mat4.h"

#include <glad/glad.h>

#include <iostream>
#include <vector>

class Player {
public:
    Player(Vec2 position, float rotation = 0.0f, float fallrate = 0.0f)
        : _class_name{ "player" },
          _position{ position }, _rotation{ rotation }, _size{ 0.5f }, _fallrate{ fallrate },
          _FALLSPEED{ -0.01f }, _RISESPEED{ 0.1f } {
        static GLfloat vertices[] = {
            -_size, -_size, 2.0f,
             _size, -_size, 2.0f,
             _size,  _size, 2.0f,
            -_size,  _size, 2.0f
        };
        static GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        static GLuint texture_coordinates[] = {
            0, 1,
            1, 1,
            1, 0,
            0, 0
        };

        InputManager::get_instance().set_rise_com(std::bind(&Player::rise, this));

        RenderManager::get_instance().load(_class_name, vertices, indices, texture_coordinates);

        const Texture2D& texture = TextureManager::get_instance().load(_class_name, 0, "Content\\bird.png");

        const Shader& shader = ShaderManager::get_instance().load(_class_name,
            "Source\\shaders\\player.vert", "Source\\shaders\\player.frag");
        shader.bind();
        shader.set_uniform1i("texture_pos", texture.slot());
    }

    Vec2 position(void) const {
        return _position;
    }

    float rotation(void) const {
        return _rotation;
    }

    float fallrate(void) const {
        return _fallrate;
    }

    void rise(void) {
        _fallrate = _RISESPEED;
    }

#ifdef _RUN_FUNCTIONAL
    std::unique_ptr<Player> updated(void) const {
        const float fallrate = _fallrate + _FALLSPEED;
        Vec2 position = _position.with_y(_position.y() + _fallrate);

        if (position.y() < Camera::dimensions().bottom + _size * 0.5f) {
            EventBus::get_instance().dispatch(GameLossEvent());
            return std::make_unique<Player>(position.with_y(Camera::dimensions().bottom + _size * 0.5f), 90.0f * fallrate, fallrate);
        }

        return std::make_unique<Player>(position, 90.0f * fallrate, fallrate);
    }
#else
    void update(void) {
        _fallrate += _FALLSPEED;
        _position = _position.with_y(_position.y() + _fallrate);
        _rotation = 90.0f * _fallrate;

        if (_position.y() < Camera::dimensions().bottom + _size * 0.5f) {
            _position = _position.with_y(Camera::dimensions().bottom + _size * 0.5f);

            EventBus::get_instance().dispatch(GameLossEvent());
        }
    }
#endif

    void render(void) const {
        const Shader& shader = ShaderManager::get_instance().shader(_class_name);
        shader.bind();
        const VertexArray& entity = RenderManager::get_instance().entity(_class_name);
        entity.bind();
        TextureManager::get_instance().texture(_class_name).bind();

        Mat4 proj = Camera::projection();
        Mat4 view = Mat4::identity();
        Mat4 model = Mat4::translation(Vec3(_position)) * Mat4::rotation(_rotation);
        shader.set_uniform_mat4("mvp", proj * view * model);

        entity.draw();
    }

private:
    const char* _class_name;

#ifdef _RUN_FUNCTIONAL
    const Vec2 _position;
#else
    Vec2 _position;
#endif
    float _rotation;
    GLfloat _size;
    float _fallrate;
    const float _FALLSPEED;
    const float _RISESPEED;
};
