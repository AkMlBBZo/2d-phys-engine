#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "engine/objects/Line.h"
#include "engine/objects/MagneticPoint.h"
#include "engine/objects/Point.h"

namespace engine {

class Renderer {
private:
    sf::RenderWindow window;
    sf::VertexArray pointVertices;
    sf::VertexArray lineVertices;
    float m_ratio_width;
    float m_ratio_height;

public:
    Renderer();

    void objRender(objects::Point& point);

    void addPoint(objects::Point& point);

    void addLine(objects::Line& line);

    void render(std::vector<std::shared_ptr<objects::Point>>& points,
                std::vector<std::shared_ptr<objects::MagneticPoint>>& magneticPoints,
                std::vector<std::shared_ptr<objects::Line>>& lines);

    void clear();
    void display();

    bool isWindowOpen() const;
    void handleEvents();
};

}  // namespace engine

#endif  // RENDER_ENGINE_H