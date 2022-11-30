#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#include "worldgen.c"

const int ScreenHeight = 900;
const int ScreenWidth = 1600;

const int tileSize = 10;
typedef enum
{
    GameStateMainMenu,
    GameStateGame,
    GameStateDead,
} Game_State_e;

Game_State_e state = GameStateGame;
typedef struct Player_t
{
    Vector2 position;
    int dmg;
    int hp;
} Player_t;

void PrintMap(void)
{
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            if (grid[y][x] == TILE_FLOOR)
            {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, GREEN);
            }
            else if (grid[y][x] == TILE_END)
            {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, YELLOW);
            }
            else if (grid[y][x] == TILE_START)
            {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, ORANGE);
            }
        }
    }
}

void DrawPlayer(Player_t *player)
{
    DrawRectangle(player->position.x - 5, player->position.y - 5, 10, 10, BLUE);
    DrawRectangle(player->position.x, player->position.y, 1, 1, RED);
}

void NewWorld(Player_t *player)
{
    player->position = _start;
}
int CheckCollisionWall(int gridX, int gridY, Vector2 nextPos)
{
    Rectangle player = {.x = nextPos.x, .y = nextPos.y, .height = tileSize, .width = tileSize};

    for (int heightChange = -1; heightChange <= 1; heightChange++)
    {
        for (int witdhChange = -1; witdhChange <= 1; witdhChange++)
        {
            if (heightChange == 0 && witdhChange == 0)
            {
                continue;
            }

            if (grid[gridY + heightChange][gridX + witdhChange] == TILE_WALL)
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

void LevelDone(Player_t *player)
{
    Rectangle exitRec = {.height = tileSize, .width = tileSize, .y = _end.y, .x = _end.x};

    Rectangle playerRec = {.x = player->position.x, .y = player->position.y, .height = tileSize, .width = tileSize};

    if (CheckCollisionRecs(playerRec, exitRec))
    {
        puts("Done\n");
        state = GameStateDead;
    }
}

void UpdatePlayer(Player_t *player)
{
    int x = player->position.x / tileSize;
    int y = player->position.y / tileSize;
    // DrawRectangle(x * 10 - 10, y * 10 - 10, 30, 30, BROWN);

    Vector2 nextPos = player->position;

    if (IsKeyDown(KEY_W))
    {
        nextPos.y += -1;
    }
    if (IsKeyDown(KEY_S))
    {
        nextPos.y += 1;
    }
    if (IsKeyDown(KEY_A))
    {
        nextPos.x += -1;
    }
    if (IsKeyDown(KEY_D))
    {
        nextPos.x += 1;
    }

    if (CheckCollisionWall(x, y, nextPos) == 1)
    {
        player->position = nextPos;
    }
}

int main(void)
{
    Player_t player = {.dmg = 1, .hp = 100, .position = (Vector2){ScreenWidth / 2.0f, ScreenHeight / 2.0f}};

    Camera2D camera = {0};
    camera.offset = (Vector2){ScreenWidth / 2.0f, ScreenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 2.5f;
    camera.target = player.position;

    int status = CreateWord();
    // Sert till så det finns en värld
    while (status != 1)
    {
        printf("%d\n", status);
        status = CreateWord();
    }
    SetTargetFPS(60);
    InitWindow(ScreenWidth, ScreenHeight, "MineHero");

    // Ritar ut kartan
    // Dev_PrintMap();
    NewWorld(&player);
    while (!WindowShouldClose())
    {
        //Update
        switch (state)
        {
        case GameStateGame:
            camera.target = player.position;
            LevelDone(&player);
            UpdatePlayer(&player);
            break;
        default:
            break;
        }

        //Draw
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        switch (state)
        {
        case GameStateGame:
            PrintMap();
            DrawPlayer(&player);
            break;

        default:
            break;
        }

        EndMode2D();
        EndDrawing();
    }

    // Avslutar programmet
    CloseWindow();
    return 0;
}