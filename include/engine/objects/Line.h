#ifndef LINE_ENGINE_H
#define LINE_ENGINE_H

#include <memory>

#include "engine/objects/Point.h"

namespace engine::objects {

class Line {
private:
    std::weak_ptr<Point> m_start;
    std::weak_ptr<Point> m_end;
    sf::Color m_color;

public:
    Line(std::shared_ptr<Point> start, std::shared_ptr<Point> end, const sf::Color& color = colors::MAGENTA);
    virtual ~Line() = default;

    std::shared_ptr<Point> getStart() const;
    std::shared_ptr<Point> getEnd() const;
    bool isValid() const noexcept;
    const sf::Color& getColor() const;
    sf::Color& getColor();
    void setColor(const sf::Color& color);

    virtual void applyForce() {};
};

}  // namespace engine::objects

#endif  // LINE_ENGINE_H