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
    std::vector<std::shared_ptr<objects::Point>> m_points;
    std::vector<std::shared_ptr<objects::MagneticPoint>> m_magnetic_points;
    std::vector<std::shared_ptr<objects::Line>> m_lines;
    Renderer renderer;
    float m_g;

public:
    PhysicsEngine(float g = 9.8f);
    std::shared_ptr<objects::Point> addPoint(objects::Point& point);
    std::shared_ptr<objects::Point> addPoint(objects::MagneticPoint& point);
    std::shared_ptr<objects::Line> addLine(objects::Line& line);
    std::shared_ptr<objects::Line> addLine(std::shared_ptr<objects::Point>& p1, std::shared_ptr<objects::Point>& p2);
    void applyGravity();
    void applyMagnetic();
    void applyLineStrength();
    void update();

    const std::vector<std::shared_ptr<objects::Point>> getPoints() const;
    std::vector<std::shared_ptr<objects::Point>> getPoints();
    const std::vector<std::shared_ptr<objects::MagneticPoint>> getMagneticPoints() const;
    std::vector<std::shared_ptr<objects::MagneticPoint>> getMagneticPoints();
    const std::vector<std::shared_ptr<objects::Line>> getLines() const;
    std::vector<std::shared_ptr<objects::Line>> getLines();

    void render();
    bool isRunning() const;
};

}  // namespace engine::core

#endif  // PHYSICS_ENGINE_H