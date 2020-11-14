#pragma once

#include <glad/glad.h>

#include <vector>

class VertexArray {
public:
    VertexArray(GLfloat vertices[], GLuint indices[], GLuint texture_coords[])
        : _vao{ 0 }, _vbo{ 0 }, _ibo{ 0 }, _tbo{ 0 } {
        glGenVertexArrays(1, &_vao);

        glBindVertexArray(_vao);

        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glGenBuffers(1, &_tbo);
        glBindBuffer(GL_ARRAY_BUFFER, _tbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLuint), texture_coords, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_UNSIGNED_INT, GL_FALSE, 0, (void*)0);

        glGenBuffers(1, &_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~VertexArray(void) {
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ibo);
        glDeleteVertexArrays(1, &_vao);
    }

    void bind(void) const {
        glBindVertexArray(_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    }

    void unbind(void) const {
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void draw(void) const {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

private:
    GLuint _vao;
    GLuint _vbo;
    GLuint _tbo;
    GLuint _ibo;
};
