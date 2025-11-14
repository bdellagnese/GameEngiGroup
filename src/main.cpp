#include <SFML/Graphics.hpp>
#include <string>

#include "MenuState.h"
#include "GameState.h"
#include "State.h"

enum class States { MENU, PLAY };

sf::Event event;

//Parameters
const int gameWidth = 1920;
const int gameHeight = 1080;
const float placeModeSpeed = 400.f;
const float time_step = 0.017f; //60 fps

void init() {

}

void update(float dt) {
	
}

void render(sf::RenderWindow& window) {
	
}

int main() {
	//create the window
	sf::RenderWindow window(sf::VideoMode({ gameWidth, gameHeight }), "FixAllShop");

	//initialise and load
	init();

	// Initialize the current state
	States currentState = States::MENU;

	// Create instances of your states
	MenuState menuState;
	GameState gameState;
	
	//Calculate dt
	sf::Clock clock;
	float dt = clock.restart().asSeconds();

	while (window.isOpen()) {
		
		// HANDLE STATE / SCENE CHANGES
		switch (currentState) {
		case States::MENU:
			menuState.handleInput();
			menuState.update();
			menuState.render(window);
			
			// Example transition to PLAY state
			if (menuState.startGame) {
				currentState = States::PLAY;
				menuState.startGame = false;
			}
			break;

		case States::PLAY:
			gameState.handleInput();
			gameState.update();
			gameState.render(window);
			break;
		}
		
		// Quit Game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}

		//Wait for Vsync
		window.display();
		window.clear();
	}
}