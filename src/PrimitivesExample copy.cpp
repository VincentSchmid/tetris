
#include <Magnum/Timeline.h>

#include <Shapes/ShapeFactory.hpp>


using namespace Magnum;
using namespace Math::Literals;

class PrimitivesExample: public Platform::Application {
    public:
        explicit PrimitivesExample(const Arguments& arguments);

    private:
        void drawEvent() override;
        void mousePressEvent(MouseEvent& event) override;
        void mouseReleaseEvent(MouseEvent& event) override;
        void mouseMoveEvent(MouseMoveEvent& event) override;

        GL::Mesh _mesh;
        Shaders::Phong _shader;
        Timeline timeline;

        Matrix4 _transformation, _projection;
        Color3 _color;
};

PrimitivesExample::PrimitivesExample(const Arguments& arguments):
    Platform::Application{arguments, Configuration{}
        .setTitle("Magnum Primitives Example")}
{
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    Trade::MeshData cube = Primitives::cubeSolid();

    GL::Buffer vertices;
    vertices.setData(MeshTools::interleave(cube.positions3DAsArray(),
                                           cube.normalsAsArray()));

    std::pair<Containers::Array<char>, MeshIndexType> compressed =
        MeshTools::compressIndices(cube.indicesAsArray());
    GL::Buffer indices;
    indices.setData(compressed.first);

    _mesh.setPrimitive(cube.primitive())
        .setCount(cube.indexCount())
        .addVertexBuffer(std::move(vertices), 0, Shaders::Phong::Position{},
                                                 Shaders::Phong::Normal{})
        .setIndexBuffer(std::move(indices), 0, compressed.second);
    
    _transformation =
        Matrix4::rotationX(30.0_degf)*Matrix4::rotationY(40.0_degf);

    _projection =
        Matrix4::perspectiveProjection(
            35.0_degf, Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f)*
        Matrix4::translation(Vector3::zAxis(-10.0f));
    _color = Color3::fromHsv({35.0_degf, 1.0f, 1.0f});

    timeline.start();
}

void PrimitivesExample::drawEvent() {
    GL::defaultFramebuffer.clear(
        GL::FramebufferClear::Color|GL::FramebufferClear::Depth);

    Float offset = timeline.previousFrameTime();
    
    for (int i = 0; i < 10; i++)
    {
        _shader.setLightPositions({{1.4f, 1.0f, 0.75f}})
        .setDiffuseColor(_color)
        .setAmbientColor(Color3::fromHsv({_color.hue(), 1.0f, 0.3f}))
        .setTransformationMatrix(_transformation * Matrix4::translation({offset, (Float) i * 2, 0.0f}))
        .setNormalMatrix(_transformation.normalMatrix())
        .setProjectionMatrix(_projection)
        .draw(_mesh);
    }

    swapBuffers();
    redraw();
    timeline.nextFrame();
}

void PrimitivesExample::mousePressEvent(MouseEvent& event) {
    if(event.button() != MouseEvent::Button::Left) return;

    event.setAccepted();
}

void PrimitivesExample::mouseReleaseEvent(MouseEvent& event) {
    _color = Color3::fromHsv({_color.hue() + 50.0_degf, 1.0f, 1.0f});

    event.setAccepted();
    redraw();
}

void PrimitivesExample::mouseMoveEvent(MouseMoveEvent& event) {
    if(!(event.buttons() & MouseMoveEvent::Button::Left)) return;

    Vector2 delta = 3.0f*Vector2{event.relativePosition()}/Vector2{windowSize()};

    _transformation =
        Matrix4::rotationX(Rad{delta.y()})*
        _transformation*
        Matrix4::rotationY(Rad{delta.x()});

    event.setAccepted();
    redraw();
}

MAGNUM_APPLICATION_MAIN(PrimitivesExample)