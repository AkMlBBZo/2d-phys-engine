#ifndef MAGNETIC_POINT_ENGINE_H
#define MAGNETIC_POINT_ENGINE_H

#include "Point.h"
#include "engine/physics/MagnetPole.h"
#include "engine/physics/MaterialType.h"

namespace engine::objects {

class MagneticPoint: public Point {
private:
    physics::MagnetPole m_pole;
    physics::MaterialType m_material;
    float m_magnetic_strength;
    float m_magnetic_radius;

public:
    MagneticPoint(physics::MagnetPole pole,        //
                  physics::MaterialType material,  //
                  float magnetic_strength,         //
                  float magnetic_radius,           //
                  const math::Vector2& pos,        //
                  float mass      = 1.0f,          //
                  float radius    = 1.0f,          //
                  bool is_pinned  = false,         //
                  sf::Color color = engine::colors::MAGENTA);

    void applyMagneticInteraction(MagneticPoint& other);
};

}  // namespace engine::objects

#endif  // MAGNETIC_POINT_ENGINE_H