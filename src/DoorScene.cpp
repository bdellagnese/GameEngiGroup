#include <SFML/Graphics.hpp>
#include "DoorScene.h"
#include "GameVariables.h"
#include "GameState.h"

GameState gameState;
bool newCharacter;
bool hasLoadedDoor = false;
bool doneCasting;

// Parameters
int cast[5];
int correctCast[5];
int castPosition = 0;
int x = 0;

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

sf::Sprite spellBannerSpr;
sf::Texture spellBannerTexture;

sf::Texture spellUpTexture;
sf::Texture spellDownTexture;
sf::Texture spellLeftTexture;
sf::Texture spellRightTexture;
sf::Texture blankTexture;

sf::Sprite SpellSpr[5];
sf::Texture currentSpellTexture;


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

	// Spell Casting Input
	// WASD inputs
	if (sf::Keyboard::isKeyPressed(controls[0]) && canPress && !doneCasting) { //up
		currentSpellTexture = spellUpTexture;
		casting(1);
	}
	else if (sf::Keyboard::isKeyPressed(controls[1]) && canPress && !doneCasting) { //down
		currentSpellTexture = spellDownTexture;
		casting(2);
	}
	else if (sf::Keyboard::isKeyPressed(controls[2]) && canPress && !doneCasting) { //left
		currentSpellTexture = spellLeftTexture;
		casting(3);
	}
	else if (sf::Keyboard::isKeyPressed(controls[3]) && canPress && !doneCasting) { //right
		casting(4);
	}
}

void casting(int direction) {
	canPress = false;
	pressTime = 1;

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

	characterHandling();

	// PLACE MODE - can be used for any sprite
	SpellSpr[0].move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));
	// DEBUG TEXT - "(x,y) Placing: t/f"
	sf::Vector2f textPosition = SpellSpr[0].getPosition();

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
	window.draw(spellBannerSpr);

	window.draw(SpellSpr[0]);
	window.draw(SpellSpr[1]);
	window.draw(SpellSpr[2]);
	window.draw(SpellSpr[3]);
	window.draw(SpellSpr[4]);
	window.draw(text);
	window.draw(flameTimerText);
	//Top Layer - UI
}

void characterHandling() {
	int getCastLength = sizeof(cast) / sizeof(cast[0]);
	if (doneCasting)
	{
		doneCasting = false;
		castPosition = 0;
		// check if arrays are same
		for (int num : cast) {
			if (num == correctCast[x]) {
				// next number
				characterSpr.setTexture(characterHappyTexture);
			}
			else
			{
				// cancel and fail
				characterSpr.setTexture(characterSadTexture);
				return;
			}
			SpellSpr[num].setTexture(blankTexture);
			x++;
		}
		x = 0;
	}

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

	correctCast[0] = 1; // 0up 1down 2left 3right 0up = 12341
	correctCast[1] = 2;
	correctCast[2] = 3;
	correctCast[3] = 4;
	correctCast[4] = 1;
}
