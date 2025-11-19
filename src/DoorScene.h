#include "State.h"

class DoorScene : public State {
public:
    void handleInput() override;
    void update(float& dt) override;
    void render(sf::RenderWindow& window) override;

    bool backDoor;
};