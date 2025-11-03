#pragma once

#include <SFML/Graphics.hpp>

inline bool doRectsIntersect(const sf::FloatRect& a, const sf::FloatRect& b)
{
    bool noOverlapX = a.position.x + a.size.x < b.position.x || b.position.x + b.size.x < a.position.x;
    bool noOverlapY = a.position.y + a.size.y < b.position.y || b.position.y + b.size.y < a.position.y;

    return !noOverlapX && !noOverlapY;
}
