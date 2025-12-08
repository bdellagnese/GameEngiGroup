#include <SFML/Graphics.hpp>
#include "OrbScene.h"
#include "GameVariables.h"

bool hasLoadedOrb = false;
void loadOrb();
bool backOrb;

// Objects
sf::Sprite orbPlaceholder;

sf::Sprite OrbBgSprite;
sf::Texture OrbBgTexture;

sf::Sprite OrbBgBlockSprite;
sf::Texture OrbBgBlockTexture;

sf::Sprite pinkTubeSprite;
sf::Texture pinkTubeTexture;

sf::Sprite yellowTubeSprite;
sf::Texture yellowTubeTexture;

sf::Sprite greenTubeSprite;
sf::Texture greenTubeTexture;

sf::Sprite redTubeSprite;
sf::Texture redTubeTexture;

// Controls
const sf::Keyboard::Key controls[6] = {
	sf::Keyboard::W,  // Up
	sf::Keyboard::S,  // Down
	sf::Keyboard::A,  // Left
	sf::Keyboard::D,   // Right
	sf::Keyboard::Space,   // PlaceMode
	sf::Keyboard::E,   // go back
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

	// Go back to game screen
	if (sf::Keyboard::isKeyPressed(controls[5]) && canPress)
	{
		// Reset Press Timer
		canPress = false;
		pressTime = 1;

		// go back
		backOrb = true;
	}
}

void OrbScene::update(float& dt) {
	// Update game logic
	if (!hasLoadedOrb) {
		loadOrb();
	}

	manaText.setString(std::to_string(static_cast<int>(currentMana)));

	// Global Timer
	if (globalTime > 0) {
		globalTime -= dt;
	}
	else {
		// lose
		backOrb = true;
	}
	flameTimerText.setString(std::to_string(static_cast<int>(globalTime)));

	// Basic Timer
	if (pressTime > 0) {
		pressTime -= dt;
	}
	else {
		canPress = true;
	}

	// PLACE MODE - can be used for any sprite
	pinkTubeSprite.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));

	// DEBUG TEXT - "(x,y) Placing: t/f"
	sf::Vector2f textPosition = pinkTubeSprite.getPosition();

	text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
		std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode));
}

void OrbScene::render(sf::RenderWindow& window) {
	// Render game


	//Bottom Layer - The background
	window.draw(OrbBgSprite);

	window.draw(greenTubeSprite);
	window.draw(pinkTubeSprite);
	window.draw(redTubeSprite);
	window.draw(yellowTubeSprite);
	window.draw(OrbBgBlockSprite);

	window.draw(text);

	window.draw(manaBgSpr);
	window.draw(manaText);

	window.draw(flameBgSpr);
	window.draw(flameTimerText);
	//Top Layer - UI
}

void loadOrb() {
	hasLoadedOrb = true;
	backOrb = false;

	// load background
	if (!OrbBgTexture.loadFromFile("Assets/Sprites/Orb/orbFullAssets.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	OrbBgSprite.setTexture(OrbBgTexture);
	OrbBgSprite.setPosition(0, 0);

	if (!OrbBgBlockTexture.loadFromFile("Assets/Sprites/Orb/orbTableBlock.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	OrbBgBlockSprite.setTexture(OrbBgBlockTexture);
	OrbBgBlockSprite.setPosition(0, 0);

	if (!greenTubeTexture.loadFromFile("Assets/Sprites/Orb/greenTube.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	greenTubeSprite.setTexture(greenTubeTexture);
	greenTubeSprite.setPosition(0, 0);

	if (!pinkTubeTexture.loadFromFile("Assets/Sprites/Orb/pinkTube.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	pinkTubeSprite.setTexture(pinkTubeTexture);
	pinkTubeSprite.setPosition(0, 0);

	if (!redTubeTexture.loadFromFile("Assets/Sprites/Orb/redTube.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	redTubeSprite.setTexture(redTubeTexture);
	redTubeSprite.setPosition(0, 0);

	if (!yellowTubeTexture.loadFromFile("Assets/Sprites/Orb/yellowTube.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	yellowTubeSprite.setTexture(yellowTubeTexture);
	yellowTubeSprite.setPosition(0, 0);
}
