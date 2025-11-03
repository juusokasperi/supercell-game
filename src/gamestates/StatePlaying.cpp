#include "StatePlaying.h"
#include "StatePaused.h"
#include "StateGameOver.h"
#include "StateStack.h"
#include "LevelDirector.h"
#include "ResourceManager.h"
#include "Constants.hpp"
#include "Helpers.hpp"
#include <memory>
#include <iostream>
#include <fstream>
#include <SFML/Graphics/RenderTarget.hpp>

typedef sf::Keyboard::Key Key;

StatePlaying::StatePlaying(StateStack& stateStack)
    : m_stateStack(stateStack)
{
}

StatePlaying::~StatePlaying() = default;

bool StatePlaying::init()
{
    m_worldSpeed = NORMAL_SPEED;

    if (!initBackground())
        return false;
    m_pPlayer = std::make_unique<Player>();
    if (!m_pPlayer || !m_pPlayer->init())
        return false;

    m_pLevelDirector = std::make_unique<LevelDirector>(m_entities);

    m_pPlayer->setPosition(sf::Vector2f(PLAYER_START_POS_X, WINDOW_HEIGHT - GROUND_HEIGHT));

    const sf::Font* pFont = ResourceManager::getOrLoadFont("Lavigne.ttf");
    if (pFont == nullptr)
        return false;

    m_pScoreText = std::make_unique<sf::Text>(*pFont);
    if (!m_pScoreText)
        return false;

    m_pScoreText->setString("0");
    m_pScoreText->setStyle(sf::Text::Bold);
    m_pScoreText->setPosition(sf::Vector2f(20.f, 10.f));
    m_pScoreText->setFillColor(sf::Color::White);

    return true;
}

void StatePlaying::update(float dt)
{
    m_pLevelDirector->update(dt);
    checkPause();
    updateWorldSpeed(dt);
    updateScrollingBackground(dt);

    m_pPlayer->update(dt, m_worldSpeed);
    for (const std::unique_ptr<Entity>& pEntity: m_entities)
        pEntity->update(dt, m_worldSpeed);
    destroyOffscreenElements();

    bool playerDied = updateCollisions();
    if (playerDied) handleDeath();

    m_scoreAccumulator += m_worldSpeed * dt / 100.f;
    m_score = static_cast<int>(m_scoreAccumulator);
    m_pScoreText->setString(std::to_string(m_score));
}

void StatePlaying::render(sf::RenderTarget& target) const
{
    target.draw(*m_sky);
    for (const auto& cloud : m_bigClouds)
        target.draw(*cloud);
    for (const auto& cloud : m_smallClouds)
        target.draw(*cloud);
    for (const auto& ground : m_grounds)
        target.draw(*ground);
    for (const std::unique_ptr<Entity>& pEntity : m_entities)
        pEntity->render(target);
    m_pPlayer->render(target);
    target.draw(*m_pScoreText);
}


void StatePlaying::checkPause()
{
    bool isPauseKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
    m_hasPauseKeyBeenReleased |= !isPauseKeyPressed;
    if (m_hasPauseKeyBeenReleased && isPauseKeyPressed)
    {
        m_hasPauseKeyBeenReleased = false;
        m_stateStack.push<StatePaused>();
    }
}

void StatePlaying::updateScrollingBackground(float dt)
{
    for (auto& ground : m_grounds)
    {
        ground->move(sf::Vector2f(-m_groundSpeed * dt, 0.f));
        if (ground->getPosition().x < -WINDOW_WIDTH)
        {
            ground->move(sf::Vector2f(2 * WINDOW_WIDTH, 0.f));
        }
    }
    for (auto& cloud : m_bigClouds)
    {
        cloud->move(sf::Vector2f(-m_bigCloudSpeed * dt, 0.f));
        if (cloud->getPosition().x < -WINDOW_WIDTH)
        {
            cloud->move(sf::Vector2f(2 * WINDOW_WIDTH, 0.f));
        }
    }
    for (auto& cloud : m_smallClouds)
    {
        cloud->move(sf::Vector2f(-m_smallCloudSpeed * dt, 0.f));
        if (cloud->getPosition().x < -WINDOW_WIDTH)
        {
            cloud->move(sf::Vector2f(2 * WINDOW_WIDTH, 0.f));
        }
    }
}

void StatePlaying::updateWorldSpeed(float dt)
{
    bool playerIsJumping = m_pPlayer->isJumping();
    if (playerIsJumping) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
    {
        m_worldSpeed += ACCELERATION * dt;
        if (m_worldSpeed > MAX_SPEED) m_worldSpeed = MAX_SPEED;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
    {
        m_worldSpeed -= DECELERATION * dt;
        if (m_worldSpeed < MIN_SPEED) m_worldSpeed = MIN_SPEED;
    }
    else
    {
        if (m_worldSpeed > NORMAL_SPEED)
        {
            m_worldSpeed -= FRICTION * dt;
            if (m_worldSpeed < NORMAL_SPEED) m_worldSpeed = NORMAL_SPEED;
        }
        else if (m_worldSpeed < NORMAL_SPEED)
        {
            m_worldSpeed += FRICTION * dt;
            if (m_worldSpeed > NORMAL_SPEED) m_worldSpeed = NORMAL_SPEED;
        }
    }
    m_groundSpeed = m_worldSpeed;
    m_smallCloudSpeed = m_worldSpeed * SMALL_CLOUD_SPEED_RATIO;
    m_bigCloudSpeed = m_worldSpeed * BIG_CLOUD_SPEED_RATIO;
}

void StatePlaying::handleDeath()
{
    int highScore = 0;
    std::ifstream inFile("highscore.txt");
    if (inFile.is_open())
    {
        inFile >> highScore;
        inFile.close();
    }

    if (m_score > highScore)
    {
        highScore = m_score;
        std::ofstream outFile("highscore.txt");
        if (outFile.is_open())
        {
            outFile << highScore;
            outFile.close();
        }
    }

    m_stateStack.push<StateGameOver>(m_score, highScore);
}

bool StatePlaying::updateCollisions()
{
    bool playerDied = false;

    if (m_pPlayer->getPosition().y > WINDOW_HEIGHT - GROUND_HEIGHT)
    {
        m_pPlayer->land(WINDOW_HEIGHT - GROUND_HEIGHT);
        m_pPlayer->setSupported(true);
    }

    for (const std::unique_ptr<Entity>& pEntity : m_entities)
    {
        sf::FloatRect playerBounds = m_pPlayer->getGlobalBounds();
        sf::FloatRect entityBounds = pEntity->getGlobalBounds();
        if (!playerBounds.findIntersection(entityBounds))
            continue;
        switch (pEntity->getType())
        {
            case EntityType::Enemy:
            case EntityType::TopWall:
            case EntityType::Ledge:
                playerDied = true;
                break;
            case EntityType::Wall:
                if (auto intersection = playerBounds.findIntersection(entityBounds))
                {
                    if (intersection->size.x < intersection->size.y)
                    {
                        playerDied = true;
                        break;
                    }
                    else if (m_pPlayer->getVelocity().y >= 0)
                    {
                        m_pPlayer->land(entityBounds.position.y);
                        m_pPlayer->setSupported(true);
                    }
                }
                break;
            case EntityType::Player:
            default:
                break;
        }
    }
    return playerDied;
}

void StatePlaying::destroyOffscreenElements()
{
    const float offScreenX = -200.f;

    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(),
            [offScreenX](const auto& pEntity) {
                return pEntity->getPosition().x < offScreenX;
            }),
        m_entities.end()
    );
}

bool StatePlaying::initBackground()
{
    const sf::Texture* sTexture = ResourceManager::getOrLoadTexture("sky.png");
    const sf::Texture* c1Texture = ResourceManager::getOrLoadTexture("big_cloud.png");
    const sf::Texture* c2Texture = ResourceManager::getOrLoadTexture("small_cloud.png");
    const sf::Texture* gTexture = ResourceManager::getOrLoadTexture("ground.png");
    if (sTexture == nullptr || c1Texture == nullptr
        || c2Texture == nullptr || gTexture == nullptr)
        return false;

    m_sky = std::make_unique<sf::Sprite>(*sTexture);
    if (!m_sky)
        return false;
    m_sky->setPosition(sf::Vector2f(0.f, 0.f));
    m_sky->setScale(sf::Vector2f(
        WINDOW_WIDTH / m_sky->getLocalBounds().size.x,
        WINDOW_HEIGHT / m_sky->getLocalBounds().size.y
    ));

    for (int i = 0; i < 2; ++i)
    {
        m_bigClouds[i] = std::make_unique<sf::Sprite>(*c1Texture);
        if (!m_bigClouds[i])
            return false;

        sf::FloatRect localBounds = m_bigClouds[i]->getLocalBounds();
        m_bigClouds[i]->setScale(sf::Vector2f(
            WINDOW_WIDTH / localBounds.size.x, WINDOW_HEIGHT / localBounds.size.y));
        m_bigClouds[i]->setPosition(sf::Vector2f(i * WINDOW_WIDTH, 0.f));
    }

    for (int i = 0; i < 2; ++i)
    {
        m_smallClouds[i] = std::make_unique<sf::Sprite>(*c2Texture);
        if (!m_smallClouds[i])
            return false;

        sf::FloatRect localBounds = m_smallClouds[i]->getLocalBounds();
        m_smallClouds[i]->setScale(sf::Vector2f(
            WINDOW_WIDTH / localBounds.size.x, WINDOW_HEIGHT / localBounds.size.y));
        m_smallClouds[i]->setPosition(sf::Vector2f(i * WINDOW_WIDTH, 0.f));
    }

    for (int i = 0; i < 2; ++i)
    {
        m_grounds[i] = std::make_unique<sf::Sprite>(*gTexture);
        if (!m_grounds[i])
            return false;

        sf::FloatRect localBounds = m_grounds[i]->getLocalBounds();
        m_grounds[i]->setScale(sf::Vector2f(
            WINDOW_WIDTH / localBounds.size.x, GROUND_HEIGHT / localBounds.size.y));
        m_grounds[i]->setPosition(sf::Vector2f(i * WINDOW_WIDTH, WINDOW_HEIGHT - GROUND_HEIGHT));
    }
    return true;
}
