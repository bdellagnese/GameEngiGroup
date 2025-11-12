#include <SFML/Graphics.hpp>

sf::Vector2f ballVelocity;
bool isPlayer1Serving = true;
const float initialVelocityX = 100.f; //horizontal velocity
const float initialVelocityY = 60.f; //vertical velocity

const float velocityMultiplier = 1.1f; //how much the ball will speed up everytime it hits a paddle. Here, 10% every time.

const sf::Keyboard::Key controls[4] = {
	sf::Keyboard::W,   // Player1 UP
	sf::Keyboard::S,   // Player1 Down
	sf::Keyboard::Up,  // Player2 UP
	sf::Keyboard::Down // Player2 Down
};

//Parameters
const sf::Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float time_step = 0.017f; //60 fps


//Objects of the game
sf::CircleShape ball;
sf::RectangleShape paddles[2];

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
	// handle paddle movement
	float direction1 = 0.0f;
	float direction2 = 0.0f;
	if (sf::Keyboard::isKeyPressed(controls[0])) {
		direction1--;
	}
	if (sf::Keyboard::isKeyPressed(controls[1])) {
		direction1++;
	}
	paddles[0].move(sf::Vector2f(0.f, direction1 * paddleSpeed * dt));

	if (sf::Keyboard::isKeyPressed(controls[2])) {
		direction2--;
	}
	if (sf::Keyboard::isKeyPressed(controls[3])) {
		direction2++;
	}
	paddles[1].move(sf::Vector2f(0.f, direction2 * paddleSpeed * dt));

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
	// Draw Everything
	window.draw(paddles[0]);
	window.draw(paddles[1]);
	window.draw(ball);
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

	sf::Clock clock;
	while (window.isOpen()) {
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