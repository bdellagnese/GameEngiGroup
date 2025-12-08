#include <SFML/Graphics.hpp>
#include "PauseScene.h"
#include "GameVariables.h"

bool hasLoadedPause = false;
void loadPause();
bool backPause;

bool unpause = false;
bool quitGame = false;
bool optionsOpen = false;

// Objects
sf::Sprite pausePlaceholder;

sf::Sprite bgSpr;
sf::Texture bgTxt;

sf::Sprite resumeSprite;
sf::Sprite optionsSprite;
sf::Sprite quitSprite;

sf::Texture resumeTexture;
sf::Texture resumeHoverTexture;

sf::Texture optionsTexture;
sf::Texture optionsHoverTexture;

sf::Texture quitTexture;
sf::Texture quitHoverTexture;

// Controls
const sf::Keyboard::Key controls[6] = {
	sf::Keyboard::W,  // Up
	sf::Keyboard::S,  // Down
	sf::Keyboard::A,  // Left
	sf::Keyboard::D,   // Right
	sf::Keyboard::Space,   // PlaceMode
	sf::Keyboard::E,   // go back
};

void PauseScene::handleInput() {
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

void PauseScene::update(float& dt) {
	// Update game logic
	if (!hasLoadedPause) {
		loadPause();
	}

	// Basic Timer
	if (pressTime > 0) {
		pressTime -= dt;
	}
	else {
		canPress = true;
	}

	// PLACE MODE - can be used for any sprite
	pausePlaceholder.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));

	// DEBUG TEXT - "(x,y) Placing: t/f"
	sf::Vector2f textPosition = pausePlaceholder.getPosition();

	text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
		std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode));
}

void PauseScene::render(sf::RenderWindow& window) {
	// Render game
	// transform the mouse position from window coordinates to world coordinates
	sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// retrieve the bounding box of the sprite
	sf::FloatRect resumeBounds = resumeSprite.getGlobalBounds();
	sf::FloatRect optionsBounds = optionsSprite.getGlobalBounds();
	sf::FloatRect quitBounds = quitSprite.getGlobalBounds();

	// Handle mouse inputs in and out of options
	if (!optionsOpen) {
		if (resumeBounds.contains(mouse) && canPress)
		{
			resumeSprite.setTexture(resumeHoverTexture);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				unpause = true;
				canPress = false;
				pressTime = 1;
			}
		}
		else if (optionsBounds.contains(mouse) && canPress)
		{
			optionsSprite.setTexture(optionsHoverTexture);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				//optionsOpen = true;
			}
		}
		else if (quitBounds.contains(mouse) && canPress)
		{
			quitSprite.setTexture(quitHoverTexture);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				quitGame = true;
			}
		}
		else {
			resumeSprite.setTexture(resumeTexture);
			optionsSprite.setTexture(optionsTexture);
			quitSprite.setTexture(quitTexture);
		}
	}
	else {

	}

	//Bottom Layer - The background
	window.draw(bgSpr);

	if (!optionsOpen) {
		window.draw(resumeSprite);
		window.draw(optionsSprite);
		window.draw(quitSprite);
	}
	else {
		// rebind WASD + E
		// window.draw();
	}
	//Top Layer - UI
}

void loadPause() {
	hasLoadedPause = true;
	backPause = false;

	// load resume
	if (!resumeTexture.loadFromFile("Assets/Sprites/PauseMenu/Resume.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	if (!resumeHoverTexture.loadFromFile("Assets/Sprites/PauseMenu/ResumeSel.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	resumeSprite.setTexture(resumeTexture);
	resumeSprite.setPosition(748, 250);
	resumeSprite.setScale(0.5f, 0.5f);

	// load doorway
	if (!bgTxt.loadFromFile("Assets/Sprites/SceneBackground.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	bgSpr.setTexture(bgTxt);
	bgSpr.setPosition(0, 0);

	// load options
	if (!optionsTexture.loadFromFile("Assets/Sprites/PauseMenu/Options.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	if (!optionsHoverTexture.loadFromFile("Assets/Sprites/PauseMenu/OptionsSel.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	optionsSprite.setTexture(optionsTexture);
	optionsSprite.setPosition(210, 250);
	optionsSprite.setScale(0.5f, 0.5f);

	// load quit
	if (!quitTexture.loadFromFile("Assets/Sprites/PauseMenu/Quit.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	if (!quitHoverTexture.loadFromFile("Assets/Sprites/PauseMenu/QuitSel.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	quitSprite.setTexture(quitTexture);
	quitSprite.setPosition(1264, 250);
	quitSprite.setScale(0.5f, 0.5f);
}
