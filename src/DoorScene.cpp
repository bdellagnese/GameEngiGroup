#include <SFML/Graphics.hpp>
#include <string>

#include "DoorScene.h"
#include "GameVariables.h"
#include "GameState.h"

GameState gameState;

bool newCharacter;
bool hasLoadedDoor = false;
bool doneCasting;
bool backDoor;

// Parameters
int cast[5];
int correctCast[5];
const int totalCharacters = 2;

// character textures and values - if one changes, they all do
int renderNum;
sf::Texture characterTexture[totalCharacters];
sf::Texture characterHappyTexture[totalCharacters];
sf::Texture characterSadTexture[totalCharacters];

sf::Texture character3DTexture[totalCharacters];
sf::Texture characterHappy3DTexture[totalCharacters];
sf::Texture characterSad3DTexture[totalCharacters];

int castPosition = 0;

bool success;

// Objects
sf::Sprite doorPlaceholder;

sf::Sprite doorframeSpr;
sf::Texture doorframeTexture;

sf::Sprite doorframeBgSpr;
sf::Texture doorframeBgTexture;

sf::Sprite characterSpr;
sf::Sprite character3DSpr;

sf::Sprite textboxSpr;
sf::Texture textboxTexture;

sf::Sprite spellBannerSpr;
sf::Texture spellBannerTexture;

sf::Texture spellUpTexture;
sf::Texture spellDownTexture;
sf::Texture spellLeftTexture;
sf::Texture spellRightTexture;
sf::Texture blankTexture;

sf::Sprite SpellSpr[5];
sf::Texture currentSpellTexture;

sf::Text characterText;
sf::Font characterFont;
sf::String characterString[4];
int currentString;

void loadDoor();
void loadCharacters();
void characterHandling();
void casting(int direction);

// Controls
const sf::Keyboard::Key controls[6] = {
	sf::Keyboard::W,  // Up
	sf::Keyboard::S,  // Down
	sf::Keyboard::A,  // Left
	sf::Keyboard::D,   // Right
	sf::Keyboard::Space,   // PlaceMode
	sf::Keyboard::E,   // go back
};

void DoorScene::handleInput() {
	// Handle input for game
	// Inputs for DEBUG PLACEMODE
	if (sf::Keyboard::isKeyPressed(controls[4]) && canPress) { // toggle placemode
		placeMode = !placeMode;
		canPress = false;
		pressTime = 1;
	}
	else if (sf::Keyboard::isKeyPressed(controls[0]) && placeMode) {
		if (direction1 > -placeModeSpeed) {
			canPress = false;
			pressTime = 1;
			direction1--;
		}
	}
	else if (sf::Keyboard::isKeyPressed(controls[1]) && placeMode) {
		if (direction1 < placeModeSpeed) {
			canPress = false;
			pressTime = 1;
			direction1++;
		}
	}
	else if (sf::Keyboard::isKeyPressed(controls[2]) && placeMode) {
		if (direction2 > -placeModeSpeed) {
			canPress = false;
			pressTime = 1;
			direction2--;
		}
	}
	else if (sf::Keyboard::isKeyPressed(controls[3]) && placeMode) {
		if (direction2 < placeModeSpeed) {
			canPress = false;
			pressTime = 1;
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
		backDoor = true;
	}

	// Dialogue
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && canPress) // Advance Dialogue
	{
		if (currentString == 0) {
			currentString = 1;
			characterText.setString(characterString[currentString]);
		}
	}

	// Spell Casting Input
	if (canPress && !doneCasting && characterArrived) {
		if (sf::Keyboard::isKeyPressed(controls[0])) { //up
			currentSpellTexture = spellUpTexture;
			casting(1);
		}
		else if (sf::Keyboard::isKeyPressed(controls[1])) { //down
			currentSpellTexture = spellDownTexture;
			casting(2);
		}
		else if (sf::Keyboard::isKeyPressed(controls[2])) { //left
			currentSpellTexture = spellLeftTexture;
			casting(3);
		}
		else if (sf::Keyboard::isKeyPressed(controls[3])) { //right
			casting(4);
		}
	}
}

void casting(int direction) {
	canPress = false;
	pressTime = 0.25f;

	if (castPosition < 5) 
	{
		cast[castPosition] = direction;
		if (direction == 1) {
			SpellSpr[castPosition].setTexture(spellUpTexture);
		}
		else if (direction == 2) {
			SpellSpr[castPosition].setTexture(spellDownTexture);
		}
		else if (direction == 3) {
			SpellSpr[castPosition].setTexture(spellLeftTexture);
		}
		else if (direction == 4) {
			SpellSpr[castPosition].setTexture(spellRightTexture);
		}
		
		int move = castPosition * 100;
		
		SpellSpr[castPosition].setPosition(1222 + move, 805);
		castPosition++;
	}
	
	if (castPosition == 5) 
	{
		doneCasting = true;
	}
}

void DoorScene::update(float& dt) {
	// Update game logic
	if (!hasLoadedDoor) {
		loadDoor();
	}

	if (totalCharacters < character) {
		//if (wins / totalCharacters > totalCharacters / 2){
		
		// Game Win
		//back
		gameDone = true;
		backDoor = true;
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
	}

	// Timer for animation pauses
	if (animTimer > 0) {
		animTimer -= dt;
	}
	else {
		animTimerDone = true;
	}

	// Show the customer after random timer
	if (!characterArrived) {
		if (randomTime > 0) {
			randomTime -= dt;
		}
		else {
			if (!characterArrived) {
				DoorScene::nextCharacter();
			}
		}
	}

	characterHandling();

	// PLACE MODE - can be used for any sprite
	characterText.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));
	// DEBUG TEXT - "(x,y) Placing: t/f"
	sf::Vector2f textPosition = characterText.getPosition();

	text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
		std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode) + 
		", Arrived: " + std::to_string(characterArrived));
}

void DoorScene::render(sf::RenderWindow& window) {
	// Render game
	//Bottom Layer - The background
	window.draw(doorframeBgSpr);

	if (characterArrived) {
		window.draw(characterSpr);
	}

	window.draw(doorframeSpr);
	
	if (characterArrived) {
		window.draw(character3DSpr);
		window.draw(textboxSpr);
		characterText.setString(characterString[currentString]);
		window.draw(characterText);
	}

	// Spells and Runes
	window.draw(spellBannerSpr);
	window.draw(SpellSpr[0]);
	window.draw(SpellSpr[1]);
	window.draw(SpellSpr[2]);
	window.draw(SpellSpr[3]);
	window.draw(SpellSpr[4]);

	// Text
	window.draw(text);
	window.draw(flameTimerText);
	//Top Layer - UI
}

void characterHandling() {
	//if()
	
	int getCastLength = sizeof(cast) / sizeof(cast[0]);
	if (doneCasting)
	{
		doneCasting = false;

		// reset timer
		canPress = false;
		pressTime = 1;
		
		int check = 0;
		int x = 0;

		// check if arrays are same
		for (int num : cast) {
			if (num == correctCast[x]) {
				// next number
				check++;
			}
			x++;
		}

		if (check == 5) {
			// win
			animTimerDone = false;
			animTimer = 4;

			success = true;
		}
		else {
			// fail
			animTimerDone = false;
			animTimer = 4;
			success = false;
		}
		pressTime = 5;
		castPosition = 0;

		if (success) {
			characterSpr.setTexture(characterHappyTexture[renderNum]);
			character3DSpr.setTexture(characterHappy3DTexture[renderNum]);
			currentString = 2; // Happy Message
		}
		else
		{
			characterSpr.setTexture(characterSadTexture[renderNum]);
			character3DSpr.setTexture(characterSad3DTexture[renderNum]);
			currentString = 3; // Hateful
		}
	}

	if (animTimerDone) {
		animTimerDone = false;
		animTimer = 1000;
		
		gameState.random();
		characterArrived = false;

		// clear spells
		for (int i = 0; i < 5; i++) {
			SpellSpr[i].setTexture(blankTexture);
		}
	}
}

void DoorScene::nextCharacter() {
	characterArrived = true;
	character++;

	currentString = 0;

	renderNum = character - 1;
	if (character == 1) // Timmy
	{
		// Change character texture
		character3DSpr.setTexture(character3DTexture[renderNum]);
		characterSpr.setTexture(characterTexture[renderNum]);

		// Adjust position
		characterSpr.setPosition(411, 160);
		character3DSpr.setPosition(characterSpr.getPosition());

		//Set text for character
		characterString[0] = "Hello mister wizard man!\nI was hoping you could maybe help me\n      out before my mum finds out I went\n      into the enchanted forest?";
		characterString[1] = "I was hangin' out in the forest by\nmy house and there were these pretty\n          orange roots that smelled\n                         SO yummy!\n          anyways I think it gave me\n          a growth spurt?";
		characterString[2] = "               THANKS!\n     I honestly wasn’t sure\n     that you were even a wizard";
		characterString[3] = "uhh I really don’t think this is\n     what I’m needing… \n     thanks for trying I guess";

		// The custom cast order needed for success
		correctCast[0] = 1; // 1up 2down 3left 4right
		correctCast[1] = 2;
		correctCast[2] = 3;
		correctCast[3] = 4;
		correctCast[4] = 1;
	}
	else if (character == 2) // Shawnson
	{
		// Change character texture
		character3DSpr.setTexture(character3DTexture[renderNum]);
		characterSpr.setTexture(characterTexture[renderNum]);

		// Adjust position
		characterSpr.setPosition(150, 193);
		character3DSpr.setPosition(characterSpr.getPosition());

		//Set text for character
		characterString[0] = "Greeting but Shawnson";
		characterString[1] = "Wait for Spell but Shawnson";
		characterString[2] = "Thankful Message but Shawnson";
		characterString[3] = "Hateful Message but Shawnson";

		// The custom cast order needed for success
		correctCast[0] = 1; // 1up 2down 3left 4right
		correctCast[1] = 2;
		correctCast[2] = 3;
		correctCast[3] = 4;
		correctCast[4] = 1;
	}
	else if (character == 3) // Shawnson
	{

	}
}

void loadDoor() {
	hasLoadedDoor = true;
	animTimer = 1000;
	animTimerDone = false;
	backDoor = false;

	if (!characterFont.loadFromFile("Assets/Fonts/RockSalt.ttf"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	characterText.setFont(characterFont);
	characterText.setCharacterSize(30);
	characterText.setFillColor(sf::Color::White);
	//characterText.setOutlineThickness(1);
	characterText.setPosition(1159, 158);

	// load doorway
	if (!doorframeTexture.loadFromFile("Assets/Sprites/Doorway.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	doorframeSpr.setTexture(doorframeTexture);
	doorframeSpr.setPosition(0, 0);

	// load doorwayBG
	if (!doorframeBgTexture.loadFromFile("Assets/Sprites/doorwayBG.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	doorframeBgSpr.setTexture(doorframeBgTexture);
	doorframeBgSpr.setPosition(0, 0);

	// load TextBox
	if (!textboxTexture.loadFromFile("Assets/Sprites/textbox.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	textboxSpr.setTexture(textboxTexture);
	textboxSpr.setPosition(884, 0);

	// load SpellBanner
	if (!spellBannerTexture.loadFromFile("Assets/Sprites/Spells/SpellBanner.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	spellBannerSpr.setTexture(spellBannerTexture);
	spellBannerSpr.setPosition(1164, 771);

	// load SpellDown
	if (!spellDownTexture.loadFromFile("Assets/Sprites/Spells/SpellDown.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}

	// load SpellLeft
	if (!spellLeftTexture.loadFromFile("Assets/Sprites/Spells/SpellLeft.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}

	// load SpellRight
	if (!spellRightTexture.loadFromFile("Assets/Sprites/Spells/SpellRight.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}

	// load SpellUp
	if (!spellUpTexture.loadFromFile("Assets/Sprites/Spells/SpellUp.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}

	if (!blankTexture.loadFromFile("Assets/Sprites/Spells/blankTexture.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	loadCharacters();
}

void loadCharacters(){
	// TIMMY
	// load neutral
	if (!characterTexture[0].loadFromFile("Assets/Sprites/Characters/Timmy/TimmyNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy
	if (!characterHappyTexture[0].loadFromFile("Assets/Sprites/Characters/Timmy/TimmyHappy.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad 
	if (!characterSadTexture[0].loadFromFile("Assets/Sprites/Characters/Timmy/TimmySad.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load neutral3D
	if (!character3DTexture[0].loadFromFile("Assets/Sprites/Characters/Timmy/Timmy3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy3D
	if (!characterHappy3DTexture[0].loadFromFile("Assets/Sprites/Characters/Timmy/Timmy3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad3D
	if (!characterSad3DTexture[0].loadFromFile("Assets/Sprites/Characters/Timmy/Timmy3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}

	// SHAWNSON
	// load neutral
	if (!characterTexture[1].loadFromFile("Assets/Sprites/Characters/Shawnson/ShawnsonNeutral2D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy
	if (!characterHappyTexture[1].loadFromFile("Assets/Sprites/Characters/Shawnson/ShawnsonHappy2D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad 
	if (!characterSadTexture[1].loadFromFile("Assets/Sprites/Characters/Shawnson/ShawnsonMad2D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load Neutral3D
	if (!character3DTexture[1].loadFromFile("Assets/Sprites/Characters/Shawnson/ShawnsonNeutral3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy3D
	if (!characterHappy3DTexture[1].loadFromFile("Assets/Sprites/Characters/Shawnson/ShawnsonHappy3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad3D 
	if (!characterSad3DTexture[1].loadFromFile("Assets/Sprites/Characters/Shawnson/ShawnsonMad3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
}