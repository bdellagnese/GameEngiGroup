#include <SFML/Graphics.hpp>
#include "OrbScene.h"
#include "GameVariables.h"

bool hasLoadedOrb = false;
void loadOrb();

// Objects
sf::Sprite orbPlaceholder;

// Controls
const sf::Keyboard::Key controls[5] = {
	sf::Keyboard::W,  // Up
	sf::Keyboard::S,  // Down
	sf::Keyboard::A,  // Left
	sf::Keyboard::D,   // Right
	sf::Keyboard::Space   // PlaceMode
};

void OrbScene::handleInput() {
	// Handle input for game
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

void OrbScene::update(float& dt) {
	// Update game logic
	if (!hasLoadedOrb) {
		loadOrb();
	}

	// Basic Timer
	if (pressTime > 0) {
		pressTime -= dt;
	}
	else {
		canPress = true;
	}

	// PLACE MODE - can be used for any sprite
	orbPlaceholder.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));

	// DEBUG TEXT - "(x,y) Placing: t/f"
	sf::Vector2f textPosition = orbPlaceholder.getPosition();

	text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
		std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode));
}

void OrbScene::render(sf::RenderWindow& window) {
	// Render game
}

void loadOrb() {
	hasLoadedOrb = true;

	// Load values in GameVariables.h
	bool placeMode = false;
	bool canPress = true;
	float pressTime = 0.0f;
	const float placeModeSpeed = 25.0f;
	const int gameWidth = 1920;
	const int gameHeight = 1080;
	float direction1 = 0.0f;
	float direction2 = 0.0f;
	sf::Font font;
	sf::Text text;
}
