#pragma once

#include "Entity.h"
#include <memory>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf { class Sprite; }

class Wall final : public Entity
{
public:
    Wall() = default;
    virtual ~Wall() = default;

    bool init() override;
    EntityType getType() const override { return EntityType::Wall; }
    void setHeight(float height);
    void update(float dt, float worldSpeed) override;
    void render(sf::RenderTarget& target) const override;

    float m_width;
    float m_height;
};
