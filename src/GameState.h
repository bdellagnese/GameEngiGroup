#include "State.h"

class GameState : public State {
public:
    void handleInput() override;
    void update() override;
    void render(sf::RenderWindow& window) override;
};