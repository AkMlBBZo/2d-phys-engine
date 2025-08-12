#include <math.h>

#include <SFML/Graphics.hpp>

// #include "Logger.h" - not in use now
#include "config.h"
#include "engine/core/PhysicsEngine.h"

// auto logger = new AsyncLogger(std::make_unique<FileLogger>(MAIN_LOG_FILE)); - Logger.h

int main() {
    engine::core::PhysicsEngine eng(100);
    engine::objects::Point point(engine::math::Vector2{100, 100});
    engine::objects::MagneticPoint mp1(engine::physics::MagnetPole::NORTH, engine::physics::MaterialType::DEFAULT, 10,
                                       100, {200, 100}, 1.0, 1.0, false, engine::colors::RED);
    engine::objects::MagneticPoint mp2(engine::physics::MagnetPole::NORTH, engine::physics::MaterialType::DEFAULT, 10,
                                       100, {250, 150}, 1.0, 1.0, false, engine::colors::CYAN);
    engine::objects::MagneticPoint ip1(engine::physics::MagnetPole::NONE, engine::physics::MaterialType::IRON, 10, 100,
                                       {200, 150}, 1.0, 1.0, false, engine::colors::WHITE);
    int cnt = 0;

    auto p1 = eng.addPoint(point);
    auto p2 = eng.addPoint(mp1);
    auto p3 = eng.addPoint(mp2);

    auto line1 = eng.addLine(p1, p2);
    auto line2 = eng.addLine(p1, p3);
    auto line3 = eng.addLine(p2, p3);

    while (eng.isRunning()) {
        if (cnt <= 50) {
            cnt++;
            auto p1 = eng.addPoint(point);
            auto p2 = eng.addPoint(mp1);
            auto p3 = eng.addPoint(mp2);

            auto line1 = eng.addLine(p1, p2);
            auto line2 = eng.addLine(p1, p3);
            auto line3 = eng.addLine(p2, p3);
        }
        eng.applyGravity();
        eng.applyMagnetic();
        eng.update();
        eng.render();
    }
    return EXIT_SUCCESS;
}