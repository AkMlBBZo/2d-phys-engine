#ifndef VECTOR2_ENGINE_H
#define VECTOR2_ENGINE_H

#include <SFML/System/Vector2.hpp>

namespace engine::math {

class Vector2 {
public:
    float x;
    float y;

    Vector2(const float x = 0, const float y = 0);

    static Vector2 lerp(const Vector2& start, const Vector2& end, float t);

    // static inline constexpr Vector2 ZERO{0, 0};
    static const Vector2 ZERO;
    static const Vector2 ONE;
    static const Vector2 UP;
    static const Vector2 DOWN;
    static const Vector2 LEFT;
    static const Vector2 RIGHT;

    float lengthSquared() const noexcept;
    float length() const noexcept;
    static float distance(const Vector2& v1, const Vector2& v2) noexcept;
    Vector2 normalize() const noexcept;

    float angle() const noexcept;
    static Vector2 fromAngle(const float angle) noexcept;
    Vector2 rotated(const float angle) const noexcept;

    Vector2 operator+(const Vector2& v) const noexcept;
    Vector2 operator-(const Vector2& v) const noexcept;
    Vector2 operator-() const noexcept;
    Vector2 operator*(const float scalar) const noexcept;
    Vector2 operator*(const Vector2& v) const noexcept;
    Vector2 operator/(const float scalar) const;

    bool operator==(const Vector2& v) const noexcept;
    bool operator!=(const Vector2& v) const noexcept;

    Vector2& operator+=(const Vector2& v) noexcept;
    Vector2& operator-=(const Vector2& v) noexcept;
    Vector2& operator*=(const float scalar) noexcept;
    Vector2& operator/=(const float scalar);

    static float dot(const Vector2& v1, const Vector2& v2) noexcept;
    float dot(const Vector2& oth) const noexcept;
    static float cross(const Vector2& v1, const Vector2& v2) noexcept;
    float cross(const Vector2& oth) const noexcept;
    static Vector2 project(const Vector2& v1, const Vector2& v2);
    Vector2 project(const Vector2& oth) const;
    static Vector2 perpendicular(const Vector2& v) noexcept;
    Vector2 perpendicular() const noexcept;

    static bool approachEquals(const Vector2& v1, const Vector2& v2, float delta) noexcept;
    bool isZero() const noexcept;

    operator sf::Vector2f() const;
};

}  // namespace engine::math

#endif  // VECTOR2_ENGINE_H