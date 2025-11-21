#include "State.h"

class GameState : public State {
public:
    void handleInput() override;
    void update(float& dt) override;
    void render(sf::RenderWindow& window) override;
    
    static void random();
    
    // Special Variables
    bool firstLoad;
    float stateChange = 0;
};