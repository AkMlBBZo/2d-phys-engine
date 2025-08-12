#include "engine/core/PhysicsEngine.h"

#include <cmath>
#include <stdexcept>

namespace engine::core {

PhysicsEngine::PhysicsEngine(float g) : m_g(g), renderer(Renderer()), m_clock(sf::Clock()) {
    if (!std::isfinite(g) || g < 0) {
        throw std::invalid_argument("Invalid gravity value: must be a positive number or zero");
    }
}

void PhysicsEngine::addPoint(objects::Point &point) {
    m_points.push_back(std::make_unique<objects::Point>(point));
}

void PhysicsEngine::addPoint(objects::MagneticPoint &point) {
    m_magnetic_points.push_back(std::make_unique<objects::MagneticPoint>(point));
}

void PhysicsEngine::applyGravity() {
    for (auto &point : m_points) {
        point->applyForce({0, m_g});
    }
    for (auto &point : m_magnetic_points) {
        point->applyForce({0, m_g});
    }
}

void PhysicsEngine::applyMagnetic() {
    for (size_t i = 0; i < m_magnetic_points.size(); ++i) {
        auto &point_i = *m_magnetic_points[i];
        for (size_t j = i + 1; j < m_magnetic_points.size(); ++j) {
            point_i.applyMagneticInteraction(*m_magnetic_points[j]);
        }
    }
}

void PhysicsEngine::update() {
    float dt = m_clock.restart().asSeconds();
    for (auto &point : m_points) {
        point->update(dt);
    }
    for (auto &point : m_magnetic_points) {
        point->update(dt);
    }
}

void PhysicsEngine::render() {
    renderer.render(m_points, m_magnetic_points);
}

bool PhysicsEngine::isRunning() const {
    return renderer.isWindowOpen();
}

}  // namespace engine::core
