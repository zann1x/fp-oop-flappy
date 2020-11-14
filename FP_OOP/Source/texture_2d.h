#pragma once

#include <glad/glad.h>

#include <stb_image.h>

#include <stdexcept>

class Texture2D {
public:
    Texture2D(uint8_t slot, const char* filepath)
        : _slot{ slot }, _path{ filepath } {
        glGenTextures(1, &_texture_id);
        glBindTexture(GL_TEXTURE_2D, _texture_id);
        
        glActiveTexture(GL_TEXTURE0 + slot);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int x, y, channels;
        stbi_uc* data = stbi_load(filepath, &x, &y, &channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            std::cout << stbi_failure_reason() << std::endl;
            throw std::runtime_error("Could not load file");
        }
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~Texture2D(void) {
        glDeleteTextures(1, &_texture_id);
    }

    void bind(void) const {
        glActiveTexture(GL_TEXTURE0 + _slot);
        glBindTexture(GL_TEXTURE_2D, _texture_id);
    }

    void unbind(void) const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    uint8_t slot(void) const {
        return _slot;
    }

private:
    GLuint _texture_id;
    const uint8_t _slot;
    const char* _path;
};
