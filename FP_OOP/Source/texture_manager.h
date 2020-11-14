#pragma once

#include "texture_2d.h"

#include <unordered_map>

class TextureManager {
public:
    static TextureManager& get_instance(void) {
        static TextureManager _instance;
        return _instance;
    }

    const Texture2D& load(const char* name, uint8_t slot, const char* texture_path) {
        if (_textures.find(name) == _textures.end()) {
            _textures.insert({ name, std::make_unique<Texture2D>(slot, texture_path) });
        }
        return *_textures.at(name);
    }

    const Texture2D& texture(const char* name) const {
        return *_textures.at(name);
    }

private:
    TextureManager(void) {}
    ~TextureManager(void) {}

private:
    std::unordered_map<const char*, std::unique_ptr<const Texture2D>> _textures;
};
