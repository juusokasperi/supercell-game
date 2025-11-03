#pragma once

#include "IState.h"
#include <SFML/Graphics/Text.hpp>
#include <memory>

class StateGameOver final : public IState
{
public:
    StateGameOver(StateStack& stateStack, int finalScore, int highScore);
    ~StateGameOver() = default;

    bool init() override;
    void update(float dt) override;
    void render(sf::RenderTarget& target) const override;

private:
    StateStack& m_stateStack;
    const IState* m_pPrevState = nullptr;
    const int m_finalScore;
    const int m_highScore;
    std::unique_ptr<sf::Text> m_pGameOverText;
    std::unique_ptr<sf::Text> m_pScoreText;
    std::unique_ptr<sf::Text> m_pContinueText;
    std::unique_ptr<sf::Text> m_pQuitText;
};
