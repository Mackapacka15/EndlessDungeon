#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TILE_FLOOR 0
#define TILE_WALL 1

const int width = 20;
const int height = 20;

// Chans att bli en vägg från början
const int wallChanse = 20;

// Hur många tiles som behöver vara väggar för att byta sig själv 3x3
const int maxAdjTiles = 4;

// Hur många tiles som behöver vara väggar för att byta sig själv 5x5
const int maxAdjTiles2 = 16;

// Hur många generationer av kartan ska genereras
const int generations = 3;

int grid[height][width];
int gridTemp[height][width];

int RandomTile(void)
{
    if (rand() % 100 < wallChanse)
    {
        return TILE_WALL;
    }
    else
    {
        return TILE_FLOOR;
    }
}
// Skapar första versionen av kartan
int InnitMap(void)
{
    // Ger random values till alla platser i gridet
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            grid[y][x] = RandomTile();
        }
    }

    // Gör så tem gridet är bara väggar
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            gridTemp[y][x] = TILE_WALL;
        }
    }

    // Gör alla kanter till väggar
    for (int y = 0; y < width; y++)
    {
        grid[y][0] = TILE_WALL;
        grid[y][width - 1] = TILE_WALL;
    }

    for (int x = 0; x < width; x++)
    {
        grid[0][x] = TILE_WALL;
        grid[height - 1][x] = TILE_WALL;
    }
    return 1;
}
// Genererar en ny generation av gridet
void NewGeneration(void)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int adjTilesCount = 0;
            int adjTilesCount2 = 0;

            // Kollar alla tiles i ett 3x3 grid
            for (int heightChange = -1; heightChange <= 1; heightChange++)
            {
                for (int witdhChange = -1; witdhChange <= 1; witdhChange++)
                {
                    if (grid[y + heightChange][x + witdhChange] != TILE_FLOOR)
                    {
                        adjTilesCount++;
                    }
                }
            }
            // Kollar alla tiles i ett 5x5 grid
            for (int heightChange = -2; heightChange <= 2; heightChange++)
            {
                for (int witdhChange = -2; witdhChange <= 2; witdhChange++)
                {
                    if (grid[y + heightChange][x + witdhChange] != TILE_FLOOR)
                    {
                        adjTilesCount2++;
                    }
                }
            }
            // Finns det tillräckligt många väggar intill så byt sig själv till vägg
            //Ändrar på temp grid för att beräkningar som kommer senare ska göras på samma grid
            if (adjTilesCount >= maxAdjTiles || adjTilesCount2 >= maxAdjTiles2)
            {
                gridTemp[y][x] = TILE_WALL;
            }
            else
            {
                gridTemp[y][x] = TILE_FLOOR;
            }
        }
    }
    // Ser till så båda gridsen ser likadana ut
    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            grid[y][x] = gridTemp[y][x];
        }
    }
}
// Ritar ut kartan
void PrintMap(void)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            switch (grid[y][x])
            {
            case TILE_WALL:
                putchar('#');
                break;
            case TILE_FLOOR:
                putchar('.');
                break;
            }
        }
        putchar('\n');
    }
}

int main(void)
{
    srand(time(NULL));
    // Skapar den ursprungliga kartan
    InnitMap();
    // Skapar nya generationer av kartan
    for (int i = 0; i < generations; i++)
    {
        NewGeneration();
    }
    // Ritar ut kartan
    PrintMap();

    // Avslutar programmet
    return 0;
}