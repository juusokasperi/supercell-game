#include "ResourceManager.h"
#include "gamestates/StateStack.h"
#include "gamestates/IState.h"
#include "gamestates/StateMenu.h"
#include <memory>
#include <stack>
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "Constants.hpp"

static bool shouldClose(sf::RenderWindow &window);

int main(int argc, char* argv[])
{
    (void)argc;

    // ResourceManager must be instantiated here -- DO NOT CHANGE
    ResourceManager::init(argv[0]);

    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Runner");
    window.setKeyRepeatEnabled(false);

    StateStack gamestates;
    if (!gamestates.push<StateMenu>())
        return -1;

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time elapsedTime = clock.restart();

        IState* pState = gamestates.getCurrentState();
        if (!pState) return -1;

        if (shouldClose(window))
            window.close();

        window.clear(sf::Color::Blue);
        pState->update(elapsedTime.asSeconds());
        pState->render(window);
        window.display();

        gamestates.performDeferredPops();

        if (gamestates.isQuitDeferred())
            window.close();
        else if (gamestates.isReplayDeferred())
            gamestates.replay();
    }
#ifdef __APPLE__
    ResourceManager::shutdown(); // Cleanup to fix crash on macOS
#endif
    return 0;
}

static bool shouldClose(sf::RenderWindow &window)
{
    while (const std::optional event = window.pollEvent())
        if (event->is<sf::Event::Closed>())
            return true;
    return false;
}
