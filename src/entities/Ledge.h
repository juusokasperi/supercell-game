#pragma once

#include "Entity.h"
#include <memory>

class Ledge final : public Entity
{
public:
    Ledge() = default;
    virtual ~Ledge() = default;

    EntityType getType() const override { return EntityType::Ledge; }
    bool init() override;
    void setWidth(float width);
    void update(float dt, float worldSpeed) override;
    void render(sf::RenderTarget& target) const override;

    float m_width;
    float m_height;
};
