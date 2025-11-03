#pragma once

#include "Entity.h"
#include <memory>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf { class Sprite; }

class Player final : public Entity
{
public:
    Player();
    virtual ~Player() = default;

    EntityType getType() const override { return EntityType::Player; }
    bool init() override;
    void update(float dt, float worldSpeed) override;
    void render(sf::RenderTarget& target) const override;
    bool isJumping() const { return m_isJumping; };
    void land(float y);
    void setSupported(bool supported) { m_isSupported = supported; }

    bool m_isJumping = false;
    bool m_canJump = true;
    bool m_isJumpHeld = false;
    bool m_isSupported = false;
};
