#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Magnum.h>
#include <Magnum/Timeline.h>

#include "Logic/Map.hpp"
#include "Logic/Game.hpp"
#include "Graphics/Graphics.hpp"

#define WIDTH 10
#define HEIGHT 24

using namespace Magnum;
using namespace Math::Literals;


class PrimitivesExample: public Platform::Application
{
    public:

    private:
        Map<WIDTH, HEIGHT> stack;
        Game game;
        Graphics graphics;
        float counter;

    public:
        explicit PrimitivesExample(const Arguments& arguments);

    private:
        void drawEvent() override;
        void keyPressEvent(KeyEvent& event) override;

        Timeline timeline;
};

PrimitivesExample::PrimitivesExample(const Arguments& arguments):
    Platform::Application{arguments, Configuration{}
        .setTitle("Magnum Primitives Example")}
    , stack(Map<WIDTH, HEIGHT>())
    , game(Game(&stack))
    , graphics(Graphics(&stack, &game, windowSize()))
{
    counter = 0;
    timeline.start();
}

void PrimitivesExample::drawEvent()
{
    counter += timeline.previousFrameDuration();

    if (counter >= 0.2f)
    {
        counter = 0;
        game.update();
    }

    graphics.draw();

    swapBuffers();
    redraw();
    timeline.nextFrame();
}

void PrimitivesExample::keyPressEvent(KeyEvent& event)
{
    if(event.keyName() != "Left arrow") return;
    
    game.activeShape.moveLeft();
    redraw();
}

MAGNUM_APPLICATION_MAIN(PrimitivesExample)