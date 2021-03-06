#include <Magnum/Magnum.h>
#include <Magnum/Timeline.h>

#include <memory>

#include "params.hpp"
#include "Logic/Map.hpp"
#include "Logic/Game.hpp"
#include "Graphics/Graphics.hpp"

using namespace Magnum;
using namespace Math::Literals;


class PrimitivesExample: public Platform::Application
{
    private:
        std::unique_ptr<Map<WIDTH, HEIGHT>> stack;
        std::unique_ptr<Game> game;
        Graphics graphics;
        float counter;
        float tickRate;
        Timeline timeline;

    public:
        explicit PrimitivesExample(const Arguments& arguments);

    private:
        void startNewGame();
        void drawEvent() override;
        void keyPressEvent(KeyEvent& event) override;
};

PrimitivesExample::PrimitivesExample(const Arguments& arguments):
    Platform::Application{arguments, Configuration{}
        .setTitle("Magnum Primitives Example")}
    , stack(new Map<WIDTH, HEIGHT>())
    , game(new Game(stack))
    , graphics(Graphics(stack, game, windowSize()))
{
    counter = 0;
    tickRate = 0.5f;
    timeline.start();
}

void PrimitivesExample::startNewGame()
{
    stack.reset(new Map<WIDTH, HEIGHT>());
    game.reset(new Game(stack));
}

void PrimitivesExample::drawEvent()
{
    counter += timeline.previousFrameDuration();

    if (counter >= tickRate)
    {
        counter = 0;
        game->update();
    }

    if (game->currentState == GameState::GAME_OVER)
    {
        startNewGame();
    }

    graphics.draw();

    swapBuffers();
    redraw();
    timeline.nextFrame();
}

void PrimitivesExample::keyPressEvent(KeyEvent& event)
{
    if(event.key() == KeyEvent::Key::A)
    {
        game->activeShape.moveLeft();
    }

    else if(event.key() == KeyEvent::Key::D)
    {
        game->activeShape.moveRight();
    }

    else if(event.key() == KeyEvent::Key::W)
    {
        game->activeShape.rotate();
    }

    else if(event.key() == KeyEvent::Key::S)
    {
        game->hardDrop();
    }
    
    if(stack->checkCollision(&(game->activeShape)))
    {
        game->activeShape.undoLastMovement();
    }

    redraw();
}

MAGNUM_APPLICATION_MAIN(PrimitivesExample)