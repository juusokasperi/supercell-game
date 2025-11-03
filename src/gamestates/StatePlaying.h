#pragma once

#include "IState.h"
#include "entities/Player.h"
#include "entities/TopWall.h"
#include "entities/Enemy.h"
#include "entities/Wall.h"
#include "entities/Ledge.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <array>

class LevelDirector;

class StatePlaying : public IState
{
public:
    StatePlaying(StateStack& stateStack);
    ~StatePlaying();

    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;

private:
    StateStack& m_stateStack;

    std::array<std::unique_ptr<sf::Sprite>, 2>  m_grounds;
    std::array<std::unique_ptr<sf::Sprite>, 2>  m_bigClouds;
    std::array<std::unique_ptr<sf::Sprite>, 2>  m_smallClouds;
    std::unique_ptr<sf::Sprite>                 m_sky;

    std::unique_ptr<Player> m_pPlayer;
    std::vector<std::unique_ptr<Entity>> m_entities;
    std::unique_ptr<LevelDirector> m_pLevelDirector;
    bool m_hasPauseKeyBeenReleased = true;

    float m_worldSpeed;
    float m_groundSpeed;
    float m_bigCloudSpeed;
    float m_smallCloudSpeed;

    int m_score = 0;
    float m_scoreAccumulator = 0.f;
    std::unique_ptr<sf::Text> m_pScoreText;

    bool updateCollisions();
    void checkPause();
    void updateWorldSpeed(float dt);
    void updateScrollingBackground(float dt);
    bool initBackground();
    void handleDeath();

    void destroyOffscreenElements();
};
