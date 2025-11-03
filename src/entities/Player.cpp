#include "Player.h"
#include "ResourceManager.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cmath>
#include <iostream>
#include "Constants.hpp"

Player::Player()
{
}

bool Player::init()
{
    const sf::Texture* pTexture = ResourceManager::getOrLoadTexture("player.png");
    if (pTexture == nullptr)
        return false;

    m_pSprite = std::make_unique<sf::Sprite>(*pTexture);
    if (!m_pSprite)
        return false;

    m_rotation = sf::degrees(0);
    m_velocity = sf::Vector2f(0.f, 0.f);
    sf::FloatRect localBounds = m_pSprite->getLocalBounds();
    m_pSprite->setOrigin({localBounds.size.x / 2.0f, localBounds.size.y});
    m_pSprite->setPosition(m_position);
    m_pSprite->setScale(sf::Vector2f(3.0f, 3.0f));

    return true;
}

void Player::update(float dt, float worldSpeed)
{
    (void)worldSpeed;
    bool isJumpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    m_isSupported = false;

    if (isJumpKeyPressed && m_canJump && !m_isJumping)
    {
        m_velocity.y = JUMP_FORCE;
        m_isJumping = true;
        m_canJump = false;
        m_isJumpHeld = true;
    }

    if (!isJumpKeyPressed)
        m_isJumpHeld = false;

    if (m_isJumping)
    {
        if (m_isJumpHeld && m_velocity.y < 0)
            m_velocity.y += (GRAVITY * 0.5f) * dt;
        else
            m_velocity.y += GRAVITY * dt;
    }

    if (!m_isJumpHeld && m_velocity.y < 0.f)
        m_velocity.y = std::max(m_velocity.y, -400.f);

    if (!m_isSupported && !m_isJumping)
    {
        m_isJumping = true;
    }

    m_position += m_velocity * dt;

    if (!m_isJumping && !isJumpKeyPressed)
        m_canJump = true;
}

void Player::land(float y)
{
    m_position.y = y;
    m_velocity.y = 0.f;
    if (m_isJumping)
    {
        m_isJumping = false;
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            m_canJump = true;
    }
}

void Player::render(sf::RenderTarget& target) const
{
    m_pSprite->setRotation(m_rotation);
    m_pSprite->setPosition(m_position);
    target.draw(*m_pSprite);
}
