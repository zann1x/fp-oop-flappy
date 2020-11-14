#pragma once

#include "math/mat4.h"

/*
====================
Dimensions
====================
*/
struct Dimensions {
    float left      = -10.0f;
    float right     = 10.0f;
    float top       = 10.0f * 9.0f / 16.0f;
    float bottom    = -10.0f * 9.0f / 16.0f;
    float far       = -5.0f; // when normalized, this will actually become positive 5
    float near      = 1.0f; // when normalized, this will actually become negative 1
};

/*
====================
Camera
====================
*/
class Camera {
public:
    static Dimensions dimensions(void) {
        return _dimensions;
    }

    static Mat4 projection(void) {
        static Mat4 projection_matrix = Mat4::orthographic(
            _dimensions.left,   _dimensions.right,
            _dimensions.bottom, _dimensions.top,
            _dimensions.near,   _dimensions.far
        );
        return projection_matrix;
    }

private:
    static Dimensions _dimensions;
};
