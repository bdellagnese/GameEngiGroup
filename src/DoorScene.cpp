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
bool enoughMana;

// Parameters
int cast[5];
int correctCast[5];
int customerHappy = 0;

const int totalChar = 6;

// character textures and values - if one changes, they all do
int renderNum;
sf::Texture characterTexture[totalChar];
sf::Texture characterHappyTexture[totalChar];
sf::Texture characterSadTexture[totalChar];

sf::Texture character3DTexture[totalChar];
sf::Texture characterHappy3DTexture[totalChar];
sf::Texture characterSad3DTexture[totalChar];

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

sf::Sprite continueSpr;
sf::Texture continueTexture;

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
sf::Keyboard::Key controls[6] = {
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
			if (currentMana > 30) {
				currentMana = currentMana - 30;
				enoughMana = true;
			}
			else {
				enoughMana = false;
			}
		}
		else if (direction == 2) {
			SpellSpr[castPosition].setTexture(spellDownTexture);
			if (currentMana > 20) {
				currentMana = currentMana - 20;
				enoughMana = true;
			}
			else {
				enoughMana = false;
			}
		}
		else if (direction == 3) {
			SpellSpr[castPosition].setTexture(spellLeftTexture);
			if (currentMana > 15) {
				currentMana = currentMana - 15;
				enoughMana = true;
			}
			else {
				enoughMana = false;
			}
		}
		else if (direction == 4) {
			SpellSpr[castPosition].setTexture(spellRightTexture);
			if (currentMana > 10) {
				currentMana = currentMana - 10;
				enoughMana = true;
			}
			else {
				enoughMana = false;
			}
		}
		
		int move = castPosition * 100;
		
		SpellSpr[castPosition].setPosition(1222 + move, 805);
		
		if (enoughMana) {
			castPosition++;
		}
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

	manaText.setString(std::to_string(static_cast<int>(currentMana)));

	if (totalCharacters < character) {
		globalTime = 0;
	}

	// Global Timer
	if (globalTime > 0) {
		globalTime -= dt;
	}
	else {
		if (customerHappy > totalCharacters / 2) {
			characterArrived = true;
			characterText.setString("You win!! \n\nThanks to you\nFradros has enough power to open his bakery");

			canPress = false;
			pressTime = 100;
		}
		else {
			// lose
			characterArrived = true;
			characterText.setString("You Lose!!");

			canPress = false;
			pressTime = 100;
		}
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
	characterSpr.move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));
	// DEBUG TEXT - "(x,y) Placing: t/f"
	sf::Vector2f textPosition = characterSpr.getPosition();

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
		window.draw(textboxSpr);
		
		if (globalTime > 0) {
			characterText.setString(characterString[currentString]);
		}
		
		window.draw(characterText);
		window.draw(character3DSpr);
	}

	// Spells and Runes
	window.draw(spellBannerSpr);
	window.draw(SpellSpr[0]);
	window.draw(SpellSpr[1]);
	window.draw(SpellSpr[2]);
	window.draw(SpellSpr[3]);
	window.draw(SpellSpr[4]);

	// Text
	if (canPress && currentString == 0 && characterArrived) {
		window.draw(continueSpr);
	}

	window.draw(manaBgSpr);
	window.draw(manaText);

	window.draw(text);
	window.draw(flameBgSpr);
	window.draw(flameTimerText);
	//Top Layer - UI
}

void characterHandling() {
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
			animTimer = 8;

			success = true;
		}
		else {
			// fail
			animTimerDone = false;
			animTimer = 8;
			success = false;
		}
		pressTime = 5;
		castPosition = 0;

		if (success) {
			characterSpr.setTexture(characterHappyTexture[renderNum]);
			character3DSpr.setTexture(characterHappy3DTexture[renderNum]);
			currentString = 2; // Happy Message

			customerHappy++;
			globalTime = globalTime + 30;
		}
		else
		{
			characterSpr.setTexture(characterSadTexture[renderNum]);
			character3DSpr.setTexture(characterSad3DTexture[renderNum]);
			currentString = 3; // Hateful

			globalTime = globalTime - 5;
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
	// Load largest sprite so all other textures fit within
	character3DSpr.setTexture(character3DTexture[3]);
	characterSpr.setTexture(characterTexture[3]);

	// New Character
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
		characterString[0] = "Hello Sir, it's nice to see you again! \nNo, I haven't gotten into any mischief\ntoday.\n\nWell...only sort of!\n\nI wasn't always this big you know?";
		characterString[1] = "Mummy said no wandering into the \nenchanted forest! \n\nI couldn't help myself though, and I \nwas really hungry waiting for dinner. \n\nI was plotting about and I saw some \norange roots, and I thought they \nwould be fine to eat. \nNow I'm TOO BIG to go back home!";
		characterString[2] = "I'm perfect now! \n\nThank goodness, she was going to whip me \nup the wall if she saw I left on my own. \n\nI'll try not coming back here, pray \ndon't tell her though.";
		characterString[3] = "That didn't work! \n\nMummy will call me home any minute \nnow, and she will be furious!\nWhat kind of a WIZARD are you? \n\nGoodbye, I will go find someone else \nwho can read a SPELL BOOK";

		// Shrink 34212 
		correctCast[0] = 3; // 1up 2down 3left 4right
		correctCast[1] = 4;
		correctCast[2] = 2;
		correctCast[3] = 1;
		correctCast[4] = 2;
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
		characterString[0] = "A wizard! \n\nFinally!\n\nI pray you have time for me, I need \nyou in this trying time.";
		characterString[1] = "My farm...the animals...all the hard \nwork gone! A...a fire! \n\nA really big fire! All I could do is grab \nsomething! I brought the goose, Betty \nis her name! \n\nI don't know what to do. \nMy barn is gone,\nhow am I to survive with no home!";
		characterString[2] = "The golden goose! Wizard, you are a \ngenius! \n\nI will live labour-free for plenty \nyears to come! \n\nBetty my saviour, I knew she would \nbe my only chance at a new life!";
		characterString[3] = "You imbecile! Nothing happened. \n\nForget I came to this stupid shack, \nI should have never trusted some old \nbag to do a real man's job! \n\nYou will never be seeing me again.";

		// gildgild 12131 
		correctCast[0] = 1; // 1up 2down 3left 4right
		correctCast[1] = 2;
		correctCast[2] = 1;
		correctCast[3] = 3;
		correctCast[4] = 1;
	}
	else if (character == 3) // Medra
	{
		// Change character texture
		character3DSpr.setTexture(character3DTexture[renderNum]);
		characterSpr.setTexture(characterTexture[renderNum]);

		// Adjust position
		characterSpr.setPosition(360, 143);
		character3DSpr.setPosition(characterSpr.getPosition());

		//Set text for character
		characterString[0] = "Wizard Fradros! You're in, at last! \n\nHelp me so! I'm miserable and I need to \nreverse this spell! The spell you gave \nme was a curse in disguise, \nI cannot stand to be this size anymore. \nI really need your help, hear my plea."
			"\n\nI hate the human boy, the one I \ngave up my family for!";
		characterString[1] = "The boy is a brat and he is always \ngrumpy. \n\nHe called me an insect, and said I had \ncoarse skin! Skin, like the stem of a \nflower, intentional and perfect! \n\nI want to go back home, I don't belong \nwith the humans.";
		characterString[2] = "I am perfect this way! \n\nI won't dare to talk to them humans \nagain. Maybe, next season...\n\nThank you Wizard, I can always \ncount on you being a good listener.";
		characterString[3] = "You are the best wizard I know! \n\nI can't believe it. I'm stuck at that \ncotttage for a while now. \n\nI'll have to start behaving like a \nhuman girl. Maybe, I can find someone \nelse to shelter me for now. \nBye Fradros, I hope to see you in \nbetter circumstances next time.";

		// shrink
		correctCast[0] = 3; // 1up 2down 3left 4right
		correctCast[1] = 4;
		correctCast[2] = 2;
		correctCast[3] = 1;
		correctCast[4] = 2;
	}
	else if (character == 4) // Sir Wompulus
	{
		// Change character texture
		character3DSpr.setTexture(character3DTexture[renderNum]);
		characterSpr.setTexture(characterTexture[renderNum]);

		// Adjust position
		characterSpr.setPosition(228, 10);
		character3DSpr.setPosition(characterSpr.getPosition());

		//Set text for character
		characterString[0] = "Fradros, the good Wizard of the forest! \n\nYes, this is an intentional visit, may I \nshare my woes and sorrows with a \ngood man like you? Yes? \n\nMy father, you know he's getting \nolder. His kingdom hasn't the money to \nkeep up with...family needs.";
		characterString[1] = "I have great ideas no one never listens \nto. I have saved a great deal of money \nwith the help of absolutely no one at \nall. All my own efforts! \n\nBut, it is not enough! I need more to \nfulfill my dreams! \n\nYou believe in my dreams, don't you?";
		characterString[2] = "Fradros! Not what I expected, but I \nshine like never before. \n\nNo one will ever ignore me now, \nand I will always have the most gold in \nthe kingdom! \n\nIf you ever come by the kingdom, you \nwill live in luxury I tell you!";
		characterString[3] = "WHAT!? YOU DON'T THINK \nI DESERVE THIS!? WHO DO YOU \nTHINK YOU ARE LIVING ON \nTHIS PROPERTY! WHOEVER \nOWNS THIS LAND WILL BE \nORDERED TO ERASE YOU FROM \nTHESE PREMISES. \nI am eternally miserable, you should \nbe ashamed. The Prince is never sad, \nand you have saddened me the most!";

		// gild
		correctCast[0] = 1; // 1up 2down 3left 4right
		correctCast[1] = 2;
		correctCast[2] = 1;
		correctCast[3] = 3;
		correctCast[4] = 1;
	}
	else if (character == 5) // Jane
	{
		// Change character texture
		character3DSpr.setTexture(character3DTexture[renderNum]);
		characterSpr.setTexture(characterTexture[renderNum]);

		// Adjust position
		characterSpr.setPosition(241, 193);
		character3DSpr.setPosition(characterSpr.getPosition());

		// Jane
		characterString[0] = "Hi.";
		characterString[1] = "I bought this box off a con. \n\nIt's supposed to have a ghoul or \nsomething in it, turns out there's \nnothing. \n\nI just need something in this thing \nso I can return it with a smile.";
		characterString[2] = "Scary. Thanks.";
		characterString[3] = "What a hack. You're dead old man.";

		// curse bark 23444 curse 12432
		correctCast[0] = 1; // 1up 2down 3left 4right
		correctCast[1] = 2;
		correctCast[2] = 4;
		correctCast[3] = 3;
		correctCast[4] = 2;
	}
	else if (character == 6) // Jester
	{
		// Change character texture
		character3DSpr.setTexture(character3DTexture[renderNum]);
		characterSpr.setTexture(characterTexture[renderNum]);

		// Adjust position
		characterSpr.setPosition(150, 193);
		character3DSpr.setPosition(characterSpr.getPosition());

		// Jester
		characterString[0] = "I'm glad I found you Wizard. Look, \njesters aren't always ready to \nentertain -I'm off duty and I need a \nsmoke. \n\nI'm here for a reason, you know!\n\nThe King - Grumble - I work for him. \nGreatest jester in the biz, don't sweat \nit baby.";
		characterString[1] = "I can dance all day and all night, but \nwhat I can't stand is crowd demands. \n\nGrumble wants me to dance this \ncomplicated routine, and I'm bound to \nmess up - not as young as I used to be. \n\nDo me a favour, I just can't stand the \nKing throwing stones at me!";
		characterString[2] = "This it? I feel hefty like cattle, just \none more reason for the wife to think im \na fat cow! \n\nThanks anyway for my thick skin. I \nwon't feel a thing from that Grumble.";
		characterString[3] = "You kidding me? I came all the way \nhere from the other side of the \nenchanted forest for nothing? \n\nYou are just as useless as those \nfairies. \n\nBye.";

		// bark skin
		correctCast[0] = 2; // 1up 2down 3left 4right
		correctCast[1] = 3;
		correctCast[2] = 4;
		correctCast[3] = 4;
		correctCast[4] = 4;
	}
}

void loadDoor() {
	hasLoadedDoor = true;
	animTimer = 1000;
	animTimerDone = false;
	backDoor = false;
	enoughMana = true;
	
	//character = 4;

	//currentMana = maxMana;
	currentMana = 1000;

	if (!characterFont.loadFromFile("Assets/Fonts/hennyPenny.ttf"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	characterText.setFont(characterFont);
	characterText.setCharacterSize(35);
	characterText.setFillColor(sf::Color::Black);
	//characterText.setOutlineThickness(1);
	characterText.setPosition(1234, 114);

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
	textboxSpr.setPosition(-3, -39);

	//load "continue"
	if (!continueTexture.loadFromFile("Assets/Sprites/continue.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	continueSpr.setTexture(continueTexture);
	continueSpr.setPosition(1758, 648);

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
	if (!characterHappy3DTexture[0].loadFromFile("Assets/Sprites/Characters/Timmy/TimmyHappy3D.tga"))
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

	// MEDRA
	// load neutral
	if (!characterTexture[2].loadFromFile("Assets/Sprites/Characters/SirWomp/WompNeutral3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy
	if (!characterHappyTexture[2].loadFromFile("Assets/Sprites/Characters/SirWomp/WompNeutral3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad 
	if (!characterSadTexture[2].loadFromFile("Assets/Sprites/Characters/SirWomp/WompNeutral3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load Neutral3D
	if (!character3DTexture[2].loadFromFile("Assets/Sprites/Characters/Medra/MedraNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy3D
	if (!characterHappy3DTexture[2].loadFromFile("Assets/Sprites/Characters/Medra/MedraHappy.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad3D 
	if (!characterSad3DTexture[2].loadFromFile("Assets/Sprites/Characters/Medra/MedraMad.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}

	// SIR WOMPULUS
	// load neutral
	if (!characterTexture[3].loadFromFile("Assets/Sprites/Characters/SirWomp/WompNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy
	if (!characterHappyTexture[3].loadFromFile("Assets/Sprites/Characters/SirWomp/WompHappy.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad 
	if (!characterSadTexture[3].loadFromFile("Assets/Sprites/Characters/SirWomp/WompMad.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load Neutral3D
	if (!character3DTexture[3].loadFromFile("Assets/Sprites/Characters/SirWomp/WompNeutral3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy3D
	if (!characterHappy3DTexture[3].loadFromFile("Assets/Sprites/Characters/SirWomp/WompHappy3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad3D 
	if (!characterSad3DTexture[3].loadFromFile("Assets/Sprites/Characters/SirWomp/WompMad3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}

	// Jane
	// load neutral
	if (!characterTexture[4].loadFromFile("Assets/Sprites/Characters/Jane/JaneNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy
	if (!characterHappyTexture[4].loadFromFile("Assets/Sprites/Characters/Jane/JaneHappy2D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad 
	if (!characterSadTexture[4].loadFromFile("Assets/Sprites/Characters/Jane/JaneMad.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load Neutral3D
	if (!character3DTexture[4].loadFromFile("Assets/Sprites/Characters/Jane/Empty3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy3D
	if (!characterHappy3DTexture[4].loadFromFile("Assets/Sprites/Characters/Jane/JaneHappy3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad3D 
	if (!characterSad3DTexture[4].loadFromFile("Assets/Sprites/Characters/Jane/Empty3D.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}

	/* Jester
	// load neutral
	if (!characterTexture[5].loadFromFile("Assets/Sprites/Characters/Jester/JestNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy
	if (!characterHappyTexture[5].loadFromFile("Assets/Sprites/Characters/Jester/JestNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad 
	if (!characterSadTexture[5].loadFromFile("Assets/Sprites/Characters/Jester/JestNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load Neutral3D
	if (!character3DTexture[5].loadFromFile("Assets/Sprites/Characters/Jester/JestNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load happy3D
	if (!characterHappy3DTexture[5].loadFromFile("Assets/Sprites/Characters/Jester/JestNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}
	// load sad3D 
	if (!characterSad3DTexture[5].loadFromFile("Assets/Sprites/Characters/Jester/JestNeutral.tga"))
	{
		printf("--ERROR LOADING ASSETS--"); // Error Loading File
	}*/
}