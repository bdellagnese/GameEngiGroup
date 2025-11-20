#include "State.h"

class GameState : public State {
public:
    void handleInput() override;
    void update(float& dt) override;
    void render(sf::RenderWindow& window) override;
    
    static void random();
    static void nextCharacter();
    
    // Special Variables
    float stateChange = 0;
};