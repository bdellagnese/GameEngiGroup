#include <SFML/Graphics.hpp>
#include "DoorScene.h"
#include "GameVariables.h"
#include "GameState.h"

GameState gameState;
bool newCharacter;

bool hasLoadedDoor = false;
void loadDoor();
void loadCharacters();
void characterHandling();

// Objects
sf::Sprite doorPlaceholder;

sf::Sprite doorframeSpr;
sf::Texture doorframeTexture;

sf::Sprite doorframeBgSpr;
sf::Texture doorframeBgTexture;

sf::Sprite characterSpr;
sf::Texture characterTexture;
sf::Texture characterHappyTexture;
sf::Texture characterSadTexture;

sf::Sprite character3DSpr;
sf::Texture character3DTexture;

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
		backDoor = true;
	}
}

void DoorScene::update(float& dt) {
	// Update game logic
	if (!hasLoadedDoor) {
		loadDoor();
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

	// PLACE MODE - can be used for any sprite
	characterSpr.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));
	// DEBUG TEXT - "(x,y) Placing: t/f"
	sf::Vector2f textPosition = characterSpr.getPosition();

	text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
		std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode));
}

void DoorScene::render(sf::RenderWindow& window) {
	// Render game

	//Bottom Layer - The background
	window.draw(doorframeBgSpr);

	if (characterArrived) {
		window.draw(characterSpr);
		//window.draw(textboxSpr);
		//window.draw(characterText);
	}

	window.draw(doorframeSpr);
	
	if (characterArrived) {
		window.draw(character3DSpr);
	}
	window.draw(text);
	window.draw(flameTimerText);
	//Top Layer - UI
}

void loadDoor() {
	hasLoadedDoor = true;

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

	loadCharacters();
}

void loadCharacters(){
	// load timmy
	if (!characterTexture.loadFromFile("Assets/Sprites/Characters/Timmy/TimmyNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load timmy happy
	if (!characterHappyTexture.loadFromFile("Assets/Sprites/Characters/Timmy/TimmyHappy.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load timmy sad 
	if (!characterSadTexture.loadFromFile("Assets/Sprites/Characters/Timmy/TimmySad.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load timmy 3D
	if (!character3DTexture.loadFromFile("Assets/Sprites/Characters/Timmy/Timmy3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	character3DSpr.setTexture(character3DTexture);
	characterSpr.setTexture(characterTexture);

	characterSpr.setPosition(411, 160);
	character3DSpr.setPosition(characterSpr.getPosition());
}

void characterHandling() {
	/*
	float conversation = 0;



	if (nextText){
		nextText = false;
		interaction++
	}

	if(conversation == 1){ // Greeting
		text = characterText;

		if click{
			nextText = true;
		}
	}
	else if (conversation == 2){ // Additional Text - spell casting
		text = characterText;

		// array
		if (currentSpell[] == CorrectSpell[]){
			conversation = 3; //success
		}
		else if (at array character limit){
			conversation = 4; //fail
		}

		// WASD inputs
		if (sf::Keyboard::isKeyPressed(controls[0])) {

		}
		else if (sf::Keyboard::isKeyPressed(controls[1])) {

		}
		else if (sf::Keyboard::isKeyPressed(controls[2])) {

		}
		else if (sf::Keyboard::isKeyPressed(controls[3])) {

		}
	}
	else if (conversation == 3){
		// Success
		charSpriteTexture = activeCharTexture; // change texture to happy / fail sprite. could be done elsewhere
		charSprite.setTexture(charSpriteTexture);
		text = "yap fest";
	}
	else if (conversation == 4){
		// Fail
		charSprite.setTexture(charSpriteTexture);
		text = "yap fest";
	}
	*/
}
