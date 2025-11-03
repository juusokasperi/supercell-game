#include "StateGameOver.h"
#include "StatePlaying.h"
#include "StateStack.h"
#include "ResourceManager.h"
#include "Constants.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

StateGameOver::StateGameOver(StateStack& stateStack, int finalScore, int highScore)
    : m_stateStack(stateStack), m_finalScore(finalScore), m_highScore(highScore)
{
}

bool StateGameOver::init()
{
    m_pPrevState = m_stateStack.getCurrentState();

    const sf::Font* pFont = ResourceManager::getOrLoadFont("Lavigne.ttf");
    if (pFont == nullptr)
        return false;

    m_pGameOverText = std::make_unique<sf::Text>(*pFont, "GAME OVER", 64);
    m_pScoreText = std::make_unique<sf::Text>(*pFont, "Score: " + std::to_string(m_finalScore) + "\nHigh Score: " + std::to_string(m_highScore), 32);
    m_pContinueText = std::make_unique<sf::Text>(*pFont, "Press Enter to play again", 24);
    m_pQuitText = std::make_unique<sf::Text>(*pFont, "Press Q to quit game", 24);

    if (!m_pGameOverText || !m_pScoreText || !m_pContinueText || !m_pQuitText)
        return false;

    m_pGameOverText->setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 100.f));
    m_pScoreText->setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
    m_pContinueText->setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 100.f));
    m_pQuitText->setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 120.f));

    return true;
}

void StateGameOver::update(float dt)
{
    (void)dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
    {
        m_stateStack.popDeferred(); // Pop self
        m_stateStack.popDeferred(); // Pop StatePlaying
        m_stateStack.replayDeferred();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        m_stateStack.quitDeferred();
}

void StateGameOver::render(sf::RenderTarget& target) const
{
    if (m_pPrevState != nullptr)
        m_pPrevState->render(target);
    sf::RectangleShape fade(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    fade.setFillColor(sf::Color(0, 0, 0, 150));
    target.draw(fade);

    target.draw(*m_pGameOverText);
    target.draw(*m_pScoreText);
    target.draw(*m_pContinueText);
    target.draw(*m_pQuitText);
}
