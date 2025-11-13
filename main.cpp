#include <SFML/Graphics.hpp>
#include <string>

sf::Vector2f ballVelocity;
sf::Event event;

bool isPlayer1Serving = true;
const float initialVelocityX = 100.f; //horizontal velocity
const float initialVelocityY = 60.f; //vertical velocity

const float velocityMultiplier = 1.1f; //how much the ball will speed up everytime it hits a paddle. Here, 10% every time.

const sf::Keyboard::Key controls[5] = {
	sf::Keyboard::W,  // UP
	sf::Keyboard::S,  // Down
	sf::Keyboard::A,  // Left
	sf::Keyboard::D,   // Right
	sf::Keyboard::Space   // PlaceMode
};

//Parameters
const sf::Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 1920;
const int gameHeight = 1080;
const float placeModeSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float time_step = 0.017f; //60 fps

bool placeMode = false;
bool canPress;
float pressTime;

//Objects of the game
sf::CircleShape ball;
sf::RectangleShape paddles[2];
sf::Font font;
sf::Text text;

void init() {
	// Set size and origin of paddles
	for (sf::RectangleShape& p : paddles) {
		p.setSize(paddleSize);
		p.setOrigin(paddleSize / 2.f);
	}
	// Set size and origin of ball
	ball.setRadius(ballRadius);
	ball.setOrigin(ballRadius / 2, ballRadius / 2); //Should be half the ball width and height
	// reset paddle position
	paddles[0].setPosition(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f);
	paddles[1].setPosition(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f);
	// reset Ball Position
	ball.setPosition(gameWidth / 2, gameHeight / 2);

}

void reset();

void update(float dt) {
	// handle Placement
	float direction1 = 0.0f;
	float direction2 = 0.0f;

	// Press Delay
	if (pressTime > 0) {
		pressTime -= dt;
	}
	else {
		canPress = true;
	}

	// Inputs
	if (sf::Keyboard::isKeyPressed(controls[4]) && canPress){ // toggle placemode
		placeMode = !placeMode;
		canPress = false;
		pressTime = 1;
	}
	if (sf::Keyboard::isKeyPressed(controls[0]) && placeMode) {
		direction1--;
	}
	if (sf::Keyboard::isKeyPressed(controls[1]) && placeMode) {
		direction1++;
	}
	if (sf::Keyboard::isKeyPressed(controls[2]) && placeMode) {
		direction2--;
	}
	if (sf::Keyboard::isKeyPressed(controls[3]) && placeMode) {
		direction2++;
	}
	paddles[0].move(sf::Vector2f(direction2 * placeModeSpeed * dt, direction1 * placeModeSpeed * dt));

	ball.move(ballVelocity * dt);

	// check ball collision
	const float bx = ball.getPosition().x;
	const float by = ball.getPosition().y;
	if (by > gameHeight) { //bottom wall
		// bottom wall
		ballVelocity.x *= velocityMultiplier;
		ballVelocity.y *= -velocityMultiplier;
		ball.move(sf::Vector2f(0.f, -10.f));
	}
	else if (by < 0) { //top wall
		// top wall
		ballVelocity.x *= velocityMultiplier;
		ballVelocity.y *= -velocityMultiplier;
		ball.move(sf::Vector2f(0.f, 10.f));
	}
	else if (bx > gameWidth) {
		// right wall
		reset();
	}
	else if (bx < 0) {
		// left wall
		reset();
	}
}

void render(sf::RenderWindow& window) {
	// update position text
	sf::Vector2f textPosition = paddles[0].getPosition();
	text.setString("(" + std::to_string(static_cast<int>(textPosition.x)) + "," +
		std::to_string(static_cast<int>(textPosition.y)) + ") Placing: " + std::to_string(placeMode));
	
	// Draw Everything
	window.draw(paddles[0]);
	window.draw(ball);
	window.draw(text);
}

void load() {
	ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
}

void clean() {

}

void reset() {
	ball.setPosition(ballRadius / 2, ballRadius / 2);
}

int main() {
	//create the window
	sf::RenderWindow window(sf::VideoMode({ gameWidth, gameHeight }), "PONG");

	//initialise and load
	init();
	load();

	if (!font.loadFromFile("Assets/Fonts/arial.ttf")) {
		return -1; // Error loading font
	}
	text.setFont(font);
	text.setString("It's all meaningless!");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(ballRadius / 2, ballRadius / 2);

	sf::Clock clock;
	while (window.isOpen()) {
		
		// Quit Game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}
		
		//Calculate dt
		double dt = clock.restart().asSeconds();

		window.clear();
		update(dt);
		render(window);

		//Wait for Vsync
		window.display();
	}

	//Unload and shutdown
	clean();
}