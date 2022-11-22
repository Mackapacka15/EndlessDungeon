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

void NewGeneration(void)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int adjTilesCount = 0;
            int adjTilesCount2 = 0;

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

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            grid[y][x] = gridTemp[y][x];
        }
    }
}

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

    InnitMap();

    for (int i = 0; i < generations; i++)
    {
        NewGeneration();
    }

    PrintMap();
    printf("Hello World \n");
    return 0;
}