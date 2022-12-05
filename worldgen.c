#include "common.h"

const int _height = 90;
const int _width = 160;

// Chans att bli en vägg från början
const int wallChanse = 31;

// Hur många tiles som behöver vara väggar för att byta sig själv 3x3
const int maxAdjTiles = 4;

// Hur många tiles som behöver vara väggar för att byta sig själv 5x5
const int maxAdjTiles2 = 15;

// Hur många generationer av kartan ska genereras
const int generations = 5;

int grid[_height][_width];
int gridTemp[_height][_width];

Tile_t tileGrid[_height][_width];

Vector2 _start;
Vector2 _end;

// Ritar ut kartan i konsollen
void Dev_PrintMap(void)
{
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
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
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            grid[y][x] = RandomTile();
        }
    }

    // Gör så tem gridet är bara väggar
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            gridTemp[y][x] = TILE_WALL;
        }
    }

    // Gör alla kanter till väggar
    for (int y = 0; y < _height; y++)
    {
        grid[y][0] = TILE_BEDROCK;
        grid[y][_width - 1] = TILE_BEDROCK;
    }

    for (int x = 0; x < _width; x++)
    {
        grid[0][x] = TILE_BEDROCK;
        grid[_height - 1][x] = TILE_BEDROCK;
    }
    return 1;
}
// Genererar en ny generation av gridet
void NewGeneration(void)
{
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
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
    // Ändrar main grid till ny generation
    for (int y = 1; y < _height - 1; y++)
    {
        for (int x = 1; x < _width - 1; x++)
        {
            grid[y][x] = gridTemp[y][x];
        }
    }
}

int CreateDoors(void)
{
    int done = 0;
    int x, y;
    int tries;

    while (done == 0)
    {
        y = rand() % _height;
        x = rand() % _width;

        if (grid[y][x] == TILE_FLOOR)
        {
            puts("Door");
            grid[y][x] = TILE_START;
            _start = (Vector2){x * tileSize + (tileSize / 2), y * tileSize + (tileSize / 2)};
            done = 1;
        }

        if (tries > 1000)
        {
            puts("Fail");
            return -1; // failed to ever create Entrance
        }
    }
    done = 0;
    while (done == 0)
    {
        y = rand() % _width;
        x = rand() % _height;

        if (grid[y][x] == TILE_FLOOR && grid[y][x] != TILE_START)
        {
            grid[y][x] = TILE_END;
            _end = (Vector2){x * tileSize + (tileSize / 2), y * tileSize + (tileSize / 2)};
            done = 1;
        }

        tries++;

        if (tries > 1000)
        {
            return -1; // failed to ever create Exit
        }
    }

    printf("%d\n", tries);
    return 1;
}

void CreateTileGrid()
{
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            int hp = 0;

            switch (grid[y][x])
            {
            case TILE_FLOOR:
                hp = 0;
                break;
            case TILE_WALL:
                hp = 10;
                break;
            default:
                hp = -1;
                break;
            }

            tileGrid[y][x] = (Tile_t){.type = grid[y][x], .hp = hp};
        }
    }
}

void CheckNeighbours(void)
{
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            tileGrid[y][x].neighbours = 0;

            if (tileGrid[y - 1][x - 1].type == TILE_WALL || tileGrid[y - 1][x - 1].type == TILE_BEDROCK)
            {
                tileGrid[y][x].neighbours = tileGrid[y][x].neighbours | 0b10000000;
            }

            if (tileGrid[y - 1][x].type == TILE_WALL || tileGrid[y - 1][x].type == TILE_BEDROCK)
            {
                tileGrid[y][x].neighbours = tileGrid[y][x].neighbours | 0b01000000;
            }

            if (tileGrid[y - 1][x + 1].type == TILE_WALL || tileGrid[y - 1][x + 1].type == TILE_BEDROCK)
            {
                tileGrid[y][x].neighbours = tileGrid[y][x].neighbours | 0b00100000;
            }

            if (tileGrid[y][x - 1].type == TILE_WALL || tileGrid[y][x - 1].type == TILE_BEDROCK)
            {
                tileGrid[y][x].neighbours = tileGrid[y][x].neighbours | 0b00010000;
            }

            if (tileGrid[y][x + 1].type == TILE_WALL || tileGrid[y][x + 1].type == TILE_BEDROCK)
            {
                tileGrid[y][x].neighbours = tileGrid[y][x].neighbours | 0b00001000;
            }

            if (tileGrid[y + 1][x - 1].type == TILE_WALL || tileGrid[y + 1][x - 1].type == TILE_BEDROCK)
            {
                tileGrid[y][x].neighbours = tileGrid[y][x].neighbours | 0b00000100;
            }

            if (tileGrid[y + 1][x].type == TILE_WALL || tileGrid[y + 1][x].type == TILE_BEDROCK)
            {
                tileGrid[y][x].neighbours = tileGrid[y][x].neighbours | 0b00000010;
            }

            if (tileGrid[y + 1][x + 1].type == TILE_WALL || tileGrid[y + 1][x + 1].type == TILE_BEDROCK)
            {
                tileGrid[y][x].neighbours = tileGrid[y][x].neighbours | 0b00000001;
            }

            // 0b00110000
            // 0b11010000
            // 0b11100000
        }
    }
}

int CreateWord(void)
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

    CreateTileGrid();

    int status = CreateDoors();
    if (status == -1)
    {
        return 0;
    }

    clock_t end = clock();

    printf("%f\n", ((double)end - start) / CLOCKS_PER_SEC);
    return 1;
}
void PrintMap(void)
{

    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            if (tileGrid[y][x].type == TILE_FLOOR)
            {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, GREEN);
            }
            else if (tileGrid[y][x].type == TILE_END)
            {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, GREEN);

                //*16 is for sprite size
                Rectangle pos = {.x = x * tileSize, .y = y * tileSize, .height = tileSize, .width = tileSize};
                Rectangle src = {.x = 5 * 16, .y = 3 * 16, .height = 16, .width = 16};

                DrawTexturePro(tilemap1, src, pos, (Vector2){0}, 0.0f, RAYWHITE);
            }
            else if (tileGrid[y][x].type == TILE_START)
            {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, GREEN);

                //*16 is for sprite size
                Rectangle pos = {.x = x * tileSize, .y = y * tileSize, .height = tileSize, .width = tileSize};
                Rectangle src = {.x = 5 * 16, .y = 3 * 16, .height = 16, .width = 16};

                DrawTexturePro(tilemap1, src, pos, (Vector2){0}, 0.0f, RAYWHITE);
            }
        }
    }
}