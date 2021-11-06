#ifndef GameState_h
#define GameState_h

#include "Map.hpp"
#include "helpers.hpp"
#include "Shapes/Shape.hpp"
#include "Shapes/ShapeFactory.hpp"

#define WIDTH 10
#define HEIGHT 24


enum GameState
{
    IS_PLACING,
    PLACED,
    GAME_OVER
};

class Game
{
    public:
        GameState currentState;
        ShapeFactory shapeFactory;
        Shape nextShape;
        Shape activeShape;
        Map<WIDTH, HEIGHT> *stack;
        int score;

    private:

    public:
        Game(Map<WIDTH, HEIGHT> *map)
        : stack(map)
        , shapeFactory(ShapeFactory())
        , nextShape(shapeFactory.getShape(false))
        , activeShape(nextShape)
        {
            currentState = GameState::PLACED;
            score = 0;
        };
    
        void update();
        void hardDrop();
        void gameOver();

    private:

};

void Game::update()
{
    if (currentState == GameState::PLACED)
    {
        activeShape = nextShape;
        nextShape = shapeFactory.getShape(true);
        std::pair<int, int> shapeBounds = activeShape.getBoundingWidth();
        activeShape.position = {getSpawnXPos(shapeBounds.first, shapeBounds.second, WIDTH), HEIGHT - 1};
        currentState = GameState::IS_PLACING;

        // collision on place means game over!
        if (stack->checkCollision(&activeShape))
        {
            currentState = GameState::GAME_OVER;
            return;
        }
    }

    activeShape.moveDown();

    if (stack->checkCollision(&activeShape))
    {
        activeShape.undoLastMovement();
        stack->addBlock(activeShape);
        currentState = GameState::PLACED;
    }

    int clearedLine = stack->checkLines();

    while (clearedLine != -1)
    {
        score += WIDTH;
        stack->shiftDown(clearedLine);
        clearedLine = stack->checkLines();
    }
}

void Game::hardDrop()
{
    while(!stack->checkCollision(&activeShape))
    {
        activeShape.moveDown();
    }

    activeShape.undoLastMovement();
}

void Game::gameOver()
{
    stack->clearMap();
}

#endif
