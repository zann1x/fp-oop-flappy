#pragma once

#include "shader.h"

#include <unordered_map>

class ShaderManager {
public:
    static ShaderManager& get_instance(void) {
        static ShaderManager _instance;
        return _instance;
    }

    const Shader& load(const char* name, const char* vertex_path, const char* fragment_path) {
        if (_shaders.find(name) == _shaders.end()) {
            _shaders.insert({ name, std::make_unique<Shader>(vertex_path, fragment_path) });
        }
        return *_shaders.at(name);
    }

    const Shader& shader(const char* name) const {
        return *_shaders.at(name);
    }

private:
    ShaderManager(void) {}
    ~ShaderManager(void) {}

private:
    std::unordered_map<const char*, std::unique_ptr<const Shader>> _shaders;
};
