#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

#include "config.h"
#include "engine/core/PhysicsEngine.h"

std::pair<float, float> calculateCentripetalAcceleration(float posx, float posy, float vx, float vy, float cx,
                                                         float cy) {
    float dx = cx - posx;
    float dy = cy - posy;

    float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0.0001f) {
        dx /= length;
        dy /= length;
    } else {
        dx = 1.0f;
        dy = 0.0f;
    }

    float current_speed  = std::sqrt(vx * vx + vy * vy);
    float current_radius = std::sqrt((posx - cx) * (posx - cx) + (posy - cy) * (posy - cy));

    if (current_radius < 0.0001f) {
        current_radius = 0.0001f;
    }

    float a_cent_mag = (current_speed * current_speed) / current_radius;

    return {a_cent_mag * dx, a_cent_mag * dy};
}

int main() {
    engine::core::PhysicsEngine eng;

    constexpr int grid_size   = 100;
    constexpr float spacing   = 3.0f;
    constexpr float start_x   = 400.0f;
    constexpr float start_y   = 300.0f;
    constexpr float stiffness = 10.0f;
    constexpr float damp      = 0.4f;
    constexpr float mass      = 100.0f;

    std::vector<std::vector<size_t>> point_grid(grid_size, std::vector<size_t>(grid_size));

    for (int y = 0; y < grid_size; ++y) {
        for (int x = 0; x < grid_size; ++x) {
            point_grid[y][x] = eng.addPoint(start_x + x * spacing, start_y + y * spacing, mass);
        }
    }

    for (int y = 0; y < grid_size; ++y) {
        for (int x = 0; x < grid_size - 1; ++x) {
            eng.addSpring(point_grid[y][x], point_grid[y][x + 1], stiffness, damp);
        }
    }

    for (int x = 0; x < grid_size; ++x) {
        for (int y = 0; y < grid_size - 1; ++y) {
            eng.addSpring(point_grid[y][x], point_grid[y + 1][x], stiffness, damp);
        }
    }

    for (int y = 0; y < grid_size - 1; ++y) {
        for (int x = 0; x < grid_size - 1; ++x) {
            eng.addSpring(point_grid[y][x], point_grid[y + 1][x + 1], stiffness, damp);
            eng.addSpring(point_grid[y][x + 1], point_grid[y + 1][x], stiffness, damp);
        }
    }

    float cx = 500.0f;
    float cy = 400.0f;
    float dt = 0.01f;

    static bool initial_push = false;
    while (eng.isOpen()) {
        for (int y = 0; y < grid_size; ++y) {
            for (int x = 0; x < grid_size; ++x) {
                auto [posx, posy]   = eng.pos(point_grid[y][x]);
                auto [pposx, pposy] = eng.prevPos(point_grid[y][x]);

                float dx = posx - pposx;
                float dy = posy - pposy;

                float vx = dx / dt;
                float vy = dy / dt;

                auto [ax_cent, ay_cent] = calculateCentripetalAcceleration(posx, posy, vx, vy, cx, cy);

                eng.applyForce(point_grid[y][x], ax_cent, ay_cent);
            }
        }

        if (!initial_push) {
            for (int y = 0; y < grid_size; ++y) {
                for (int x = 0; x < grid_size; ++x) {
                    auto [posx, posy] = eng.pos(point_grid[y][x]);
                    float dx          = posx - cx;
                    float dy          = posy - cy;
                    float length      = std::sqrt(dx * dx + dy * dy);

                    if (length > 0.0001f) {
                        float perp_x = -dy / length;
                        float perp_y = dx / length;

                        eng.applyForce(point_grid[y][x], perp_x * 2500.0f, perp_y * 2500.0f);
                    }
                }
            }
            initial_push = true;
        }

        eng.integrate(dt, 1);
        eng.render();
    }

    return EXIT_SUCCESS;
}