// GameVariables.h
#ifndef GAMEVARIABLES_H
#define GAMEVARIABLES_H

// Bools
extern bool placeMode;
extern bool canPress;
extern bool characterArrived;

// Numbers
extern float pressTime;
extern const int gameWidth;
extern const int gameHeight;
extern const float placeModeSpeed;
extern float direction1;
extern float direction2;
extern float globalTime;

// Objects
extern sf::Font font;
extern sf::Font flameTimerFont;
extern sf::Text text;
extern sf::Text flameTimerText;

#endif // GAMEVARIABLES_H
