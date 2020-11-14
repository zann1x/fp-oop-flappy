#pragma once

#include "vertex_array.h"

#include <unordered_map>

class RenderManager {
public:
    static RenderManager& get_instance() {
        static RenderManager _instance;
        return _instance;
    }

    const VertexArray& load(const char* name, GLfloat vertices[], GLuint indices[], GLuint texture_coords[]) {
        if (_entities.find(name) == _entities.end()) {
            _entities.insert({ name, std::make_unique<VertexArray>(vertices, indices, texture_coords) });
        }
        return *_entities.at(name);
    }

    const VertexArray& entity(const char* name) const {
        return *_entities.at(name);
    }

private:
    RenderManager() {}
    ~RenderManager() {}

private:
    std::unordered_map<const char*, std::unique_ptr<const VertexArray>> _entities;
};
