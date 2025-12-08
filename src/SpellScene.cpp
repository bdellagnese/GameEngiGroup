#include <SFML/Graphics.hpp>
#include "SpellScene.h"
#include "GameVariables.h"

bool backSpell;
bool hasLoaded = false;

// Objects
sf::Sprite spellPlaceholder;

sf::Sprite bgSprite;
sf::Sprite altarSprite;

sf::Texture bgTexture;
sf::Texture altarTexture;

// Controls
const sf::Keyboard::Key controls[6] = {
	sf::Keyboard::W,  // Up
	sf::Keyboard::S,  // Down
	sf::Keyboard::A,  // Left
	sf::Keyboard::D,   // Right
	sf::Keyboard::Space,   // PlaceMode
	sf::Keyboard::E,   // go back
};

void SpellScene::handleInput() {
	// Handle input for game
	// Inputs for DEBUG PLACEMODE
	if (sf::Keyboard::isKeyPressed(controls[4]) && canPress) { // toggle placemode
		placeMode = !placeMode;
		//canPress = false;
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
		backSpell = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && canPress) {
		currentPage++;
		updatePageText();
		canPress = false;
		pressTime = 0.25f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && canPress) {
		currentPage--;
		updatePageText();
		canPress = false;
		pressTime = 0.25f;
	}
}

void SpellScene::update(float& dt) {
	// Update game logic
	if (!hasLoaded) {
		loadAssets();
	}
	
	// Global Timer
	if (globalTime > 0) {
		globalTime -= dt;
	}
	else {
		// lose
		backSpell = true;
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
	spellPlaceholder.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));

	// DEBUG TEXT - "(x,y) Placing: t/f"
	sf::Vector2f textPosition = spellPlaceholder.getPosition();

	text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
		std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode));
}

void SpellScene::render(sf::RenderWindow& window) {
	// Render game

	//Bottom Layer - The background
	window.draw(bgSprite);
	window.draw(altarSprite);

	window.draw(manaBgSpr);
	window.draw(manaText);

	window.draw(flameBgSpr);
	window.draw(flameTimerText);
	window.draw(text);
	window.draw(pageText);
	window.draw(pageText2);
	//Top Layer - UI
}

void SpellScene::loadAssets() {
	hasLoaded = true;
	backSpell = false;

	// load background
	if (!bgTexture.loadFromFile("Assets/Sprites/SceneBackground.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	if (!altarTexture.loadFromFile("Assets/Sprites/Spellbook.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	bgSprite.setTexture(bgTexture);
	altarSprite.setTexture(altarTexture);
	bgSprite.setPosition(0, 0);
	altarSprite.setPosition(56, 38);
	altarSprite.setScale(0.9f, 0.9f);

	if (!spellFont.loadFromFile("Assets/Fonts/lacquer.ttf"))
	{
		printf("--ERROR LOADING FONT--");
	}
	if (!spellFont.loadFromFile("Assets/Fonts/lacquer.ttf"))
	{
		printf("--ERROR LOADING FONT--");
	}
	pageText.setFont(spellFont);
	pageText.setCharacterSize(40);
	pageText.setFillColor(sf::Color::Black);
	pageText.setPosition(56 + 340, 38 + 250);

	

	pageText2.setFont(spellFont);
	pageText2.setCharacterSize(67);
	pageText2.setFillColor(sf::Color::Black);
	pageText2.setPosition(56 + 910, 38 + 300);

	leftPageTexts = {
	"Shrink\nMakes enemies smaller.",
	"Gild\nWhatever this spell\n is cast upon is \nturned to gold.",
	"Bark Skin\nReinforces the \nspell targets skin \nto make them more\n resistent to harm.",
	"Curse\nCurses a customers\n object, results will\n vary depending on \nthe target.",
	
	};

	rightPageTexts = {
		"<  >  V  ^  V",
		"^  V  ^  <  ^",
		"V  <  >  >  >",
		"^  V  >  <  V",
	};
	updatePageText();

}
void SpellScene::updatePageText() {
    if (currentPage < 0) currentPage = 0;
    if (currentPage >= leftPageTexts.size()) 
        currentPage = leftPageTexts.size() - 1;

    pageText.setString(leftPageTexts[currentPage]);
    pageText2.setString(rightPageTexts[currentPage]);
}