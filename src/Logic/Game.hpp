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
        Game(Map<WIDTH, HEIGHT> *map)
        : stack(map)
        , shapeFactory(ShapeFactory())
        , activeShape(shapeFactory.getShape(false))
        {
            currentState = GameState::PLACED;
            score = 0;
        };

        GameState currentState;
        ShapeFactory shapeFactory;
        Shape activeShape;
        Map<WIDTH, HEIGHT> *stack;
        int score;

    private:

    public:
        void update();

    private:

};

void Game::update()
{
    if (currentState == GameState::PLACED)
    {
        activeShape = shapeFactory.getShape(true);
        std::pair<int, int> shapeBounds = activeShape.getBoundingWidth();
        activeShape.position = {getSpawnXPos(shapeBounds.first, shapeBounds.second, WIDTH), HEIGHT - 1};
        currentState = GameState::IS_PLACING;
    }

    activeShape.moveDown();

    if (stack->checkCollision(activeShape))
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

#endif