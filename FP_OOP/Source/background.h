#pragma once

#include "camera.h"
#include "render_manager.h"
#include "shader.h"
#include "shader_manager.h"
#include "texture_2d.h"
#include "texture_manager.h"
#include "vertex_array.h"
#include "math/vec2.h"

#include <iostream>

class Background {
public:
    Background(Vec2 position, Vec2 render_offset)
        : _class_name{ "background" }, _position{ position }, _render_offset{ render_offset } {
        static GLfloat vertices[] = {
            Camera::dimensions().left, Camera::dimensions().bottom, 4.0f,
            0.0f,                      Camera::dimensions().bottom, 4.0f,
            0.0f,                      Camera::dimensions().top,    4.0f,
            Camera::dimensions().left, Camera::dimensions().top,    4.0f
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

        RenderManager::get_instance().load(_class_name, vertices, indices, texture_coordinates);
        const Texture2D& texture = TextureManager::get_instance().load(_class_name, 0, "Content\\background.png");
        const Shader& shader = ShaderManager::get_instance().load(_class_name,
            "Source\\shaders\\background.vert", "Source\\shaders\\background.frag");

        shader.bind();
        shader.set_uniform1i("texture_pos", texture.slot());
    }

    ~Background(void) {}

    std::unique_ptr<Background> with_render_offset(Vec2 render_offset) const {
        return std::make_unique<Background>(_position, render_offset);
    }

    void set_render_offset(Vec2 offset) {
        _render_offset = offset;
    }

    void update() { }

    void render(void) const {
        const Shader& shader = ShaderManager::get_instance().shader(_class_name);
        shader.bind();
        const VertexArray& entity = RenderManager::get_instance().entity(_class_name);
        entity.bind();
        TextureManager::get_instance().texture(_class_name).bind();

        Mat4 proj = Camera::projection();
        Mat4 view = Mat4::identity();
        Mat4 model = Mat4::translation(Vec3(_position + _render_offset));
        shader.set_uniform_mat4("mvp", proj * view * model);

        entity.draw();
    }

    void move_to(const Vec2& position) {
        _position = position;
    }

private:
    const char* _class_name;
    Vec2 _position;
    Vec2 _render_offset;
};
