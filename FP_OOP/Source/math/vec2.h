#pragma once

class Vec2 {
public:
    Vec2(void)
        : _x{ 0.0f }, _y{ 0.0f } { }

    Vec2(float x, float y)
        : _x{ x }, _y{ y } { }

    float x(void) const { return _x; }
    float y(void) const { return _y; }

    Vec2 with_x(float new_x) const { return new_x == _x ? *this : Vec2{ new_x, _y }; }
    Vec2 with_y(float new_y) const { return new_y == _y ? *this : Vec2{ _x, new_y }; }

    Vec2 operator+(const Vec2& vec) const {
        return Vec2(
            _x + vec.x(),
            _y + vec.y()
        );
    }

    Vec2 operator-(const Vec2& vec) const {
        return Vec2(
            _x - vec.x(),
            _y - vec.y()
        );
    }

    Vec2 operator*(float val) const {
        return Vec2(
            _x * val,
            _y * val
        );
    }

    Vec2 normalized(void) const {
        float length = sqrtf((_x * _x) + (_y * _y));

        return Vec2(
            _x / length,
            _y / length
        );
    }

private:
    float _x;
    float _y;
};
