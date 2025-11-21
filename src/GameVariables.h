// GameVariables.h
#ifndef GAMEVARIABLES_H
#define GAMEVARIABLES_H

// Bools
extern bool placeMode;
extern bool canPress;
extern bool characterArrived;
extern bool animTimerDone;
extern bool gameDone;

// Numbers
extern const int gameWidth;
extern const int gameHeight;

extern const float placeModeSpeed;

extern float direction1;
extern float direction2;

extern float pressTime;
extern float globalTime;
extern float animTimer;
extern float randomTime;
extern int character;
extern int randomNumber;

// Objects
extern sf::Font font;
extern sf::Font flameTimerFont;
extern sf::Text text;
extern sf::Text flameTimerText;

#endif // GAMEVARIABLES_H
