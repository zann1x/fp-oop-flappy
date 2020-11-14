#pragma once

#include <cstring>
#include <algorithm>
#include <array>
#define _USE_MATH_DEFINES
#include <math.h>

#include "vec2.h"
#include "vec3.h"

/*
====================
mat4 (column major)
====================
*/
class Mat4 {
public:
    Mat4(void)
        : _elements{ 0.0f } { }

    Mat4(float diagonal)
        : _elements{ 0.0f } {
        _elements[0 + 0 * 4] = diagonal;
        _elements[1 + 1 * 4] = diagonal;
        _elements[2 + 2 * 4] = diagonal;
        _elements[3 + 3 * 4] = diagonal;
    }

    Mat4(const float arr[]) {
        memcpy(_elements.data(), arr, 16 * sizeof(float));
    }

   std::array<float, 16> elements(void) const { return _elements; }

    static const Mat4 identity(void) {
        return Mat4(1.0f);
    }

    static Mat4 translation(const Vec2& vec) {
        return translation(Vec3(vec.x(), vec.y(), 0.0f));
    }

    static Mat4 translation(const Vec3& vec) {
        Mat4 result(1.0f);

        result._elements[0 + 3 * 4] = vec.x();
        result._elements[1 + 3 * 4] = vec.y();
        result._elements[2 + 3 * 4] = vec.z();

         return result;
    }

    static Mat4 rotation(float angle) {
        Mat4 result(1.0f);

        float rotation = (angle * M_PI) / 180.0f;
        float cosine = cos(rotation);
        float sine = sin(rotation);

        result._elements[0 + 0 * 4] = cosine;
        result._elements[1 + 0 * 4] = sine;

        result._elements[0 + 1 * 4] = -sine;
        result._elements[1 + 1 * 4] = cosine;

        return result;
    }

    static Mat4 orthographic(float left, float right, float bottom, float top, float near, float far) {
        Mat4 result(1.0f);

        result._elements[0 + 0 * 4] = 2.0f / (right - left);

        result._elements[1 + 1 * 4] = 2.0f / (top - bottom);

        result._elements[2 + 2 * 4] = -2.0f / (far - near);

        result._elements[0 + 3 * 4] = -(right + left) / (right - left);
        result._elements[1 + 3 * 4] = -(top + bottom) / (top - bottom);
        result._elements[2 + 3 * 4] = -(far + near) / (far - near);

        return result;
    }

    static Mat4 look_at(const Vec3& eye, const Vec3& target, const Vec3& up) {
        Mat4 result(1.0f);
        Vec3 zaxis = (eye - target).normalized(); // forward
        Vec3 xaxis = up.crossed(zaxis).normalized(); // right
        Vec3 yaxis = zaxis.crossed(xaxis).normalized(); // up

        result._elements[0 + 0 * 4] = xaxis.x();
        result._elements[0 + 1 * 4] = xaxis.y();
        result._elements[0 + 2 * 4] = xaxis.z();
        result._elements[0 + 3 * 4] = -xaxis.dotted(eye);

        result._elements[1 + 0 * 4] = yaxis.x();
        result._elements[1 + 1 * 4] = yaxis.y();
        result._elements[1 + 2 * 4] = yaxis.z();
        result._elements[1 + 3 * 4] = -yaxis.dotted(eye);

        result._elements[2 + 0 * 4] = zaxis.x();
        result._elements[2 + 1 * 4] = zaxis.y();
        result._elements[2 + 2 * 4] = zaxis.z();
        result._elements[2 + 3 * 4] = -zaxis.dotted(eye);

        return result; 
    }

    const Mat4 operator*(const Mat4& mat) const {
        float data[16] = { 0.0f };
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                float sum = 0.0f;
                for (int e = 0; e < 4; e++) {
                    sum += _elements[x + e * 4] * mat._elements[e + y * 4];
                }
                data[x + y * 4] = sum;
            }
        }

        return Mat4(data);
    }

private:
    std::array<float, 16> _elements;
};
