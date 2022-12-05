#include "common.h"

void DrawPlayer(Player_t *player)
{
    DrawRectangle(player->position.x - (playerSize / 2), player->position.y - (playerSize / 2), playerSize, playerSize, BLUE);
    DrawRectangle(player->position.x, player->position.y, 1, 1, RED);
}

int CheckCollisionWall(int gridX, int gridY, Vector2 nextPos)
{
    Rectangle player = {.x = nextPos.x, .y = nextPos.y, .height = playerSize, .width = playerSize};

    for (int heightChange = -1; heightChange <= 1; heightChange++)
    {
        for (int witdhChange = -1; witdhChange <= 1; witdhChange++)
        {
            // Hoppa över tilen där spelaren är i
            if (heightChange == 0 && witdhChange == 0)
            {
                continue;
            }

            if (tileGrid[gridY + heightChange][gridX + witdhChange].type == TILE_WALL || tileGrid[gridY + heightChange][gridX + witdhChange].type == TILE_BEDROCK)
            {
                Rectangle tile = {.height = tileSize, .width = tileSize, .y = (gridY + heightChange) * tileSize + (playerSize / 2), .x = (gridX + witdhChange) * tileSize + (playerSize / 2)};

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
    if (tileGrid[gridY][gridX].type == TILE_WALL)
    {
        tileGrid[gridY][gridX].hp -= 1;

        if (tileGrid[gridY][gridX].hp == 0)
        {
            tileGrid[gridY][gridX].type = TILE_FLOOR;
        }

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
        nextPos.y += -2;
        dir = 'u';
    }
    if (IsKeyDown(KEY_S))
    {
        nextPos.y += 2;
        dir = 'd';
    }
    if (IsKeyDown(KEY_A))
    {
        nextPos.x += -2;
        dir = 'l';
    }
    if (IsKeyDown(KEY_D))
    {
        nextPos.x += 2;
        dir = 'r';
    }

    if (IsKeyPressed(KEY_SPACE))
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
