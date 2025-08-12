#ifndef COLORS_ENGINE_H
#define COLORS_ENGINE_H

#include <SFML/Graphics.hpp>

namespace engine::colors {

constexpr sf::Color RED     = {255, 0, 0};
constexpr sf::Color GREEN   = {0, 255, 0};
constexpr sf::Color BLUE    = {0, 0, 255};
constexpr sf::Color WHITE   = {255, 255, 255};
constexpr sf::Color BLACK   = {0, 0, 0};
constexpr sf::Color CYAN    = {0, 255, 255};
constexpr sf::Color FUCHSIA = {255, 0, 255};
constexpr sf::Color YELLOW  = {255, 255, 0};
constexpr sf::Color MAGENTA = {255, 0, 144};

}  // namespace engine::colors

#endif  // COLORS_ENGINE_H