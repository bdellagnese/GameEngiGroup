#include <SFML/Graphics.hpp>
#include <string>
#include "GameVariables.h"
#include "GameState.h"

bool hasLoadedGame = false;
void loadGame();

bool canClick = false;

// Objects
sf::Sprite gamePlaceholder;

sf::Sprite backgroundSprite;
sf::Texture backgroundTexture;

sf::Sprite doorSprite;
sf::Texture doorTexture;
sf::Texture doorTextureHover;

sf::Sprite orbSprite;
sf::Texture orbTexture;
sf::Texture orbTextureHover;

sf::Sprite bookSprite;
sf::Texture bookTexture;
sf::Texture bookTextureHover;

sf::Text debugText;
sf::Font debugFont;

// Controls
const sf::Keyboard::Key controls[5] = {
	sf::Keyboard::W,  // Up
	sf::Keyboard::S,  // Down
	sf::Keyboard::A,  // Left
	sf::Keyboard::D,   // Right
	sf::Keyboard::Space   // PlaceMode
};

void GameState::handleInput() {
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

void GameState::update(float& dt) {
	// Update game logic
    if (!hasLoadedGame) {
		pressTime = 1;
		loadGame();
    }

    // Basic Timer
    if (pressTime > 0) {
        pressTime -= dt;
    }
    else {
		canClick = true;
    }

    // PLACE MODE - can be used for any sprite
	bookSprite.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));
    // DEBUG TEXT - "(x,y) Placing: t/f"
    sf::Vector2f textPosition = bookSprite.getPosition();

	debugText.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
        std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode));
}

void GameState::render(sf::RenderWindow& window) {
    // Render game

	// transform the mouse position from window coordinates to world coordinates
	sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// retrieve the bounding box of the sprite
	sf::FloatRect doorBounds = doorSprite.getGlobalBounds();
	sf::FloatRect orbBounds = orbSprite.getGlobalBounds();
	sf::FloatRect bookBounds = bookSprite.getGlobalBounds();

	// mouse hovering on startButton
	if (doorBounds.contains(mouse) && canClick)
	{
		doorSprite.setTexture(doorTextureHover);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			stateChange = 1; // DOOR
		}
	}
	/*else if (orbBounds.contains(mouse) && canClick)
	{
		orbSprite.setTexture(orbTextureHover);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			stateChange = 2; // ORB
		}
	}*/
	else if (bookBounds.contains(mouse) && canClick)
	{
		bookSprite.setTexture(bookTextureHover);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			stateChange = 3; // BOOK
		}
	}
	else {
		doorSprite.setTexture(doorTexture);
		orbSprite.setTexture(orbTexture);
		bookSprite.setTexture(bookTexture);
	}

	//Bottom Layer - The background
	window.draw(backgroundSprite);
	window.draw(doorSprite);
	window.draw(orbSprite);
	window.draw(bookSprite);
	window.draw(debugText);
	//Top Layer - UI
}

void loadGame() {
	hasLoadedGame = true;

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
	if (!debugFont.loadFromFile("Assets/Fonts/arial.ttf")) {
		// Error loading font
	}
	debugText.setFont(debugFont);
	debugText.setCharacterSize(24);
	debugText.setFillColor(sf::Color::White);
	debugText.setPosition(10, 10);

	// load door
	if (!doorTexture.loadFromFile("Assets/Sprites/gameDoor.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	if (!doorTextureHover.loadFromFile("Assets/Sprites/gameDoorHover.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	doorSprite.setTexture(doorTexture);
	doorSprite.setPosition(763, 136);

	/* load orb
	if (!orbTexture.loadFromFile("Assets/Sprites/gameOrb.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	if (!orbTextureHover.loadFromFile("Assets/Sprites/gameOrbHover.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	orbSprite.setTexture(orbTexture);
	orbSprite.setPosition(0, 0);*/

	// load spellbook
	if (!bookTexture.loadFromFile("Assets/Sprites/gameBook.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	if (!bookTextureHover.loadFromFile("Assets/Sprites/gameBookHover.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	bookSprite.setTexture(bookTexture);
	bookSprite.setPosition(225, 342);

	if (!backgroundTexture.loadFromFile("Assets/Sprites/gameBackground.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	backgroundSprite.setTexture(backgroundTexture);
}
