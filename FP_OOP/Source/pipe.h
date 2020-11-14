#pragma once

#include "camera.h"
#include "render_manager.h"
#include "shader.h"
#include "shader_manager.h"
#include "texture_2d.h"
#include "texture_manager.h"
#include "vertex_array.h"
#include "math/mat4.h"
#include "math/vec2.h"
#include "math/vec3.h"

#include <glad/glad.h>

class Pipe {
public:
    Pipe(Vec2 position = Vec2(0.0f, 0.0f), float rotation = 0.0f, Vec2 render_offset = Vec2(0.0f, 0.0f), int32_t xscroll = 0)
        : _class_name{ "pipe" }, _SIZE{ 1.5f, 10.0f }, _position{ position }, _rotation{ rotation }, _render_offset{ render_offset } {
        static GLfloat vertices[] = {
                 0.0f, -_SIZE.y(), 3.0f,
            _SIZE.x(), -_SIZE.y(), 3.0f,
            _SIZE.x(),       0.0f, 3.0f,
                 0.0f,       0.0f, 3.0f
        };
        static GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        static GLuint texture_coords[] = {
            0, 1,
            1, 1,
            1, 0,
            0, 0
        };

        RenderManager::get_instance().load(_class_name, vertices, indices, texture_coords);
        const Texture2D& texture = TextureManager::get_instance().load(_class_name, 0, "Content\\pipe.png");
        const Shader& shader = ShaderManager::get_instance().load(_class_name,
            "Source\\shaders\\pipe.vert", "Source\\shaders\\pipe.frag");

        shader.bind();
        shader.set_uniform1i("texture_pos", texture.slot());
    }

    static Pipe generate_pipe(void) {

    }

    std::unique_ptr<Pipe> with_render_offset(Vec2 render_offset) {
        return std::make_unique<Pipe>(_position, _rotation, render_offset);
    }

    std::unique_ptr<Pipe> with_position(Vec2 position) {
        return std::make_unique<Pipe>(position, _rotation, _render_offset);
    }

    Vec2 position(void) const { return _position; }

    void update(void) { }

    void render(void) const {
        const Shader& shader = ShaderManager::get_instance().shader(_class_name);
        shader.bind();
        const VertexArray& entity = RenderManager::get_instance().entity(_class_name);
        entity.bind();
        TextureManager::get_instance().texture(_class_name).bind();

        Mat4 proj = Camera::projection();
        Mat4 view = Mat4::identity();
        Mat4 model = Mat4::translation(Vec3(_position + _render_offset)) * Mat4::rotation(_rotation);
        shader.set_uniform_mat4("mvp", proj * view * model);
        entity.draw();
    }

    void move_to(Vec2 position) {
        _position = position;
    }

    void set_render_offset(Vec2 offset) {
        _render_offset = offset;
    }

    static std::tuple<Vec2, Vec2> generate_pipe_position(int32_t val) {
        float x = 5.0f + 2.5f * val;
        float y = (float)(rand() % 6 - 4);
        return std::make_tuple<Vec2, Vec2>(
            Vec2(x, y),
            Vec2(x + 1.5f, y + 3.0f));
    }

private:
    const char* _class_name;

    const Vec2 _SIZE;
    Vec2 _position;
    float _rotation;
    Vec2 _render_offset;
};
