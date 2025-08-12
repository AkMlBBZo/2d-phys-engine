#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <SFML/System.hpp>

#include "engine/objects/Line.h"
#include "engine/objects/MagneticPoint.h"
#include "engine/objects/Point.h"
#include "engine/rendering/Colors.h"
#include "engine/rendering/Renderer.h"

namespace engine::core {

class PhysicsEngine {
private:
    sf::Clock m_clock;
    std::vector<std::unique_ptr<engine::objects::Point>> m_points;
    std::vector<std::unique_ptr<engine::objects::MagneticPoint>> m_magnetic_points;
    Renderer renderer;
    float m_g;

public:
    PhysicsEngine(float g = 9.8f);
    void addPoint(objects::Point& point);
    void addPoint(objects::MagneticPoint& point);
    void applyGravity();
    void applyMagnetic();
    void update();

    void render();
    bool isRunning() const;
};

}  // namespace engine::core

#endif  // PHYSICS_ENGINE_H