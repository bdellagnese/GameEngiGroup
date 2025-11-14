#include "MenuState.h"

bool hasLoaded = false;

// Objects
sf::Font font;
sf::Text text;

sf::Sprite DoorwaySpr;
sf::Texture DoorwayTexture;

sf::Sprite DebugSpr;

// Timers
bool placeMode = false;
bool canPress;
float pressTime;

// Parameters
const int gameWidth = 1920;
const int gameHeight = 1080;
const float placeModeSpeed = 25.f;
void load();

// handle Placement
float direction1 = 0.0f;
float direction2 = 0.0f;

// Controls
const sf::Keyboard::Key controls[5] = {
	sf::Keyboard::W,  // Up
	sf::Keyboard::S,  // Down
	sf::Keyboard::A,  // Left
	sf::Keyboard::D,   // Right
	sf::Keyboard::Space   // PlaceMode
};

void MenuState::handleInput() { // Handle input for main menu
	// Start Game
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::F))
	{
		startGame = true;
	}

	// Inputs for DEBUG PLACEMODE
	if (sf::Keyboard::isKeyPressed(controls[4]) && canPress) { // toggle placemode
		placeMode = !placeMode;
		canPress = false;
		pressTime = 1;
	}
	else if (sf::Keyboard::isKeyPressed(controls[0]) && placeMode) {
		if (direction1 > -placeModeSpeed) {
			direction1--;
		}
	}
	else if (sf::Keyboard::isKeyPressed(controls[1]) && placeMode) {
		if (direction1 < placeModeSpeed) {
			direction1++;
		}
	}
	else if (sf::Keyboard::isKeyPressed(controls[2]) && placeMode) {
		if (direction2 > -placeModeSpeed) {
			direction2--;
		}
	}
	else if (sf::Keyboard::isKeyPressed(controls[3]) && placeMode) {
		if (direction2 < placeModeSpeed) {
			direction2++;
		}
	}
	else {
		direction2 = 0; direction1 = 0;
	}
}

void MenuState::update(float& dt) {
	// Press Delay
	if (pressTime > 0) {
		pressTime -= dt;
	}
	else {
		canPress = true;
	}

	//PLACE MODE - can be used for any sprite
	DoorwaySpr.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));
    
    // Check if has already loaded
    if (!hasLoaded) {
		load();
    }

    // update debug position text
    sf::Vector2f textPosition = DoorwaySpr.getPosition();
    text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
        std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode));
}

void MenuState::render(sf::RenderWindow& window) {
    // Render main menu
	
    // Render Objects
	window.draw(DoorwaySpr);
	window.draw(text);
}

void load() {
	hasLoaded = true;

	// Load Font
	if (!font.loadFromFile("Assets/Fonts/arial.ttf")) {
		// Error loading font
	}
	text.setFont(font);
	text.setString("It's all meaningless!");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(5, 5);

	// load doorway
	if (!DoorwayTexture.loadFromFile("Assets/Sprites/TestDoorway.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	DoorwaySpr.setTexture(DoorwayTexture);
	DoorwaySpr.setPosition(0, 0);
}
