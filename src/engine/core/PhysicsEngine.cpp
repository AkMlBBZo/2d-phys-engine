#include "engine/core/PhysicsEngine.h"

#include <cmath>
#include <stdexcept>

#include "PhysicsEngine.h"

namespace engine::core {

PhysicsEngine::PhysicsEngine(float g) : m_g(g), renderer(Renderer()), m_clock(sf::Clock()) {
    if (!std::isfinite(g) || g < 0) {
        throw std::invalid_argument("Invalid gravity value: must be a positive number or zero");
    }
}

std::shared_ptr<objects::Point> PhysicsEngine::addPoint(objects::Point &point) {
    m_points.push_back(std::make_shared<objects::Point>(point));
    return m_points.back();
}

std::shared_ptr<objects::Point> PhysicsEngine::addPoint(objects::MagneticPoint &point) {
    m_magnetic_points.push_back(std::make_shared<objects::MagneticPoint>(point));
    return m_magnetic_points.back();
}

std::shared_ptr<objects::Line> PhysicsEngine::addLine(objects::Line &line) {
    m_lines.push_back(std::make_shared<objects::Line>(line));
    return m_lines.back();
}

std::shared_ptr<objects::Line> PhysicsEngine::addLine(std::shared_ptr<objects::Point> &p1,
                                                      std::shared_ptr<objects::Point> &p2) {
    m_lines.push_back(std::make_shared<objects::Line>(p1, p2));
    return m_lines.back();
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

void PhysicsEngine::applyLineStrength() {
    for (auto &line : m_lines) {
        if (line->isValid()) {
            line->applyForce();
        } else {
            // TODO: implement
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
    renderer.render(m_points, m_magnetic_points, m_lines);
}

bool PhysicsEngine::isRunning() const {
    return renderer.isWindowOpen();
}

const std::vector<std::shared_ptr<objects::Point>> PhysicsEngine::getPoints() const {
    return m_points;
}

std::vector<std::shared_ptr<objects::Point>> PhysicsEngine::getPoints() {
    return m_points;
}

const std::vector<std::shared_ptr<objects::MagneticPoint>> PhysicsEngine::getMagneticPoints() const {
    return m_magnetic_points;
}

std::vector<std::shared_ptr<objects::MagneticPoint>> PhysicsEngine::getMagneticPoints() {
    return m_magnetic_points;
}

const std::vector<std::shared_ptr<objects::Line>> PhysicsEngine::getLines() const {
    return m_lines;
}

std::vector<std::shared_ptr<objects::Line>> PhysicsEngine::getLines() {
    return m_lines;
}
}  // namespace engine::core
