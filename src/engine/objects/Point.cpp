#include "engine/objects/Point.h"

#include <stdexcept>

#include "config.h"

namespace engine::objects {

Point::Point(const math::Vector2& pos, float mass, float radius, bool is_pinned, sf::Color color)
    : m_pos(pos)
    , m_prev_pos(pos)
    , m_mass(mass)
    , m_inv_mass(mass != 0 ? 1 / mass : 0)
    , m_radius(radius)
    , m_is_pinned(is_pinned)
    , m_is_active(is_pinned)
    , m_prev_dt(0)
    , m_color(color) {
    if (mass <= 0 && !m_is_pinned)
        throw std::invalid_argument("Non-pinned points must have mass > 0");
    if (radius <= 0)
        throw std::invalid_argument("radius must be greater than 0");
}

void Point::applyForce(const math::Vector2& force) {
    m_forces += force;
}

void Point::update(float dt) {
    if (m_is_pinned || dt <= 0)
        return;

    math::Vector2 temp_pos = m_pos;

    const float delta = (m_prev_dt > 0) ? dt / m_prev_dt * 0.9f : 0.9f;
    m_pos += (m_pos - m_prev_pos) * delta + (m_forces * m_inv_mass) * (dt * dt);

    m_velocity  = (m_pos - m_prev_pos) / dt;
    m_prev_pos  = temp_pos;
    m_prev_dt   = dt;
    m_is_active = math::Vector2::approachEquals(m_velocity, math::Vector2::ZERO, 0.001f);

    if (m_pos.y > Bounds::BOTTOM) {
        m_prev_pos = m_pos;
        m_prev_pos.x -= dt * m_velocity.x * 0.5;
        m_prev_pos.y = Bounds::BOTTOM;
        m_pos.y      = Bounds::BOTTOM - m_velocity.y * dt * 0.3;
    } else if (m_pos.y < Bounds::TOP) {
        m_prev_pos.y = Bounds::TOP + 1;
        m_pos.y      = Bounds::TOP + 1;
    }
    if (m_pos.x > Bounds::RIGHT) {
        m_prev_pos = m_pos;
        m_prev_pos.y -= dt * m_velocity.y * 0.5;
        m_prev_pos.x = Bounds::RIGHT;
        m_pos.x      = Bounds::RIGHT - m_velocity.x * dt * 0.3;
    } else if (m_pos.x < Bounds::LEFT) {
        m_prev_pos = m_pos;
        m_prev_pos.y -= dt * m_velocity.y * 0.5;
        m_prev_pos.x = Bounds::LEFT;
        m_pos.x      = Bounds::LEFT - m_velocity.x * dt * 0.3;
    }

    this->resetForces();
}

void Point::resetForces() {
    m_forces = math::Vector2::ZERO;
}

const math::Vector2& Point::getPosition() const {
    return m_pos;
}

math::Vector2& Point::getPosition() {
    return m_pos;
}

const math::Vector2& Point::getVelocity() const {
    return m_velocity;
}

math::Vector2& Point::getVelocity() {
    return m_velocity;
}

float Point::getMass() const {
    return m_mass;
}

float Point::getInvMass() const {
    return m_inv_mass;
}

float Point::getRadius() const {
    return m_radius;
}

bool Point::isPinned() const {
    return m_is_pinned;
}

bool Point::isActive() const {
    return m_is_active;
}

void Point::setPinned(bool pinned) {
    m_is_pinned = pinned;
}

void Point::setMass(float mass) {
    if (mass <= 0 && !m_is_pinned)
        throw std::invalid_argument("Non-pinned points must have mass > 0");
    m_mass     = mass;
    m_inv_mass = (mass != 0) ? 1 / mass : 0;
}

void Point::setPosition(const math::Vector2& pos) {
    m_pos = pos;
}

void Point::move(const math::Vector2& delta) {
    if (m_is_pinned)
        return;
    m_pos += delta;
}

void Point::teleport(const math::Vector2& new_pos) {
    m_prev_pos  = new_pos;
    m_pos       = new_pos;
    m_velocity  = math::Vector2::ZERO;
    m_is_active = false;
}

const sf::Color& Point::getColor() const {
    return m_color;
}

sf::Color& Point::getColor() {
    return m_color;
}

void Point::setColor(sf::Color& color) {
    this->m_color = color;
}

}  // namespace engine::objects