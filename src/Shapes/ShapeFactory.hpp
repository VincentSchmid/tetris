#ifndef ShapeFactory_h
#define ShapeFactory_h

#include <math.h>
#include <ctime>
#include <unordered_map>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <array>

#include "Shape.hpp"


class ShapeFactory
{
    private:
    std::unordered_map<ShapeType, std::array<bool, 16>> shapeData
    {
        {ShapeType::S, {0, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 0}},

        {ShapeType::Z, {0, 0, 1, 0,
                        0, 1, 1, 0,
                        0, 1, 0, 0,
                        0, 0, 0, 0}},

        {ShapeType::T, {0, 0, 1, 0,
                        0, 1, 1, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 0}},
        
        {ShapeType::L, {0, 1, 0, 0,
                        0, 1, 0, 0,
                        0, 1, 1, 0,
                        0, 0, 0, 0}},

        {ShapeType::L_INV, {0, 0, 1, 0,
                            0, 0, 1, 0,
                            0, 1, 1, 0,
                            0, 0, 0, 0}},
        
        {ShapeType::LINE, {0, 0, 1, 0,
                           0, 0, 1, 0,
                           0, 0, 1, 0,
                           0, 0, 1, 0}}
    };

    public:
        ShapeFactory() { srand(time(0)); }
        Shape getShape(bool randOrientation);
        Shape getShape(ShapeType shapeType, bool randOrientation);
        
    private:
        void randomOrientation(Shape shape);
        ShapeType getRandomShapeType();
        std::array<ShapeType, 3> symShps = {ShapeType::S, ShapeType::Z, ShapeType::LINE};
};

Shape ShapeFactory::getShape(bool randOrientation)
{
    return getShape(getRandomShapeType(), randOrientation);
}

Shape ShapeFactory::getShape(ShapeType shapeType, bool randOrientation)
{
    // bool symetricalRot = std::find(std::begin(symShps), std::end(symShps), shapeType) != std::end(symShps);

    Shape newShape {shapeType, shapeData.at(shapeType), {0, 0}};

    if (randOrientation)
    {
        randomOrientation(newShape);
    }

    return newShape;
}

void ShapeFactory::randomOrientation(Shape shape)
{
    for (int i = 0; i < rand() % 4; i++)
    {
        shape.rotate();
    }
}

ShapeType ShapeFactory::getRandomShapeType()
{
    return ShapeType(rand() % ShapeType::LINE + 1);
}

#endif