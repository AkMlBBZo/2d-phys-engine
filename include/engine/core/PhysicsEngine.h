#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "engine/objects/PointsSystem.h"
#include "engine/objects/SpringsSystem.h"
#include "engine/rendering/Renderer.h"

namespace engine::core {

class PhysicsEngine {
private:
    objects::PointsSystem ps;
    objects::SpringsSystem ss;
    Renderer renderer;

public:
    PhysicsEngine() : ss(&ps) {}
    ~PhysicsEngine() = default;

    size_t addPoint(float pos_x, float pos_y, float mass) { return ps.addPoint(pos_x, pos_y, mass); }

    uint32_t addSpring(uint32_t start_point, uint32_t end_point, float stiffness, float damping,
                       float rest_length = -1.0f, float min_length = -1.0f, float max_length = -1.0f) {
        return ss.addSpring(start_point, end_point, stiffness, damping, rest_length, min_length, max_length);
    }

    void applyForce(size_t idx, float force_x, float force_y) { ps.applyForce(idx, force_x, force_y); }
    void applyGlobalForce(float force_x, float force_y) { ps.applyGlobalForce(force_x, force_y); }

    void integrate(float dt, float damping = 0.95f) {
        dt = std::min(dt, 0.01f);
        ss.applySpringsForce(dt);
        ps.integrate(dt, damping);
    }

    void render() {
        renderer.clear();
        renderer.handleEvents();
        renderer.renderPoints(ps.positionsX(), ps.positionsY(), ps.size());
        renderer.renderSprings(ps.positionsX(), ps.positionsY(), ss.startIndices(), ss.endIndices(), ss.size());
        renderer.display();
    }

    bool isOpen() { return renderer.isWindowOpen(); }

    std::tuple<float, float> pos(size_t point_idx) {
        // if(point_idx >= ps.size()) => ?? (OK)
        float* posx = ps.positionsX();
        float* posy = ps.positionsY();
        return {posx[point_idx], posy[point_idx]};
    }

    std::tuple<float, float> prevPos(size_t point_idx) {
        // if(point_idx >= ps.size()) => ?? (OK)
        float* posx = ps.prevPositionsX();
        float* posy = ps.prevPositionsY();
        return {posx[point_idx], posy[point_idx]};
    }
};

}  // namespace engine::core

#endif  // PHYSICS_ENGINE_H