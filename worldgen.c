#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TILE_FLOOR 0
#define TILE_WALL 1
#define TILE_START 2
#define TILE_END 3

const int width = 100;
const int height = 100;

// Chans att bli en vägg från början
const int wallChanse = 31;

// Hur många tiles som behöver vara väggar för att byta sig själv 3x3
const int maxAdjTiles = 4;

// Hur många tiles som behöver vara väggar för att byta sig själv 5x5
const int maxAdjTiles2 = 15;

// Hur många generationer av kartan ska genereras
const int generations = 5;

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
            case TILE_START:
                putchar('5');
                break;
            case TILE_END:
                putchar('8');
                break;
            }
        }
        putchar('\n');
    }
}

int CreateDoors(void)
{
    int done = 0;
    int x, y;
    int tries;

    while (done == 0)
    {
        y = rand() % height;
        x = rand() % width;

        if (grid[y][x] == TILE_FLOOR)
        {
            grid[y][x] = TILE_START;
            done = 1;
        }

        if (tries > 10000)
        {
            return -1; // failed to ever create Entrance
        }
    }
    done = 0;
    while (done == 0)
    {
        y = rand() % height;
        x = rand() % width;

        if (grid[y][x] == TILE_FLOOR && grid[y][x] != TILE_START)
        {
            grid[y][x] = TILE_END;
            done = 1;
        }

        tries++;

        if (tries > 10000)
        {
            return -1; // failed to ever create Exit
        }
    }

    printf("%d\n", tries);
    return 1;
}

int main(void)
{
    srand(time(NULL));

    clock_t start = clock();

    // Skapar den ursprungliga kartan
    InnitMap();
    // Skapar nya generationer av kartan
    for (int i = 0; i < generations; i++)
    {
        NewGeneration();
    }
    int status = CreateDoors();
    if (status == -1)
    {
        return 0;
    }

    clock_t end = clock();

    // Ritar ut kartan
    PrintMap();

    printf("%f\n", ((double)end - start) / CLOCKS_PER_SEC);
    // Avslutar programmet
    return 0;
}