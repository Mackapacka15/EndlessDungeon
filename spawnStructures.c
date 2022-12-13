#include "common.h"

int CheckValidPlacement(int topLeftX, int topLeftY, int width, int height)
{
    for (int heightChange = 0; heightChange < height; heightChange++)
    {
        for (int widthChange = 0; widthChange < width; widthChange++)
        {
            if (tileGrid[topLeftY + heightChange][topLeftX + widthChange].type != TILE_FLOOR && tileGrid[topLeftY + heightChange][topLeftX + widthChange].type != TILE_WALL)
            {
                return 0;
            }
        }
    }
    return 1;
}

void SpawnDoorStructure(int topLeftX, int topLeftY, int isStart)
{
    if (isStart)
    {
        tileGrid[topLeftY + 2][topLeftX + 2].type = TILE_START;
    }
    else
    {
        tileGrid[topLeftY + 2][topLeftX + 2].type = TILE_END;
    }

    tileGrid[topLeftY + 3][topLeftX + 2].type = TILE_BEDROCK;

    tileGrid[topLeftY + 0][topLeftX + 0].type = TILE_FLOOR;
    tileGrid[topLeftY + 0][topLeftX + 1].type = TILE_FLOOR;
    tileGrid[topLeftY + 0][topLeftX + 2].type = TILE_FLOOR;
    tileGrid[topLeftY + 0][topLeftX + 3].type = TILE_FLOOR;
    tileGrid[topLeftY + 0][topLeftX + 4].type = TILE_FLOOR;

    tileGrid[topLeftY + 1][topLeftX + 0].type = TILE_FLOOR;
    tileGrid[topLeftY + 1][topLeftX + 1].type = TILE_FLOOR;
    tileGrid[topLeftY + 1][topLeftX + 2].type = TILE_FLOOR;
    tileGrid[topLeftY + 1][topLeftX + 3].type = TILE_FLOOR;
    tileGrid[topLeftY + 1][topLeftX + 4].type = TILE_FLOOR;

    tileGrid[topLeftY + 2][topLeftX + 0].type = TILE_FLOOR;
    tileGrid[topLeftY + 2][topLeftX + 1].type = TILE_FLOOR;
    tileGrid[topLeftY + 2][topLeftX + 3].type = TILE_FLOOR;
    tileGrid[topLeftY + 2][topLeftX + 4].type = TILE_FLOOR;

    tileGrid[topLeftY + 3][topLeftX + 0].type = TILE_FLOOR;
    tileGrid[topLeftY + 3][topLeftX + 1].type = TILE_FLOOR;
    tileGrid[topLeftY + 3][topLeftX + 3].type = TILE_FLOOR;
    tileGrid[topLeftY + 3][topLeftX + 4].type = TILE_FLOOR;

    tileGrid[topLeftY + 4][topLeftX + 0].type = TILE_FLOOR;
    tileGrid[topLeftY + 4][topLeftX + 1].type = TILE_FLOOR;
    tileGrid[topLeftY + 4][topLeftX + 2].type = TILE_FLOOR;
    tileGrid[topLeftY + 4][topLeftX + 3].type = TILE_FLOOR;
    tileGrid[topLeftY + 4][topLeftX + 4].type = TILE_FLOOR;
}