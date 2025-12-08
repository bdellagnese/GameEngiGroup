#include <SFML/Graphics.hpp>
#include "OrbScene.h"
#include "GameVariables.h"

bool hasLoadedOrb = false;
void loadOrb();
bool backOrb;

//Initialises counters for mana regeneration
//These are needed to delay the speed of mana regeneration without
//retroactively having to switch currentMana to a float in all other files
int manaIncreaseCountP = 0;
int manaIncreaseCountY = 0;
int manaIncreaseCountG = 0;
int manaIncreaseCountR = 0;

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
	//if (sf::Keyboard::isKeyPressed(controls[4]) && canPress) { // toggle placemode
	//	placeMode = !placeMode;
	//	canPress = false;
	//	pressTime = 1;
	//}

	//Reads inputs, each directional key (WASD) resets a tubes liquid to the top to continue generating mana
	if (sf::Keyboard::isKeyPressed(controls[0]) && canPress) {
		pinkTubeSprite.setPosition(0, 0);
		canPress = false;
		pressTime = 1;
	}

	else if (sf::Keyboard::isKeyPressed(controls[1]) && canPress) {
		greenTubeSprite.setPosition(0, 0);
		canPress = false;
		pressTime = 1;
	}

	else if (sf::Keyboard::isKeyPressed(controls[2]) && canPress) {
		redTubeSprite.setPosition(0, 0);
		canPress = false;
		pressTime = 1;
	}

	else if (sf::Keyboard::isKeyPressed(controls[3]) && canPress) {
		yellowTubeSprite.setPosition(0, 0);
		canPress = false;
		pressTime = 1;
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

	// Global Timer and mana regeneration
	if (globalTime > 0) {

		//Gets position of tube contents
		sf::Vector2f positionTrackerPink = pinkTubeSprite.getPosition();

		//Checks if tube contents are above a certain point, if they are then their y position is lowered
		if (positionTrackerPink.y < 230) {

			//Moves tube contents down
			pinkTubeSprite.move(sf::Vector2f(0, 3 * placeModeSpeed * dt));

			//This section first counts up to 1000 through the associated manaIncreaseCount, when it reaches
			//1000 then currentMana is increased by 1 and the count is reset, this is done to slow down the 
			//speed of mana regeneration as without it your mana would regen almost instantly. Decimal points
			//couldn't be utilised as currentMana is stored as an int and it would be more effort to go back
			//and change all references of it to a float
			if (manaIncreaseCountP < 1000) {

				manaIncreaseCountP++;

			}

			else {

				if (currentMana < 100) {

					currentMana += 1;

					if (currentMana > 100) {

						currentMana = 100;

					}
				}
				manaIncreaseCountP = 0;
			}

		}

		sf::Vector2f positionTrackerGreen = greenTubeSprite.getPosition();

		if (positionTrackerGreen.y < 230) {

			greenTubeSprite.move(sf::Vector2f(0, 3 * placeModeSpeed * dt));

			if (manaIncreaseCountG < 1000) {

				manaIncreaseCountG++;

			}

			else {

				if (currentMana < 100) {

					currentMana += 1;

					if (currentMana > 100) {

						currentMana = 100;

					}
				}
				manaIncreaseCountG = 0;
			}

		}

		sf::Vector2f positionTrackerRed = redTubeSprite.getPosition();

		if (positionTrackerRed.y < 230) {

			redTubeSprite.move(sf::Vector2f(0, 3 * placeModeSpeed * dt));

			if (manaIncreaseCountR < 1000) {

				manaIncreaseCountR++;

			}

			else {

				if (currentMana < 100) {

					currentMana += 1;

					if (currentMana > 100) {

						currentMana = 100;

					}
				}
				manaIncreaseCountR = 0;
			}

		}

		sf::Vector2f positionTrackerYellow = yellowTubeSprite.getPosition();

		if (positionTrackerYellow.y < 230) {

			yellowTubeSprite.move(sf::Vector2f(0, 3 * placeModeSpeed * dt));

			if (manaIncreaseCountY < 1000) {

				manaIncreaseCountY++;

			}

			else {

				if (currentMana < 100) {

					currentMana += 1;

					if (currentMana > 100) {

						currentMana = 100;

					}
				}
				manaIncreaseCountY = 0;
			}
		}

		if (startTimer) {
			globalTime -= dt;
		}
	}
	else {
		// lose
		backOrb = true;
	}

	if (currentMana == maxMana) {
		startTimer = true;
	}

	//Displays global timer
	flameTimerText.setString(std::to_string(static_cast<int>(globalTime)));

	// Basic Timer
	if (pressTime > 0) {
		pressTime -= dt;
	}
	else {
		canPress = true;
	}

	// PLACE MODE - can be used for any sprite
	//pinkTubeSprite.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));

	// DEBUG TEXT - "(x,y) Placing: t/f"
	//sf::Vector2f textPosition = pinkTubeSprite.getPosition();

	//text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
	//	std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode));
}

void OrbScene::render(sf::RenderWindow& window) {
	// Render game
	//Bottom Layer - The background
	window.draw(OrbBgSprite);

	//Middle Layer - Tube liquids
	window.draw(greenTubeSprite);
	window.draw(pinkTubeSprite);
	window.draw(redTubeSprite);
	window.draw(yellowTubeSprite);

	//Top Layer - Copy of lower half of background, layered over the tube liquids
	//so they can move down below this layer and appear to be draining 
	window.draw(OrbBgBlockSprite);

	//Displays current mana
	window.draw(manaBgSpr);
	window.draw(manaText);

	//Displays global timer
	window.draw(flameBgSpr);
	window.draw(flameTimerText);
}

void loadOrb() {
	hasLoadedOrb = true;
	backOrb = false;

	// Loads all assets
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
	greenTubeSprite.setPosition(0, 230);

	if (!pinkTubeTexture.loadFromFile("Assets/Sprites/Orb/pinkTube.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	pinkTubeSprite.setTexture(pinkTubeTexture);
	pinkTubeSprite.setPosition(0, 230);

	if (!redTubeTexture.loadFromFile("Assets/Sprites/Orb/redTube.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	redTubeSprite.setTexture(redTubeTexture);
	redTubeSprite.setPosition(0, 230);

	if (!yellowTubeTexture.loadFromFile("Assets/Sprites/Orb/yellowTube.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	yellowTubeSprite.setTexture(yellowTubeTexture);
	yellowTubeSprite.setPosition(0, 230);
}
