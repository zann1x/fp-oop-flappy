#pragma once

#include "vec2.h"

class Vec3 {
public:
    Vec3(void)
        : _x{ 0.0f }, _y{ 0.0f }, _z{ 0.0f } { }

    Vec3(float x, float y, float z)
        : _x{ x }, _y{ y }, _z{ z } { }

    Vec3(const Vec2& vec)
        : _x{ vec.x() }, _y{ vec.y() }, _z{ 0.0f } { }

    const float x(void) const { return _x; }
    const float y(void) const { return _y; }
    const float z(void) const { return _z; }

    Vec3 with_x(float new_x) const { return new_x == _x ? *this : Vec3(new_x, _y, _z); }
    Vec3 with_y(float new_y) const { return new_y == _y ? *this : Vec3(_x, new_y, _z); }
    Vec3 with_z(float new_z) const { return new_z == _z ? *this : Vec3(_x, _y, new_z); }

    Vec3 operator+(const Vec3& vec) const {
        return Vec3(
            _x + vec.x(),
            _y + vec.y(),
            _z + vec.z()
        );
    }

    Vec3 operator-(const Vec3& vec) const {
        return Vec3(
            _x - vec.x(),
            _y - vec.y(),
            _z - vec.z()
        );
    }

    Vec3 operator*(float val) const {
        return Vec3(
            _x * val,
            _y * val,
            _z * val
        );
    }

    Vec3 normalized(void) const {
        float length = sqrtf((_x * _x) + (_y * _y) + (_z * _z));

        return Vec3(
            _x / length,
            _y / length,
            _z / length
        );
    }

    Vec3 crossed(const Vec3& vec) const {
        return Vec3(
            _y * vec.z() - _z * vec.y(),
            _z * vec.x() - _x * vec.z(),
            _x * vec.y() - _y * vec.x()
        );
    }

    float dotted(const Vec3& vec) const {
        return _x * vec.x() + _y * vec.y() + _z * vec.z();
    }

private:
    float _x;
    float _y;
    float _z;
};
