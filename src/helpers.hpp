#ifndef helpers_h
#define helpers_h

struct Coord
{
    int x;
    int y;
};

template<int W>
Coord indexToCoords(int index)
{
    return {index % W, index / W};
};

template<int W>
int coordsToIndex(Coord coords)
{
    return coords.y * W + coords.x;
};

int getSpawnXPos(int shapeStartingIndex, int shapeWidth, int mapWidth)
{
    int rightMostPos = mapWidth - shapeWidth;
    int leftMostPos = -shapeStartingIndex;
    int range = leftMostPos - rightMostPos + 1;
    return leftMostPos + rand() % range;
}

#endif