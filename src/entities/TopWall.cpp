#include "TopWall.h"
#include "ResourceManager.h"
#include "Constants.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

bool TopWall::init()
{
    const sf::Texture* pTexture = ResourceManager::getOrLoadTexture("pipe.png");
    if (pTexture == nullptr)
        return false;

    m_pSprite = std::make_unique<sf::Sprite>(*pTexture);
    if (!m_pSprite)
        return false;

	m_height = WALL_HEIGHT;
	m_width = m_height * 0.75;

    sf::FloatRect localBounds = m_pSprite->getLocalBounds();
    m_pSprite->setOrigin({localBounds.size.x / 2.0f, localBounds.size.y});
    m_pSprite->setScale(sf::Vector2f(m_width / localBounds.size.x, -m_height / localBounds.size.y));

    return true;
}

void TopWall::setHeight(float height)
{
	m_height = height;
	sf::FloatRect localBounds = m_pSprite->getLocalBounds();
	m_pSprite->setScale(sf::Vector2f(m_width / localBounds.size.x, -m_height / localBounds.size.y));
}

void TopWall::update(float dt, float worldSpeed)
{
    m_position.x -= worldSpeed * dt;
}

void TopWall::render(sf::RenderTarget& target) const
{
    m_pSprite->setPosition(m_position);
    target.draw(*m_pSprite);
}
