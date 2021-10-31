#ifndef Shape_h
#define Shape_h

#include <Magnum/Math/Vector2.h>
#include "helpers.hpp"
#include <array>
#include <utility>


enum ShapeType
{
    S,
    Z,
    T,
    L,
    SQUARE,
    L_INV,
    LINE
};

class Shape
{
    public:
        int8_t width;
        int8_t height;
        ShapeType shapeType;
        std::array<bool, 16> shape;
        std::array<bool, 16> prevShape;
        Coord position;
        Coord prevPos;

    private:
        bool lastMovementWasRotate = false;

    public:
        Shape(ShapeType shapeType, std::array<bool, 16> shape, std::array<int8_t, 2> position)
        : shapeType(shapeType)
        , shape(shape)
        , position({position[0], position[1]})
        , prevPos({position[0], position[1]})
        , prevShape(shape)
        {};
    
        void moveDown()  { setPosition(position.x, position.y - 1); }
        void moveLeft()  { setPosition(position.x - 1, position.y); }
        void moveRight() { setPosition(position.x + 1, position.y); }
        void rotate();
        void undoLastMovement();

        std::pair<int, int> getBoundingWidth();

    private:
        void setPosition(int x, int y);
};

void Shape::rotate()
{
    lastMovementWasRotate = true;
    prevShape = shape;
    
    std::array<bool, 16> nextRot;
    nextRot.fill({});

    for (int8_t y = 0; y < 4; y++)
    {
        for (int8_t x = 0; x < 4; x++)
        {
            nextRot[x * 4 + 3-y] = shape[y * 4 + x];
        }
    }

    shape = nextRot;
}

void Shape::setPosition(int x, int y)
{
    lastMovementWasRotate = false;
    prevPos = position;
    position = {x, y};
}

void Shape::undoLastMovement()
{
    if (lastMovementWasRotate)
    {
        shape = prevShape;
    } else
    {
        position = prevPos;
    }
}

std::pair<int, int> Shape::getBoundingWidth()
{
    // returns {x index of first block inside shape, horizontal width of shape}
    int startingIndex = -1;
    int boundingWidth = -1;

    for (int8_t x = 0; x < 4; x++)
    {
        for (int8_t y = 0; y < 4; y++)
        {
            Coord relCoord {x, y};

            if (shape[coordsToIndex<4>(relCoord)] == 1)
            {
                startingIndex = x;
                break;
            }
        }

        if (startingIndex != -1)
        {
            break;
        }
    }

    for (int x = 4 - 1; x >= 0; x--)
    {
        for (int8_t y = 0; y < 4; y++)
        {
            Coord relCoord {x, y};

            if (shape[coordsToIndex<4>(relCoord)] == 1)
            {
                boundingWidth = x - startingIndex + 1;
                break;
            }
        }

        if (boundingWidth != -1)
        {
            break;
        }
    }

    return std::pair<int, int> {startingIndex, boundingWidth};
}

#endif