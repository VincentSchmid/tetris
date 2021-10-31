// This line should be included only in one file 
// it instructs catch2 to generate the main() for you
// (important: the define should be before the include)
#define CATCH_CONFIG_MAIN

#include <array>
#include <unordered_map>
#include <string>
#include <iostream>
#include <utility>

#include "catch2/catch.hpp"

#include "helpers.hpp"
#include "Shapes/Shape.hpp"
#include "Logic/Map.hpp"
#include "Shapes/ShapeFactory.hpp"
#include "Logic/Game.hpp"


#define COMPARE_ARRAYS(lhs, rhs) compareArrays(Catch::getResultCapture().getCurrentTestName(), __LINE__, lhs, rhs)

template < typename T, size_t N >
void compareArrays(const std::string & test, unsigned line, std::array<T, N> lhs, std::array<T, N> rhs) {
  std::vector<T> lv(lhs.begin(), lhs.end());
  std::vector<T> rv(rhs.begin(), rhs.end());
  INFO("Test case [" << test << "] failed at line " << line); // Reported only if REQUIRE fails
  REQUIRE(lv == rv);
}

// Test case is a single test that you run
// You give it a name/description and also you give it some tags.
TEST_CASE("Testing framework is working fine", "[Catch2]")
{

    // Tests have to meet some requirements to be considered valid
    REQUIRE(true);
}

TEST_CASE("Get Value from unordered_map, [CPP]")
{
    std::unordered_map<ShapeType, std::array<bool, 2>> testData
    {
        {ShapeType::S, {0, 1}},
        {ShapeType::Z, {0, 0}}
    };

    std::array<bool, 2> testArr = {0, 0};

    COMPARE_ARRAYS(testData.at(ShapeType::Z), testArr);
}

TEST_CASE("Shape can be instatiated", "[Shape]")
{
    std::array<bool, 16> testShape = {0, 1, 0, 0,
                                      0, 1, 1, 0,
                                      0, 0, 1, 0,
                                      0, 0, 0, 0};

    Shape myShape = Shape(ShapeType::S, testShape, {0, 0});

    REQUIRE(myShape.shapeType == ShapeType::S);
}

SCENARIO("Rotating shape", "[Shape]")
{
    GIVEN("A rotated shape")
    {
         std::array<bool, 16> testShape = {0, 1, 0, 0,
                                           0, 1, 1, 0,
                                           0, 0, 1, 0,
                                           0, 0, 0, 0};

        Shape myShape = Shape(ShapeType::S, testShape, {0, 0});

        myShape.rotate();

        // VALIDATE ASSUMPTION OF GIVEN CLAUSE
        THEN("The shape changed")
        {
            std::array<bool, 16> rotated = {0, 0, 0, 0,
                                            0, 0, 1, 1,
                                            0, 1, 1, 0,
                                            0, 0, 0, 0};
                
            COMPARE_ARRAYS(myShape.shape, rotated);
        }

        THEN("bounding width of shape")
        {
            std::pair<int, int> boundginWidth = myShape.getBoundingWidth();
            REQUIRE(boundginWidth.first == 1);
            REQUIRE(boundginWidth.second == 3);
        }
    
        // validate one use case for given clause
        WHEN("undoing the rotation")
        {
            myShape.undoLastMovement();

            THEN("shape returns to origianl")
            {
                COMPARE_ARRAYS(myShape.shape, testShape);
            }

            THEN("bounding width changes")
            {
                std::pair<int, int> boundginWidth = myShape.getBoundingWidth();
                REQUIRE(boundginWidth.first == 1);
                REQUIRE(boundginWidth.second == 2);
            }
        }
    }
    
}

TEST_CASE("index to coord, [helpers]")
{
    Coord myCoord = indexToCoords<4>(6);

    UNSCOPED_INFO("x: " << myCoord.x << ", y: " << myCoord.y);

    REQUIRE(myCoord.x == 2);
    REQUIRE(myCoord.y == 1);

    myCoord = indexToCoords<4>(9);

    UNSCOPED_INFO("x: " << myCoord.x << ", y: " << myCoord.y);
    
    REQUIRE(myCoord.x == 1);
    REQUIRE(myCoord.y == 2);
}

TEST_CASE("coord to index, [helpers]")
{
    Coord myCoord {2, 1};
    int myIndex = coordsToIndex<4>(myCoord);

    REQUIRE(myIndex == 6);

    myCoord = {1, 2};
    myIndex = coordsToIndex<4>(myCoord);

    REQUIRE(myIndex == 9);
}

TEST_CASE("Instatiating Map, [MAP]")
{
    Map<5, 4> myMap = Map<5, 4>();
}

SCENARIO("filling and removing lines, [MAP]")
{
    GIVEN("Map with full row")
    {
        std::array<bool, 16> testShape = {0, 1, 0, 0,
                                          1, 1, 1, 1,
                                          0, 0, 1, 0,
                                          0, 0, 0, 1};

        Map<4, 4> myMap = Map<4, 4>();
        myMap.map = testShape;

        THEN("checkLines() will return 2")
        {
            REQUIRE(myMap.checkLines() == 2);
        }

        WHEN("shiftDown() is called first row moves down")
        {
            myMap.shiftDown(2);

            std::array<bool, 16> newMap = {0, 0, 0, 0,
                                           0, 1, 0, 0,
                                           0, 0, 1, 0,
                                           0, 0, 0, 1};

            COMPARE_ARRAYS(myMap.map, newMap);
        }

        WHEN("after shiftDown() checkLines() returns -1")
        {
            myMap.shiftDown(2);

            REQUIRE(myMap.checkLines() == -1);
        }
    }
}

SCENARIO("Adding blocks moving and colliding, [MAP]")
{
    GIVEN("Adding a block")
    {
        std::array<bool, 16> testShape = {0, 1, 0, 0,
                                          0, 1, 1, 0,
                                          0, 0, 1, 0,
                                          0, 0, 0, 0};

        Map<6, 5> myMap = Map<6, 5>();
        Shape myShape = Shape(ShapeType::S, testShape, {1, 3});
        myMap.addBlock(myShape);

        THEN("block is added to map")
        {
            std::string output = ""; 

            for (int y = 0; y < 5; y++)
            {
                for (int x = 0; x < 6; x++)
                {
                    Coord curr {x, y};

                    if (myMap.map[coordsToIndex<6>(curr)] == true)
                    {
                        output += "X ";
                    } else
                    {
                        output += "O ";
                    }
                }
                output += "\n";
            }

            UNSCOPED_INFO(output);

            std::array<bool, 6 * 5> expectedMap = {0, 0, 0, 0, 0, 0,
                                                   0, 0, 1, 0, 0, 0,
                                                   0, 0, 1, 1, 0, 0,
                                                   0, 0, 0, 1, 0, 0,
                                                   0, 0, 0, 0, 0, 0};

            COMPARE_ARRAYS(myMap.map, expectedMap);
        }

        WHEN("shape is moved to the left")
        {
            myShape.moveLeft();

            THEN("x position is offset by one")
            {
                REQUIRE(myShape.position.x == 0);
            }
        }

        WHEN("shape is moved down")
        {
            myShape.moveRight();
            myShape.moveRight();
            myShape.moveDown();

            THEN("y position changes")
            {
                REQUIRE(myShape.position.y == 2);
            }

            THEN("block is lower in the map")
            {
                myMap.addBlock(myShape);

                std::array<bool, 6 * 5> expectedMap = {0, 0, 0, 0, 0, 0,
                                                       0, 0, 1, 0, 0, 0,
                                                       0, 0, 1, 1, 1, 0,
                                                       0, 0, 0, 1, 1, 1,
                                                       0, 0, 0, 0, 0, 1};
                
                COMPARE_ARRAYS(myMap.map, expectedMap);
            }
        }
    
        WHEN("shape is moved to the right twice")
        {
            myShape.moveRight();
            myShape.moveRight();

            THEN("x position is ofset by 2")
            {
                REQUIRE(myShape.position.x == 3);
            }

            THEN("shape should not collide with map")
            {
                REQUIRE_FALSE(myMap.checkCollision(&myShape));
            }
        }

        WHEN("shape is collding with object")
        {
            myShape.moveLeft();

            THEN("shape position changes")
            {
                REQUIRE(myShape.position.x == 0);
                REQUIRE(myShape.position.y == 3);
            }

            THEN("shape collides")
            {
                REQUIRE(myMap.checkCollision(&myShape));
            }
        }

        WHEN("shape is colliding with walls")
        {
            myShape.moveRight();
            myShape.moveRight();
            myShape.moveRight();

            THEN("shape position changes")
            {
                REQUIRE(myShape.position.x == 4);
                REQUIRE(myShape.position.y == 3);
            }

            THEN("shape collides")
            {
                REQUIRE(myMap.checkCollision(&myShape));
            }
        }
    }
}
