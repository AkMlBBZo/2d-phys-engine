#include "engine/objects/MagneticPoint.h"

#include <stdexcept>

#include "engine/physics/Interactions.h"

namespace engine::objects {

MagneticPoint::MagneticPoint(physics::MagnetPole pole, physics::MaterialType material, float magnetic_strength,
                             float magnetic_radius, const math::Vector2& pos, float mass, float radius, bool is_pinned,
                             sf::Color color)
    : Point(pos, mass, radius, is_pinned, color)  //
    , m_pole(pole)                                //
    , m_material(material)                        //
    , m_magnetic_strength(magnetic_strength)      //
    , m_magnetic_radius(magnetic_radius) {
    if (m_magnetic_strength < 0.0f)
        throw std::invalid_argument("Magnetic strength cannot be negative");
    if (m_magnetic_radius < 0.0f)
        throw std::invalid_argument("Magnetic radius cannot be negative");
    if (m_magnetic_radius < radius)
        throw std::invalid_argument("Magnetic radius (" + std::to_string(m_magnetic_radius) +
                                    ") cannot be less than collision radius (" + std::to_string(radius) + ")");
}

void MagneticPoint::applyMagneticInteraction(MagneticPoint& other) {
    if (!physics::Interactions::magneticInteraction(this->m_pole, this->m_material) ||
        !physics::Interactions::magneticInteraction(other.m_pole, other.m_material) ||
        !physics::Interactions::haveMagneticField(this->m_pole) &&
            !physics::Interactions::haveMagneticField(other.m_pole))
        return;

    math::Vector2 delta = other.getPosition() - getPosition();
    float distance_sq   = delta.lengthSquared();

    if (distance_sq > this->m_magnetic_radius * this->m_magnetic_radius ||
        distance_sq > other.m_magnetic_radius * other.m_magnetic_radius || distance_sq < 0.0001f)
        return;

    float k                  = 1000.0f;
    float materialMultiplier = physics::Interactions::getMaterialMultiplier(this->m_material) *
                               physics::Interactions::getMaterialMultiplier(other.m_material);
    float sign = 1.0f;
    if (m_pole != physics::MagnetPole::NONE && other.m_pole != physics::MagnetPole::NONE) {
        sign = (m_pole == other.m_pole) ? -1.0f : 1.0f;
    }

    float strengthA = (physics::Interactions::isMetall(this->m_material)) ? this->m_magnetic_strength * 0.5f
                                                                          : this->m_magnetic_strength;
    float strengthB = (physics::Interactions::isMetall(other.m_material)) ? other.m_magnetic_strength * 0.5f
                                                                          : other.m_magnetic_strength;

    float forceMagnitude = sign * k * materialMultiplier * (strengthA * strengthB) / (distance_sq);

    math::Vector2 force = delta.normalize() * forceMagnitude;
    applyForce(force);
    other.applyForce(-force);
}

}  // namespace engine::objects
