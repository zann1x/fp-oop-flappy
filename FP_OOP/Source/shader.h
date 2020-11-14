#pragma once

#include "file_util.h"
#include "math/mat4.h"

#include <glad/glad.h>

#include <string>
#include <vector>

class Shader {
public:
    Shader(const char* vertex_path, const char* fragment_path) {
        std::string vertex_string = FileUtil::load_as_string(vertex_path);
        std::string fragment_string = FileUtil::load_as_string(fragment_path);
        const GLchar* vertex_source = vertex_string.c_str();
        const GLchar* fragment_source = fragment_string.c_str();

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

        GLint result;
        int info_log_length;

        glShaderSource(vertex, 1, &vertex_source, nullptr);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &info_log_length);
        if (result == GL_FALSE) {
            throw std::runtime_error("Could not compile vertex shader");
        }

        glShaderSource(fragment, 1, &fragment_source, nullptr);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &info_log_length);
        if (result == GL_FALSE) {
            throw std::runtime_error("Could not compile fragment shader");
        }

        _program = glCreateProgram();
        glAttachShader(_program, vertex);
        glAttachShader(_program, fragment);

        glLinkProgram(_program);
        glValidateProgram(_program);
        glGetProgramiv(_program, GL_LINK_STATUS, &result);
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &info_log_length);
        if (result == GL_FALSE) {
            throw std::runtime_error("Could not link program");
        }

        glDetachShader(_program, vertex);
        glDetachShader(_program, fragment);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    ~Shader(void) {
        glDeleteProgram(_program);
    }

    void bind(void) const {
        glUseProgram(_program);
    }

    void unbind(void) const {
        glUseProgram(0);
    }

    GLint get_uniform_location(const GLchar* name) const {
        return glGetUniformLocation(_program, name);
    }

    void set_uniform1i(const GLchar* name, GLint val) const {
        GLint location = get_uniform_location(name);
        glUniform1i(location, val);
    }

    void set_uniform_mat4(const GLchar* name, const Mat4& matrix) const {
        GLint location = get_uniform_location(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix.elements().data());
    }

private:
    GLuint _program;
};
