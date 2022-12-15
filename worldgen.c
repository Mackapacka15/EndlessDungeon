#include "common.h"

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
                for (int widthChange = -1; widthChange <= 1; widthChange++)
                {
                    if (grid[y + heightChange][x + widthChange] != TILE_FLOOR)
                    {
                        adjTilesCount++;
                    }
                }
            }
            // Kollar alla tiles i ett 5x5 grid
            for (int heightChange = -2; heightChange <= 2; heightChange++)
            {
                for (int widthChange = -2; widthChange <= 2; widthChange++)
                {
                    if (grid[y + heightChange][x + widthChange] != TILE_FLOOR)
                    {
                        adjTilesCount2++;
                    }
                }
            }
            // Finns det tillräckligt många väggar intill så byt sig själv till vägg
            // Ändrar på temp grid för att beräkningar som kommer senare ska göras på samma grid
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

        if (CheckValidPlacement(x, y, 5, 4))
        {
            SpawnDoorStructure(x, y, 1);
            x += 2;
            y += 2;

            _start = (Vector2){x * tileSize + (tileSize / 2), y * tileSize + (tileSize / 2)};

            tries++;
            done = 1;
        }

        if (tries > 1000)
        {
            return -1; // failed to ever create Entrance
        }
    }
    done = 0;
    while (done == 0)
    {
        y = rand() % _height;
        x = rand() % _width;

        if (CheckValidPlacement(x, y, 5, 4))
        {
            SpawnDoorStructure(x, y, 0);
            x += 2;
            y += 2;

            _end = (Vector2){x * tileSize + (tileSize / 2), y * tileSize + (tileSize / 2)};

            tries++;
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

void UpdateNeighbours(int gridX, int gridY)
{
    tileGrid[gridY][gridX].neighbours = 0;

    if (tileGrid[gridY - 1][gridX - 1].type == TILE_WALL || tileGrid[gridY - 1][gridX - 1].type == TILE_BEDROCK)
    {
        tileGrid[gridY][gridX].neighbours = tileGrid[gridY][gridX].neighbours | 0b10000000;
    }

    if (tileGrid[gridY - 1][gridX].type == TILE_WALL || tileGrid[gridY - 1][gridX].type == TILE_BEDROCK)
    {
        tileGrid[gridY][gridX].neighbours = tileGrid[gridY][gridX].neighbours | 0b01000000;
    }

    if (tileGrid[gridY - 1][gridX + 1].type == TILE_WALL || tileGrid[gridY - 1][gridX + 1].type == TILE_BEDROCK)
    {
        tileGrid[gridY][gridX].neighbours = tileGrid[gridY][gridX].neighbours | 0b00100000;
    }

    if (tileGrid[gridY][gridX - 1].type == TILE_WALL || tileGrid[gridY][gridX - 1].type == TILE_BEDROCK)
    {
        tileGrid[gridY][gridX].neighbours = tileGrid[gridY][gridX].neighbours | 0b00010000;
    }

    if (tileGrid[gridY][gridX + 1].type == TILE_WALL || tileGrid[gridY][gridX + 1].type == TILE_BEDROCK)
    {
        tileGrid[gridY][gridX].neighbours = tileGrid[gridY][gridX].neighbours | 0b00001000;
    }

    if (tileGrid[gridY + 1][gridX - 1].type == TILE_WALL || tileGrid[gridY + 1][gridX - 1].type == TILE_BEDROCK)
    {
        tileGrid[gridY][gridX].neighbours = tileGrid[gridY][gridX].neighbours | 0b00000100;
    }

    if (tileGrid[gridY + 1][gridX].type == TILE_WALL || tileGrid[gridY + 1][gridX].type == TILE_BEDROCK)
    {
        tileGrid[gridY][gridX].neighbours = tileGrid[gridY][gridX].neighbours | 0b00000010;
    }

    if (tileGrid[gridY + 1][gridX + 1].type == TILE_WALL || tileGrid[gridY + 1][gridX + 1].type == TILE_BEDROCK)
    {
        tileGrid[gridY][gridX].neighbours = tileGrid[gridY][gridX].neighbours | 0b00000001;
    }
}

void CheckNeighbours(void)
{
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            UpdateNeighbours(x, y);
        }
    }
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

    CheckNeighbours();

    clock_t end = clock();

    printf("%f\n", ((double)end - start) / CLOCKS_PER_SEC);
    return 1;
}

Rectangle GetTileMapRect(int posX, int posY, int tilemapWidth)
{
    return (Rectangle){
        .y = (int)(posY * 16),
        .x = (int)(posX * 16),
        .width = 16,
        .height = 16};
}

void DrawMap(void)
{
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            Rectangle pos = {.x = x * tileSize, .y = y * tileSize, .height = tileSize, .width = tileSize};

            if (tileGrid[y][x].type == TILE_WALL)
            {
                DrawTexturePro(tilemap2, wallTileset[tileGrid[y][x].neighbours], pos, (Vector2){0}, 0.0f, RAYWHITE);
            }
            else
            {
                DrawTexturePro(tilemap1, decorationTiles[tileGrid[y][x].type], pos, (Vector2){0}, 0.0f, RAYWHITE);
            }
        }
    }
}