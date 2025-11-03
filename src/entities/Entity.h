#pragma once

#include <memory>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf { class RenderTarget; };

enum class EntityType {
    Enemy,
    Wall,
    TopWall,
    Ledge,
    Player = -1,
    None = -1
};

class Entity
{
public:
    Entity() = default;
    Entity(const sf::Vector2f& position, const sf::Angle& rotation);
    virtual ~Entity() = default;

    virtual bool init() = 0;
    virtual void update(float dt, float worldSpeed) = 0;
    virtual void render(sf::RenderTarget& target) const = 0;
    virtual EntityType getType() const = 0;

    const sf::Vector2f& getPosition() const { return m_position; }
    void setPosition(const sf::Vector2f& position) { m_position = position; };

    const sf::Angle& getRotation() const { return m_rotation; }
    void setRotation(const sf::Angle& rotation) { m_rotation = rotation; };

    virtual sf::FloatRect getGlobalBounds() const
    {
        if (m_pSprite) return m_pSprite->getGlobalBounds();
        return sf::FloatRect(m_position, {0, 0});
    }

    sf::Vector2f getVelocity() const { return m_velocity; };

    int getHealth() const { return m_health; }
    void setHealth(int health) { m_health = health; }

protected:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Angle m_rotation;
    std::unique_ptr<sf::Sprite> m_pSprite;
    int m_health = 0;
};
