#include "common.h"

typedef struct Player_t
{
    Vector2 position;
    int dmg;
    int hp;
} Player_t;

void DrawPlayer(Player_t *player)
{
    DrawRectangle(player->position.x - 5, player->position.y - 5, 10, 10, BLUE);
    DrawRectangle(player->position.x, player->position.y, 1, 1, RED);
}

int CheckCollisionWall(int gridX, int gridY, Vector2 nextPos)
{
    Rectangle player = {.x = nextPos.x, .y = nextPos.y, .height = tileSize, .width = tileSize};

    for (int heightChange = -1; heightChange <= 1; heightChange++)
    {
        for (int witdhChange = -1; witdhChange <= 1; witdhChange++)
        {
            // Hoppa över tilen där spelaren är i
            if (heightChange == 0 && witdhChange == 0)
            {
                continue;
            }

            if (grid[gridY + heightChange][gridX + witdhChange] == TILE_WALL || grid[gridY + heightChange][gridX + witdhChange] == TILE_BEDROCK)
            {
                Rectangle tile = {.height = tileSize, .width = tileSize, .y = (gridY + heightChange) * tileSize + 5, .x = (gridX + witdhChange) * tileSize + 5};

                if (CheckCollisionRecs(tile, player))
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}

int MineBlock(int gridX, int gridY)
{
    if (grid[gridY][gridX] == TILE_WALL)
    {
        grid[gridY][gridX] = TILE_FLOOR;
        return 1;
    }
    return 0;
}

void UpdatePlayer(Player_t *player)
{
    static char dir = 'u';
    int x = player->position.x / tileSize;
    int y = player->position.y / tileSize;
    // DrawRectangle(x * 10 - 10, y * 10 - 10, 30, 30, BROWN);

    Vector2 nextPos = player->position;

    if (IsKeyDown(KEY_W))
    {
        nextPos.y += -1;
        dir = 'u';
    }
    if (IsKeyDown(KEY_S))
    {
        nextPos.y += 1;
        dir = 'd';
    }
    if (IsKeyDown(KEY_A))
    {
        nextPos.x += -1;
        dir = 'l';
    }
    if (IsKeyDown(KEY_D))
    {
        nextPos.x += 1;
        dir = 'r';
    }

    if (IsKeyDown(KEY_SPACE))
    {
        switch (dir)
        {
        case 'u':
            MineBlock(x, y - 1);
            break;
        case 'd':
            MineBlock(x, y + 1);
            break;
        case 'l':
            MineBlock(x - 1, y);
            break;
        case 'r':
            MineBlock(x + 1, y);
            break;
        default:
            break;
        }
    }

    if (CheckCollisionWall(x, y, nextPos) == 1)
    {
        player->position = nextPos;
    }
}
