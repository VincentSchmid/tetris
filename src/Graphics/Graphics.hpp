#ifndef Graphics_h
#define Graphics_h

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

#include "Logic/Map.hpp"
#include "Logic/Game.hpp"
#include "helpers.hpp"

using namespace Magnum;
using namespace Math::Literals;


class Graphics
{
    public:
        Graphics(Map<10, 24> *currentMap, Game *game, Vector2i windowSize);
        void draw();

    private:
        void drawActiveShape();
        void drawMap();
        void drawBorder();
        void drawNextShape();
        void drawBox(Coord position, Color3 color);

        void drawShape(Shape *shape, Coord position, Color3 color);
        
    private:
        Game *game;
        Map<10, 24> *currentMap;
        Vector2i windowSize;
        Vector3 lightPosition;

        GL::Mesh mesh;
        Shaders::Phong shader;
        Matrix4 transformation, projection;
        Color3 activeShape;
        Color3 mapShapes;
        Color3 mapBorder;
};

Graphics::Graphics(Map<10, 24> *currentMap, Game *game, Vector2i windowSize) 
: currentMap(currentMap)
, game(game)
, windowSize(windowSize)
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

    mesh.setPrimitive(cube.primitive())
        .setCount(cube.indexCount())
        .addVertexBuffer(std::move(vertices), 0, Shaders::Phong::Position{},
                                                 Shaders::Phong::Normal{})
        .setIndexBuffer(std::move(indices), 0, compressed.second);

    activeShape = Color3::fromHsv({35.0_degf, 1.0f, 1.0f});
    mapShapes = Color3::fromHsv({15.0_degf, 1.0f, 1.0f});
    mapBorder = Color3::fromHsv({100.0_degf, 1.0f, 1.0f});

    lightPosition = {1.4f, 1.0f, 2.0f};

    transformation = Matrix4::scaling({.1f, .1f, .1f}) * Matrix4::rotationX(30.0_degf)*Matrix4::rotationY(40.0_degf);

    projection =
        Matrix4::perspectiveProjection(
            35.0_degf, Vector2{windowSize}.aspectRatio(), 0.01f, 150.0f)*
        Matrix4::translation({-5.0f, -22.0f, -130.0f});
}

void Graphics::draw()
{
    GL::defaultFramebuffer.clear(
        GL::FramebufferClear::Color|GL::FramebufferClear::Depth);

    drawNextShape();
    drawActiveShape();
    drawMap();
    drawBorder();
}

void Graphics::drawMap()
{
    for (int y = 0; y < currentMap->height; y++)
    {
        for (int x = 0; x < currentMap->width; x++)
        {
            Coord mapCoords {x, y};

            if (currentMap->map[coordsToIndex<10>(mapCoords)] == 1)
            {
                drawBox({mapCoords.x, currentMap->height - mapCoords.y - 1}, mapShapes);
            }
        }
    }
}

void Graphics::drawBorder()
{
    for (int y = -1; y < currentMap->height + 1; y++)
    {
        drawBox({-1, y}, mapBorder);
        drawBox({currentMap->width, y}, mapBorder);
    }

    for (int x = -1; x < currentMap->width + 1; x++)
    {
        drawBox({x, -1}, mapBorder);
        drawBox({x, currentMap->height}, mapBorder);
    }
}

void Graphics::drawActiveShape()
{
    drawShape(&game->activeShape, game->activeShape.position, activeShape);
}

void Graphics::drawNextShape()
{
    drawShape(&game->nextShape, {-5, 12}, activeShape);
}

void Graphics::drawShape(Shape *shape, Coord position, Color3 color)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            Coord currPos {position.x + x, position.y - y};

            if (shape->shape[coordsToIndex<4>({x, y})] == 1)
            {
                drawBox(currPos, color);
            }
        }
    }
}

void Graphics::drawBox(Coord position, Color3 color)
{
    shader.setLightPositions({lightPosition})
        .setDiffuseColor(color)
        .setAmbientColor(Color3::fromHsv({color.hue(), 1.0f, 0.3f}))
        .setTransformationMatrix(Matrix4::translation({(Float) position.x * 2.1f, (Float) position.y * 2.1f, 0.0f}))
        .setNormalMatrix(transformation.normalMatrix())
        .setProjectionMatrix(projection)
        .draw(mesh);
}

#endif