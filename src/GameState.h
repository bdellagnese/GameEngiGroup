#include "State.h"

class GameState : public State {
public:
    void handleInput() override;
    void update(float& dt) override;
    void render(sf::RenderWindow& window) override;
    
    // Special Variables
    float stateChange = 0;

};