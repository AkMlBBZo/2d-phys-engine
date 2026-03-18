#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <SFML/Graphics.hpp>

namespace engine {

class Renderer {
private:
    sf::RenderWindow window;
    float m_ratio_width;
    float m_ratio_height;

public:
    Renderer()
        : window(sf::VideoMode::getDesktopMode(), APP_NAME, sf::Style::Default, sf::State::Fullscreen)
        , m_ratio_width(window.getSize().x / Bounds::WIDTH)
        , m_ratio_height(window.getSize().y / Bounds::HEIGHT) {
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);
    }
    ~Renderer() = default;

    void clear() { window.clear(); }
    void display() { window.display(); }
    bool isWindowOpen() const { return window.isOpen(); }

    void handleEvents() {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }

    void renderPoints(float* pos_x, float* pos_y, size_t size) {
        sf::VertexArray points(sf::PrimitiveType::Points, size);

        for (size_t i = 0; i < size; ++i) {
            points[i].position = sf::Vector2f(pos_x[i], pos_y[i]);
            points[i].color    = sf::Color::White;
        }

        window.draw(points);
    }

    void renderSprings(float* pos_x, float* pos_y, const uint32_t* start_idx, const uint32_t* end_idx, size_t size) {
        sf::VertexArray lines(sf::PrimitiveType::Lines, size * 2);

        for (size_t i = 0; i < size; ++i) {
            uint32_t start = start_idx[i];
            uint32_t end   = end_idx[i];

            lines[i * 2].position     = sf::Vector2f(pos_x[start], pos_y[start]);
            lines[i * 2 + 1].position = sf::Vector2f(pos_x[end], pos_y[end]);

            lines[i * 2].color     = sf::Color::Magenta;
            lines[i * 2 + 1].color = sf::Color::Magenta;
        }

        window.draw(lines);
    }

    sf::Vector2u getWindowSize() const { return window.getSize(); }
};

}  // namespace engine

#endif  // RENDER_ENGINE_H