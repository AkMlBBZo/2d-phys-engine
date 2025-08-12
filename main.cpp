#include <math.h>

#include <SFML/Graphics.hpp>

// #include "Logger.h" - not in use now
#include "config.h"
#include "engine/core/PhysicsEngine.h"

// auto logger = new AsyncLogger(std::make_unique<FileLogger>(MAIN_LOG_FILE)); - Logger.h

int main() {
    engine::core::PhysicsEngine eng(300);
    engine::objects::Point point(engine::math::Vector2{100, 100});
    engine::objects::MagneticPoint mp1(engine::physics::MagnetPole::NORTH, engine::physics::MaterialType::DEFAULT, 200,
                                       1e5, {200, 100}, 5.0, 1.0, false, engine::colors::WHITE);
    engine::objects::MagneticPoint mp2(engine::physics::MagnetPole::SOUTH, engine::physics::MaterialType::DEFAULT,
                                       100000, 10, {300, 400}, 2.0, 1.0, true, engine::colors::CYAN);
    engine::objects::MagneticPoint mp4(engine::physics::MagnetPole::SOUTH, engine::physics::MaterialType::DEFAULT, 200,
                                       1000, {900, 400}, 2.0, 1.0, true, engine::colors::CYAN);
    engine::objects::MagneticPoint mp3(engine::physics::MagnetPole::SOUTH, engine::physics::MaterialType::DEFAULT, 10,
                                       10000, {200, 500}, 2.0, 1.0, true);
    // eng.addPoint(point);
    // eng.addPoint(mp1);
    // eng.addPoint(mp2);
    // eng.addPoint(mp4);
    // eng.addPoint(mp3);
    int cnt = 0;

    while (eng.isRunning()) {
        if (cnt <= 200) {
            cnt++;
            mp1.setPosition(mp1.getPosition() + engine::math::Vector2{1, 0});
            eng.addPoint(mp1);
        }
        eng.applyGravity();
        eng.applyMagnetic();
        eng.update();
        eng.render();
    }
    return EXIT_SUCCESS;
}