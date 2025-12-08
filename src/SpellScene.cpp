#include <SFML/Graphics.hpp>
#include "SpellScene.h"
#include "GameVariables.h"
#include "DoorScene.h"

bool backSpell;
bool hasLoaded = false;

DoorScene doorScene2;

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
	if (startTimer) {
		if (globalTime > 0) {
			globalTime -= dt;
		}
		else {
			// lose
			backSpell = true;
		}
	}

	// Random Arrival Timer
	if (startTimer) {
		if (!characterArrived) {
			if (randomTime > 0) {
				randomTime -= dt;
			}
			else {
				if (!characterArrived) {
					doorScene2.nextCharacter();
					pressTime = 1;
				}
			}
		}
	}

	// Timer for animation pauses
	if (animTimer > 0) {
		animTimer -= dt;
	}
	else {
		animTimerDone = true;
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
		printf("--ERROR LOADING ASSETS--");
	}
	//load spelbook texture
	if (!altarTexture.loadFromFile("Assets/Sprites/Spellbook.tga"))
	{
		printf("--ERROR LOADING ASSETS--");
	}

	//aply textures to sprites
	bgSprite.setTexture(bgTexture);
	altarSprite.setTexture(altarTexture);
	bgSprite.setPosition(0, 0); //setting positions
	altarSprite.setPosition(56, 38);
	altarSprite.setScale(0.9f, 0.9f); //scale down a bit

	//load font
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

	
	//instructions setup
	pageText2.setFont(spellFont);
	pageText2.setCharacterSize(67);
	pageText2.setFillColor(sf::Color::Black);
	pageText2.setPosition(56 + 910, 38 + 300);

	//text on left page
	leftPageTexts = {
	"Shrink\nMakes enemies smaller. Use this on customers\nthat require shrinkage",
	"Gild\nWhatever this spell\n is cast upon is \nturned to gold.",
	"Bark Skin\nReinforces the \nspell targets skin \nto make them more\n resistent to harm.",
	"Curse\nCurses a customers\n object, results will\n vary depending on \nthe target.",
	
	};

	//keybind instructions
	rightPageTexts = {
		"<  >  V  ^  V",
		"^  V  ^  <  ^",
		"V  <  >  >  >",
		"^  V  >  <  V",
	};
	updatePageText();

}

//
void SpellScene::updatePageText() {
    if (currentPage < 0) currentPage=0;
    if (currentPage >= leftPageTexts.size()) 
        currentPage = leftPageTexts.size()-1;

    pageText.setString(leftPageTexts[currentPage]);
    pageText2.setString(rightPageTexts[currentPage]);
}