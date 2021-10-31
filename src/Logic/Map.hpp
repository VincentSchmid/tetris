#ifndef Map_h
#define Map_h

#include "Shapes/Shape.hpp"
#include "helpers.hpp"


template<int W, int H>
class Map
{
    public:
        int width = W;
        int height = H;
        std::array<bool, W * H> map;

    public:
        Map()
        {
            map.fill({});
        };

        bool checkCollision(Shape *shape);
        int checkLines();
        void addBlock(Shape shape);
        Coord coordsToMapPos(Coord coord);
        void shiftDown(int row);

    private:
        bool isOnMap(Coord coords);
        Coord indexToMapPos(int index);
};


template<int W, int H>
bool Map<W, H>::checkCollision(Shape *shape)
{
    for (int8_t i = 0; i < shape->shape.size(); i++)
    {
        Coord relativePos = indexToCoords<4>(i);
        Coord posOnMap = {shape->position.x + relativePos.x, shape->position.y - relativePos.y};
        posOnMap = coordsToMapPos(posOnMap);

        // check if shape is there
        if (shape->shape[i] == 1)
        {
            //check if is inside map
            if (!isOnMap(posOnMap))
            {
                return true;
            }

            // check if it collides with object on map
            else if (map[coordsToIndex<W>(posOnMap)] == 1)
            {
                return true;
            }
        }
    }

    return false;
}

template<int W, int H>
void Map<W, H>::addBlock(Shape shape)
{
    for (int8_t i = 0; i < shape.shape.size(); i++)
    {
        Coord relativePos = indexToCoords<4>(i);
        Coord posOnMap = {shape.position.x + relativePos.x, shape.position.y - relativePos.y};
        posOnMap = coordsToMapPos(posOnMap);

        if (shape.shape[i] == 1)
        {
            map[coordsToIndex<W>(posOnMap)] = 1;
        }
    }
}

template<int W, int H>
int Map<W, H>::checkLines()
{
    // starting form the top, will check the lines 
    // if they were filled by ones
    // will return index of row that is filled
    int count;
    for (int y = H - 1; y >= 0; y--)
    {
        count = 0;
        for (int x = 0; x < W; x++)
        {
            Coord currCoords = coordsToMapPos({x, y});
            int index = coordsToIndex<W>(currCoords);

            if (map[index] == 1)
            {
                count++;
            }
        }

        if (count == W)
        {
            return y;
        }
    }

    return -1;
}

template<int W, int H>
void Map<W, H>::shiftDown(int row)
{
    // will shift all rows down by 1 including passed row
    // first row will be filled with 0
    std::array<bool, W> prevRow;
    prevRow.fill({});
    for (int y = H - 1; y >= 0; y--)
    {
        for (int x = 0; x < W; x++)
        {
            Coord currCoords = coordsToMapPos({x, y});
            int index = coordsToIndex<W>(currCoords);

            int tmp = map[index];
            map[index] = prevRow[x];
            prevRow[x] = tmp;
        }   

        if (y == row)
        {
            break;
        }
    }
}

template<int W, int H>
Coord Map<W, H>::coordsToMapPos(Coord coord)
{
    return {coord.x, H - coord.y - 1};
}

template<int W, int H>
Coord Map<W, H>::indexToMapPos(int index)
{
    return coordsToMapPos(indexToCoords<W>(index));
}


template<int W, int H>
bool Map<W, H>::isOnMap(Coord coords)
{
    if (coords.x < 0 || coords.x >= W)
    {
        return false;
    }

    if (coords.y < 0 || coords.y >= H)
    {
        return false;
    }

    return true;
}

#endif