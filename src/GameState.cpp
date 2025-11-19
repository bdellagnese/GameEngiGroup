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

sf::Sprite behindDoorSprite;
sf::Texture behindDoorTexture;

sf::Sprite orbSprite;
sf::Texture orbTexture;
sf::Texture orbTextureHover;

sf::Sprite bookSprite;
sf::Texture bookTexture;
sf::Texture bookTextureHover;

sf::Sprite charSilhouetteSprite;
sf::Texture charSilhouetteTexture;

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

	// Global Timer
	if (globalTime > 0) {
		globalTime -= dt;
	}
	else {
		// lose
	}
	flameTimerText.setString(std::to_string(static_cast<int>(globalTime)));

    // Basic Timer
    if (pressTime > 0) {
        pressTime -= dt;
    }
    else {
		canPress = true;
		canClick = true;
    }

    // PLACE MODE - can be used for any sprite
	flameTimerText.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));
    // DEBUG TEXT - "(x,y) Placing: t/f"
    sf::Vector2f textPosition = flameTimerText.getPosition();

	text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
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
	else if (orbBounds.contains(mouse) && canClick)
	{
		orbSprite.setTexture(orbTextureHover);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			stateChange = 2; // ORB
		}
	}
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
		bookSprite.setPosition(225, 342);
	}

	//Bottom Layer - The background
	window.draw(behindDoorSprite);
	window.draw(backgroundSprite);
	if (characterArrived)
	{
		window.draw(charSilhouetteSprite);
	}
	window.draw(doorSprite);
	window.draw(orbSprite);
	window.draw(bookSprite);
	window.draw(text);
	window.draw(flameTimerText);
	//Top Layer - UI
}

void loadGame() {
	hasLoadedGame = true;
	characterArrived = true;

	// Load global timer font
	if (!flameTimerFont.loadFromFile("Assets/Fonts/lacquer.ttf")) {
		// Error loading font
	}
	flameTimerText.setFont(flameTimerFont);
	flameTimerText.setString("test");
	flameTimerText.setCharacterSize(50);
	flameTimerText.setFillColor(sf::Color::Black);
	flameTimerText.setPosition(1840, 10);

	globalTime = 100;

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

	// load behind door
	if (!behindDoorTexture.loadFromFile("Assets/Sprites/doorwayBG.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	behindDoorSprite.setTexture(behindDoorTexture);
	behindDoorSprite.setScale(0.5f, 0.5f);
	behindDoorSprite.setPosition(612, 108);

	// load orb
	if (!orbTexture.loadFromFile("Assets/Sprites/gameOrb.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	if (!orbTextureHover.loadFromFile("Assets/Sprites/gameOrbHover.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	orbSprite.setTexture(orbTexture);
	orbSprite.setPosition(1324, 319);

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

	if (!charSilhouetteTexture.loadFromFile("Assets/Sprites/charSilhouette.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	charSilhouetteSprite.setTexture(charSilhouetteTexture);
	charSilhouetteSprite.setPosition(858, 250);
}
