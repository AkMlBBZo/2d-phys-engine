#include "engine/objects/Line.h"

namespace engine::objects {

Line::Line(std::shared_ptr<Point> start, std::shared_ptr<Point> end, const sf::Color& color)
    : m_start(start), m_end(end), m_color(color) {
    if (!start || !end) {
        throw std::invalid_argument("Line points cannot be null");
    }
    if (start == end) {
        throw std::invalid_argument("Line points must be different");
    }
}

std::shared_ptr<Point> Line::getStart() const {
    return m_start.lock();
}

std::shared_ptr<Point> Line::getEnd() const {
    return m_end.lock();
}

bool Line::isValid() const noexcept {
    return !m_start.expired() && !m_end.expired();
}

const sf::Color& Line::getColor() const {
    return m_color;
}

sf::Color& Line::getColor() {
    return m_color;
}

void Line::setColor(const sf::Color& color) {
    m_color = color;
}

}  // namespace engine::objects