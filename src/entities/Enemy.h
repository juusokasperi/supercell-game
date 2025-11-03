#pragma once

#include "Entity.h"
#include <memory>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf { class Sprite; }

class Enemy final : public Entity
{
public:
    Enemy() = default;
    virtual ~Enemy() = default;

    EntityType getType() const override { return EntityType::Enemy; }
    bool init() override;
    void update(float dt, float worldSpeed) override;
    void render(sf::RenderTarget& target) const override;
    void setVerticalMovement(float radius, float speed);

private:
    float m_initialY;
    float m_verticalMovementAngle = 0.f;
    float m_verticalMovementRadius = 0.f;
    float m_verticalMovementSpeed = 0.f;
};
