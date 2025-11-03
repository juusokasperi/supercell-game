#include "Enemy.h"
#include "ResourceManager.h"
#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

bool Enemy::init()
{
    const sf::Texture* pTexture = ResourceManager::getOrLoadTexture("enemy.png");
    if (pTexture == nullptr)
        return false;

    m_pSprite = std::make_unique<sf::Sprite>(*pTexture);
    if (!m_pSprite)
        return false;

    sf::FloatRect localBounds = m_pSprite->getLocalBounds();
    m_pSprite->setOrigin({localBounds.size.x / 2.0f, localBounds.size.y});
    m_pSprite->setPosition(m_position);
    m_pSprite->setScale(sf::Vector2f(2.5f, 2.5f));
    m_initialY = m_position.y;

    return true;
}

void Enemy::update(float dt, float worldSpeed)
{
    m_position.x -= worldSpeed * dt;
    m_verticalMovementAngle += m_verticalMovementSpeed * dt;
    m_position.y = m_initialY + std::sin(m_verticalMovementAngle) * m_verticalMovementRadius;
}

void Enemy::setVerticalMovement(float radius, float speed) {
    m_verticalMovementRadius = radius;
    m_verticalMovementSpeed = speed;
}

void Enemy::render(sf::RenderTarget& target) const
{
    m_pSprite->setPosition(m_position);
    target.draw(*m_pSprite);
}
