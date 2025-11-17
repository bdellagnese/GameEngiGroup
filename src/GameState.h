#include "State.h"

class GameState : public State {
public:
    void handleInput() override;
    void update(float& dt) override;
    void render(sf::RenderWindow& window) override;
    float stateChange = 0;
};