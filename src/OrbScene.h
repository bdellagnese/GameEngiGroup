#include "State.h"

class OrbScene : public State {
public:
    void handleInput() override;
    void update(float& dt) override;
    void render(sf::RenderWindow& window) override;
};