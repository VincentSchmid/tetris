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
        Game *game;
        Map<10, 24> *currentMap;
        Vector2i windowSize;

        GL::Mesh mesh;
        Shaders::Phong shader;
        Matrix4 transformation, projection;
        Color3 activeShape;
        Color3 mapShapes;
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

    transformation = Matrix4::scaling({.1f, .1f, .1f}) * Matrix4::rotationX(30.0_degf)*Matrix4::rotationY(40.0_degf);

    projection =
        Matrix4::perspectiveProjection(
            35.0_degf, Vector2{windowSize}.aspectRatio(), 0.01f, 100.0f)*
        Matrix4::translation({-5.0f, -12.0f, -70.0f});

}

void Graphics::draw()
{
    GL::defaultFramebuffer.clear(
        GL::FramebufferClear::Color|GL::FramebufferClear::Depth);

    
    Coord activeShapePos = {game->activeShape.position.x, game->activeShape.position.y};

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            Coord currPos {activeShapePos.x + x, activeShapePos.y - y};

            if (game->activeShape.shape[coordsToIndex<4>({x, y})] == 1)
            {
                shader.setLightPositions({{1.4f, 1.0f, 0.75f}})
                .setDiffuseColor(activeShape)
                .setAmbientColor(Color3::fromHsv({activeShape.hue(), 1.0f, 0.3f}))
                .setTransformationMatrix(Matrix4::translation({(Float) currPos.x * 2, (Float) currPos.y * 2, 0.0f}))
                .setNormalMatrix(transformation.normalMatrix())
                .setProjectionMatrix(projection)
                .draw(mesh);
            }
        }
    }

    for (int y = 0; y < currentMap->height; y++)
    {
        for (int x = 0; x < currentMap->width; x++)
        {
            Coord mapCoords {x, currentMap->height - y - 1};

            if (currentMap->map[coordsToIndex<10>(mapCoords)] == 1)
            {
                shader.setLightPositions({{1.4f, 1.0f, 0.75f}})
                .setDiffuseColor(mapShapes)
                .setAmbientColor(Color3::fromHsv({mapShapes.hue(), 1.0f, 0.3f}))
                .setTransformationMatrix(Matrix4::translation({(Float) mapCoords.x * 2, (Float) (currentMap->height - mapCoords.y - 1) * 2, 0.0f}))
                .setNormalMatrix(transformation.normalMatrix())
                .setProjectionMatrix(projection)
                .draw(mesh);
            }
        }
    }
}

#endif