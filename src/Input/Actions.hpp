#ifndef Actions_h
#define Actions_h

#include "Logic/Game.hpp"


class Actions
{
    public:
        Actions(Game *currentGame)
        : currentGame(currentGame)
        {};

        void moveRight();
        void moveLeft();
        void rotate();

    private:
        Game *currentGame;
        void action(void (*function)());
};

void Actions::moveLeft()
{
    currentGame->activeShape.moveLeft();
    //draw()
}

void Actions::moveRight()
{
    currentGame->activeShape.moveRight();
}

void Actions::rotate()
{
    currentGame->activeShape.rotate();
}

void Actions::action(void (*actionToExecute)())
{
    actionToExecute();
    // draw();
}

#endif