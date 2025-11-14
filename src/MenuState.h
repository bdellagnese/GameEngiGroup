#include "State.h"

class MenuState : public State {
public:
    void handleInput() override;
    void update() override;
    void render(sf::RenderWindow& window) override;
    bool startGame = false;
};
