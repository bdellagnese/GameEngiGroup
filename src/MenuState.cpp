#include <SFML/Graphics.hpp>
#include "MenuState.h"
#include "GameVariables.h"

bool hasLoadedMenu = false;
void loadMenu();

// Objects
sf::Sprite DoorwaySpr;
sf::Sprite startSprite;
sf::Texture DoorwayTexture;
sf::Texture startTexture;
sf::Texture startHoverTexture;

sf::Sprite DebugSpr;

// Controls
const sf::Keyboard::Key controls[5] = {
	sf::Keyboard::W,  // Up
	sf::Keyboard::S,  // Down
	sf::Keyboard::A,  // Left
	sf::Keyboard::D,   // Right
	sf::Keyboard::Space   // PlaceMode
};

void MenuState::handleInput() { // Handle input for main menu
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
	// Check if has already loaded
	if (!hasLoadedMenu) {
		loadMenu();
	}

	// Press Delay
	if (pressTime > 0) {
		pressTime -= dt;
	}
	else {
		canPress = true;
	}

	// PLACE MODE - can be used for any sprite
	startSprite.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));

    // DEBUG TEXT - "(x,y) Placing: t/f"
    sf::Vector2f textPosition = startSprite.getPosition();

    text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
        std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode));
}

void MenuState::render(sf::RenderWindow& window) { // Render Scene
	// Mouse Input
	// transform the mouse position from window coordinates to world coordinates
	sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// retrieve the bounding box of the sprite
	sf::FloatRect bounds = startSprite.getGlobalBounds();

	// mouse hovering on startButton
	if (bounds.contains(mouse))
	{
		startSprite.setTexture(startHoverTexture);
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// start game
			startGame = true;
			hasLoadedMenu = false;
		}
	}
	else {
		startSprite.setTexture(startTexture);
	}

	//Bottom Layer - The background
	window.draw(DoorwaySpr);
	window.draw(startSprite);
	window.draw(text);
	//Top Layer - UI
}

void loadMenu() {
	hasLoadedMenu = true;

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

	// load startButton
	if (!startTexture.loadFromFile("Assets/Sprites/startButton.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	startSprite.setTexture(startTexture);
	startSprite.setPosition(748, 232);
	startSprite.setScale(0.5f, 0.5f);

	// load startButtonClick - when hovered or clicked on
	if (!startHoverTexture.loadFromFile("Assets/Sprites/startButtonClick.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}

	// load
}
