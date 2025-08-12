#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "engine/objects/MagneticPoint.h"
#include "engine/objects/Point.h"

namespace engine {

class Renderer {
private:
    sf::RenderWindow window;
    sf::VertexArray pointVertices;
    float m_ratio_width;
    float m_ratio_height;

public:
    Renderer();

    void objRender(objects::Point& point);

    void addPoint(objects::Point& point);

    void render(std::vector<std::unique_ptr<engine::objects::Point>>& points,
                std::vector<std::unique_ptr<engine::objects::MagneticPoint>>& magneticPoints);

    void clear();
    void display();

    bool isWindowOpen() const;
    void handleEvents();
};

}  // namespace engine

#endif  // RENDER_ENGINE_H