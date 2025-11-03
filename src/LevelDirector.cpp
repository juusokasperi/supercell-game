#include "Constants.hpp"
#include "LevelDirector.h"
#include "entities/Wall.h"
#include "entities/TopWall.h"
#include "entities/Enemy.h"
#include "entities/Ledge.h"

LevelDirector::LevelDirector(std::vector<std::unique_ptr<Entity>>& entityList)
	: m_entities(entityList), m_rng(std::random_device()())
{
}

void LevelDirector::update(float dt)
{
	spawnEnemy(dt);
	spawnLedge(dt);
	spawnWall(dt);
}

void LevelDirector::spawnWall(float dt)
{
    m_timeUntilWallSpawn -= dt;

    if (m_timeUntilWallSpawn < 0.0f)
    {
        m_timeUntilWallSpawn = enemySpawnInterval + 1.25f;

        const float gapSize = 175.f;
        const float bottomHeight = std::uniform_real_distribution<float>(75.f, 200.f)(m_rng);

        auto pBottomWall = std::make_unique<Wall>();
        if (pBottomWall->init())
        {
            pBottomWall->setHeight(bottomHeight);
            pBottomWall->setPosition(sf::Vector2f(WINDOW_WIDTH + 100, WINDOW_HEIGHT - GROUND_HEIGHT));
            m_entities.push_back(std::move(pBottomWall));
        }

        auto pTopWall = std::make_unique<TopWall>();
        if (pTopWall->init())
        {
            float topHeight = WINDOW_HEIGHT - GROUND_HEIGHT - bottomHeight - gapSize;
            pTopWall->setHeight(topHeight);
            pTopWall->setPosition(sf::Vector2f(WINDOW_WIDTH + 100, 0));
            m_entities.push_back(std::move(pTopWall));
        }

    }
}

void LevelDirector::spawnLedge(float dt)
{
    m_timeUntilLedgeSpawn -= dt;

    if (m_timeUntilLedgeSpawn < 0.0f)
    {
        m_timeUntilLedgeSpawn = std::uniform_real_distribution<float>(3.0f, 5.0f)(m_rng);
        auto pLedge = std::make_unique<Ledge>();
        if (pLedge->init())
        {
            pLedge->setPosition(sf::Vector2f(WINDOW_WIDTH + 100, WINDOW_HEIGHT - 0.5f));
            pLedge->setWidth(150.f);
            m_entities.push_back(std::move(pLedge));
        }
    }
}

void LevelDirector::spawnEnemy(float dt)
{
    m_timeUntilEnemySpawn -= dt;

    if (m_timeUntilEnemySpawn < 0.0f)
    {
        m_timeUntilEnemySpawn = enemySpawnInterval;
        std::unique_ptr<Enemy> pEnemy = std::make_unique<Enemy>();
        const float enemyY = std::uniform_real_distribution<float>(300.f, WINDOW_HEIGHT - GROUND_HEIGHT - 20.f)(m_rng);
        pEnemy->setPosition(sf::Vector2f(WINDOW_WIDTH + 50, enemyY));
        pEnemy->setVerticalMovement(50.f, 0.75f);
        if (pEnemy->init())
            m_entities.push_back(std::move(pEnemy));
    }
}
