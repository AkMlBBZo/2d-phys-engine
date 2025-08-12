#include "engine/math/EngineVector2.h"

#include <cmath>
#include <stdexcept>

namespace engine::math {

const Vector2 Vector2::ZERO{0, 0};
const Vector2 Vector2::ONE{1, 1};
const Vector2 Vector2::UP{0, 1};
const Vector2 Vector2::DOWN{0, -1};
const Vector2 Vector2::LEFT{-1, 0};
const Vector2 Vector2::RIGHT{1, 0};

Vector2::Vector2(const float x, const float y) : x(x), y(y) {
    if (!std::isfinite(x) || !std::isfinite(y))
        throw std::invalid_argument("Vector2: Non-finite value detected");
}

Vector2 Vector2::lerp(const Vector2& start, const Vector2& end, float t) {
    return start + (end - start) * t;
}

float Vector2::lengthSquared() const noexcept {
    return this->x * this->x + this->y * this->y;
}

float Vector2::length() const noexcept {
    return std::hypot(this->x, this->y);
}

float Vector2::distance(const Vector2& v1, const Vector2& v2) noexcept {
    return (v2 - v1).length();
}

Vector2 Vector2::normalize() const noexcept {
    float length = this->length();
    return length == 0 ? ZERO : *this / length;
}

float Vector2::angle() const noexcept {
    return *this == ZERO ? 0 : std::atan2(y, x);
}

Vector2 Vector2::fromAngle(const float angle) noexcept {
    return Vector2(cos(angle), sin(angle));
}

Vector2 Vector2::rotated(const float angle) const noexcept {
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);
    return Vector2(x * cos - y * sin, x * sin + y * cos);
}

Vector2 Vector2::operator+(const Vector2& v) const noexcept {
    return Vector2(this->x + v.x, this->y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v) const noexcept {
    return Vector2(this->x - v.x, this->y - v.y);
}

Vector2 Vector2::operator-() const noexcept {
    return Vector2(-this->x, -this->y);
}

Vector2 Vector2::operator*(const float scalar) const noexcept {
    return Vector2(this->x * scalar, this->y * scalar);
}

Vector2 Vector2::operator*(const Vector2& v) const noexcept {
    return Vector2(this->x * v.x, this->y * v.y);
}
Vector2 Vector2::operator/(const float scalar) const {
    if (!std::isfinite(scalar))
        throw std::runtime_error("divide by Non-finite value");
    if (scalar == 0)
        throw std::runtime_error("divide by 0");

    return Vector2(this->x / scalar, this->y / scalar);
}

bool Vector2::operator==(const Vector2& v) const noexcept {
    return this->x == v.x && this->y == v.y;
}

bool Vector2::operator!=(const Vector2& v) const noexcept {
    return !(*this == v);
}

Vector2& Vector2::operator+=(const Vector2& v) noexcept {
    return *this = *this + v;
}

Vector2& Vector2::operator-=(const Vector2& v) noexcept {
    return *this = *this - v;
}

Vector2& Vector2::operator*=(const float scalar) noexcept {
    return *this = *this * scalar;
}

Vector2& Vector2::operator/=(const float scalar) {
    return *this = *this / scalar;
}

float Vector2::dot(const Vector2& v1, const Vector2& v2) noexcept {
    return v1.x * v2.x + v1.y * v2.y;
}

float Vector2::dot(const Vector2& oth) const noexcept {
    return dot(*this, oth);
}

float Vector2::cross(const Vector2& v1, const Vector2& v2) noexcept {
    return v1.x * v2.y - v1.y * v2.x;
}

float Vector2::cross(const Vector2& oth) const noexcept {
    return cross(*this, oth);
}

Vector2 Vector2::project(const Vector2& v1, const Vector2& v2) {
    return v2 * (dot(v1, v2) / v2.lengthSquared());
}

Vector2 Vector2::project(const Vector2& oth) const {
    return project(*this, oth);
}

Vector2 Vector2::perpendicular(const Vector2& v) noexcept {
    return Vector2(-v.y, v.x);
}

Vector2 Vector2::perpendicular() const noexcept {
    return perpendicular(*this);
}

bool Vector2::approachEquals(const Vector2& v1, const Vector2& v2, float delta) noexcept {
    return (v1 - v2).lengthSquared() <= delta * delta;
}

bool Vector2::isZero() const noexcept {
    return *this == ZERO;
}

Vector2::operator sf::Vector2f() const {
    return sf::Vector2f({x, y});
}

}  // namespace engine::math