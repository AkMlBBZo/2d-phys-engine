#include "engine/rendering/Renderer.h"

#include "config.h"

namespace engine {

Renderer::Renderer()
    : window(sf::VideoMode::getDesktopMode(), APP_NAME, sf::Style::Default, sf::State::Fullscreen)
    , m_ratio_width(window.getSize().x / Bounds::WIDTH)
    , m_ratio_height(window.getSize().y / Bounds::HEIGHT) {
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    pointVertices.setPrimitiveType(sf::PrimitiveType::Points);
}

void Renderer::objRender(objects::Point& point) {
    sf::CircleShape pict(point.getRadius());
    pict.setPosition(point.getPosition());
    pict.setFillColor(point.getColor());
    window.draw(pict);
}

void Renderer::addPoint(objects::Point& point) {
    pointVertices.append(
        sf::Vertex({point.getPosition() * math::Vector2(m_ratio_width, m_ratio_height), point.getColor(), {}}));
}

void Renderer::render(std::vector<std::unique_ptr<engine::objects::Point>>& points,
                      std::vector<std::unique_ptr<engine::objects::MagneticPoint>>& magneticPoints) {
    clear();
    handleEvents();

    for (const auto& pointPtr : points) {
        if (pointPtr) {
            addPoint(*pointPtr);
        }
    }
    for (const auto& pointPtr : magneticPoints) {
        if (pointPtr) {
            addPoint(*pointPtr);
        }
    }
    window.draw(pointVertices);
    pointVertices.clear();

    display();
}

void Renderer::clear() {
    window.clear();
}

void Renderer::display() {
    window.display();
}

bool Renderer::isWindowOpen() const {
    return window.isOpen();
}

void Renderer::handleEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

}  // namespace engine