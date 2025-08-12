#ifndef POINT_ENGINE_H
#define POINT_ENGINE_H

#include "engine/math/EngineVector2.h"
#include "engine/rendering/Colors.h"

namespace engine::objects {

class Point {
private:
    math::Vector2 m_pos;
    math::Vector2 m_prev_pos;
    math::Vector2 m_forces;
    math::Vector2 m_velocity;
    float m_mass;
    float m_inv_mass;
    float m_radius;
    float m_prev_dt;
    bool m_is_pinned;
    bool m_is_active;
    sf::Color m_color;

public:
    Point(const math::Vector2& pos = math::Vector2::ZERO,  //
          float mass               = 1.0f,                 //
          float radius             = 1.0f,                 //
          bool is_pinned           = false,                //
          sf::Color color          = engine::colors::YELLOW);

    virtual ~Point() = default;

    void applyForce(const math::Vector2& force);
    void update(float dt);
    void resetForces();

    const math::Vector2& getPosition() const;
    math::Vector2& getPosition();

    const math::Vector2& getVelocity() const;
    math::Vector2& getVelocity();

    float getMass() const;
    float getInvMass() const;
    float getRadius() const;
    bool isPinned() const;
    bool isActive() const;

    void setPinned(bool pinned = true);
    void setMass(float mass);
    void setPosition(const math::Vector2& pos);

    /**
     * Moves the point by a given offset (delta) while preserving its current velocity.
     * Used for smooth movement with physics (e.g., external forces).
     * @param delta The offset vector in world coordinates (e.g., in pixels/meters).
     * @example
     * point.move(math::Vector2(1.0f, 0.0f)); // Moves the point right by 1 unit
     */
    void move(const math::Vector2& delta);
    /**
     * Instantly teleports the point to a new position and resets velocity to zero.
     * Used for forced relocation (e.g., teleportation, state reset).
     * @param new_pos The target position in world coordinates.
     * @example
     * point.teleport(math::Vector2(0.0f, 0.0f)); // Moves the point to the origin
     */
    void teleport(const math::Vector2& new_pos);

    const sf::Color& getColor() const;
    sf::Color& getColor();
    void setColor(sf::Color& color);
};

}  // namespace engine::objects

#endif  // POINT_ENGINE_H