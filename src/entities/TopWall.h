#pragma once

#include "Entity.h"
#include <memory>

class TopWall final : public Entity
{
public:
    TopWall() = default;
    virtual ~TopWall() = default;

    EntityType getType() const override { return EntityType::TopWall; }
    bool init() override;
    void setHeight(float height);
    void update(float dt, float worldSpeed) override;
    void render(sf::RenderTarget& target) const override;

    float m_width;
    float m_height;
};
