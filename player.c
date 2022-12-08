#include "common.h"

void DrawPlayer(Player_t *player)
{
    DrawRectangle(player->position.x - (playerSize / 2), player->position.y - (playerSize / 2), playerSize, playerSize, BLUE);
    DrawRectangle(player->position.x, player->position.y, 1, 1, RED);
}

Vector2 CheckCollisionWall(int gridX, int gridY, Vector2 nextPos, Player_t *p)
{
    Vector2 playerPos = p->position;
    Rectangle newPlayer = {.x = playerPos.x + nextPos.x, .y = playerPos.y + nextPos.y, .height = playerSize, .width = playerSize};

    for (int heightChange = -1; heightChange <= 1; heightChange++)
    {
        for (int widthChange = -1; widthChange <= 1; widthChange++)
        {
            // Hoppa över tilen där spelaren är i
            if (heightChange == 0 && widthChange == 0)
            {
                continue;
            }

            if (tileGrid[gridY + heightChange][gridX + widthChange].type == TILE_WALL || tileGrid[gridY + heightChange][gridX + widthChange].type == TILE_BEDROCK)
            {
                Rectangle tile = {.height = tileSize, .width = tileSize, .y = (gridY + heightChange) * tileSize + (playerSize / 2), .x = (gridX + widthChange) * tileSize + (playerSize / 2)};

                if (CheckCollisionRecs(tile, newPlayer))
                {

                    if (playerPos.y >= tile.y + tile.height && (playerPos.x < tile.x + tile.width && playerPos.x + playerSize > tile.x))
                    {
                        puts("Col Top");
                        nextPos.y = fmax(0, nextPos.y);
                    }

                    else if (playerPos.y + newPlayer.height <= tile.y)
                    {
                        nextPos.y = fmin(0, nextPos.y);
                    }

                    if (playerPos.x >= tile.x + tile.width)
                    {
                        nextPos.x = fmax(0, nextPos.x);
                    }

                    else if (playerPos.x + newPlayer.width <= tile.x)
                    {
                        nextPos.x = fmin(0, nextPos.x);
                    }

                    // if (widthChange)
                    // {
                    //     nextPos.x = widthChange < 0 ? fmax(0, nextPos.x) : fmin(0, nextPos.x);
                    // }

                    // if (heightChange)
                    // {
                    //     nextPos.y = heightChange < 0 ? fmax(0, nextPos.y) : fmin(0, nextPos.y);
                    // }

                    // if ((heightChange == 1 && (widthChange == -1 || widthChange == 0 || widthChange == 1)) || (heightChange == 0 && (widthChange == -1 || widthChange == 0 || widthChange == 1)) || (heightChange == -1 && (widthChange == -1 || widthChange == 0 || widthChange == 1)))
                    // {
                    //     nextPos.y = 0;
                    // }
                }
            }
        }
    }

    return (Vector2){nextPos.x, nextPos.y};
}

int MineBlock(int gridX, int gridY, int dmg)
{
    if (tileGrid[gridY][gridX].type == TILE_WALL)
    {
        tileGrid[gridY][gridX].hp -= dmg;

        if (tileGrid[gridY][gridX].hp == 0)
        {
            tileGrid[gridY][gridX].type = TILE_FLOOR;

            // Update neighbour count for adjacent tiles
            for (int heightChange = -1; heightChange <= 1; heightChange++)
            {
                for (int widthChange = -1; widthChange <= 1; widthChange++)
                {
                    UpdateNeighbours(gridX + widthChange, gridY + heightChange);
                }
            }
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

    Vector2 nextPos = {0};

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
            MineBlock(x, y - 1, player->dmg);
            break;
        case 'd':
            MineBlock(x, y + 1, player->dmg);
            break;
        case 'l':
            MineBlock(x - 1, y, player->dmg);
            break;
        case 'r':
            MineBlock(x + 1, y, player->dmg);
            break;
        default:
            break;
        }
    }

    Vector2 newPos = CheckCollisionWall(x, y, nextPos, player);

    player->position.x += newPos.x;
    player->position.y += newPos.y;
}
