#include "State.h"

class PauseScene : public State {
public:
    void handleInput() override;
    void update(float& dt) override;
    void render(sf::RenderWindow& window) override;

    bool unpause = false;
    bool quitGame = false;
};