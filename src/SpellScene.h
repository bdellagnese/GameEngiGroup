#include "State.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SpellScene : public State {
public:
    void handleInput() override;
    void update(float& dt) override;
    void render(sf::RenderWindow& window) override;

    bool backSpell = false;
    void updatePageText();

    
private:
    sf::Texture spellBackgroundTexture;
    sf::Sprite spellBackground;


    sf::Font spellFont;
    sf::Text pageText;
    sf::Text pageText2;


    bool hasLoaded = false;
    void loadAssets();

    int currentPage = 0;
    std::vector<std::string> leftPageTexts;
    std::vector<std::string> rightPageTexts;
    

    bool canPress = true;
    float pressTime = 0.f;

private:
    sf::Texture backButtonTexture;
    sf::Sprite backButton;
};