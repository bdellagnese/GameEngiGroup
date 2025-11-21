#include "State.h"

class SpellScene : public State {
public:
    void handleInput() override;
    void update(float& dt) override;
    void render(sf::RenderWindow& window) override;

    bool backSpell;
};