#include <SFML/Graphics.hpp>
#include <string>

#include "MenuState.h"
#include "GameState.h"
#include "GameVariables.h"
#include "DoorScene.h"
#include "OrbScene.h"
#include "SpellScene.h"

#include "State.h"

bool placeMode = false;
bool canPress = true;
bool characterArrived = false;
bool animTimerDone = false;
bool gameDone = false;

float pressTime = 0.0f;
const float placeModeSpeed = 25.0f;
const int gameWidth = 1920;
const int gameHeight = 1080;

const int maxMana = 100; 
int currentMana;

float direction1 = 0.0f;
float direction2 = 0.0f;
float globalTime;
float animTimer;
float randomTime;

int character;
int randomNumber;

sf::Font font;
sf::Font flameTimerFont;
sf::Text text;
sf::Text flameTimerText;

enum class States { MENU, PLAY, DOOR, ORB, FLAME, BOOK };

sf::Event event;

//Parameters
const float time_step = 0.017f; //60 fps

int main() {
	// Load values in GameVariables.h
	bool placeMode = false;
	bool canPress = true;
	bool characterArrived = false;
	bool animTimerDone = false;
	bool gameDone = false;

	float pressTime = 0.0f;
	const float placeModeSpeed = 25.0f;
	const int gameWidth = 1920;
	const int gameHeight = 1080;

	const int maxMana = 100;
	int currentMana;

	float direction1 = 0.0f;
	float direction2 = 0.0f;
	float globalTime;
	float randomTime;
	float animTimer;

	int character;
	int randomNumber;

	sf::Font font;
	sf::Text text;
	sf::Text flameTimerText;
	
	//create the window
	sf::RenderWindow window(sf::VideoMode({ gameWidth, gameHeight }), "FixAllShop");
	sf::Clock clock;

	// Initialize the current state
	States currentState = States::MENU;

	// Create instances of your states
	MenuState menuState;
	GameState gameState;
	DoorScene doorScene;
	OrbScene orbScene;
	SpellScene spellScene;

	while (window.isOpen()) {
		//Calculate dt
		float dt = clock.restart().asSeconds();

		// HANDLE STATE / SCENE CHANGES
		switch (currentState) {
		case States::MENU:
			menuState.handleInput();
			menuState.update(dt);
			menuState.render(window);

			// Example transition to PLAY state
			if (menuState.startGame) {
				currentState = States::PLAY;
				menuState.startGame = false;
			}
			break;

		case States::PLAY:
			gameState.handleInput();
			gameState.update(dt);
			gameState.render(window);

			if (gameState.firstLoad) {
				gameState.stateChange = 1;
			}

			if (gameState.stateChange == 1) {
				// Door
				gameState.stateChange = 0;
				currentState = States::DOOR;
			}
			else if (gameState.stateChange == 2) {
				// Orb
				gameState.stateChange = 0;
				currentState = States::ORB;
			}
			else if (gameState.stateChange == 3) {
				// Spellbook
				gameState.stateChange = 0;
				currentState = States::BOOK;
			}
			break;

		case States::DOOR:
			doorScene.handleInput();
			doorScene.update(dt);
			doorScene.render(window);

			if (gameState.firstLoad) {
				gameState.firstLoad = false;
				currentState = States::PLAY;
			}

			if (doorScene.backDoor) {
				// Door
				doorScene.backDoor = false;
				currentState = States::PLAY;
			}

			break;

		case States::ORB:
			orbScene.handleInput();
			orbScene.update(dt);
			orbScene.render(window);

			if (orbScene.backOrb) {
				// Door
				orbScene.backOrb = false;
				currentState = States::PLAY;
			}

			break;

		case States::BOOK:
			spellScene.handleInput();
			spellScene.update(dt);
			spellScene.render(window);

			if (spellScene.backSpell) {
				// Door
				spellScene.backSpell = false;
				currentState = States::PLAY;
			}

			break;
		}
		gameState.stateChange;
		// Quit Game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || gameState.stateChange == 5) {
			window.close();
		}

		//Wait for Vsync
		window.display();
		window.clear();
	}
}