#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#include "worldgen.c"
const int ScreenHeight = 900;
const int ScreenWidth = 1600;

const int tileSize = 10;

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

void UpdatePlayer(Player_t *player)
{
    if (IsKeyDown(KEY_W))
    {
        player->position.y += -1;
    }
    if (IsKeyDown(KEY_S))
    {
        player->position.y += 1;
    }
    if (IsKeyDown(KEY_A))
    {
        player->position.x += -1;
    }
    if (IsKeyDown(KEY_D))
    {
        player->position.x += 1;
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
        UpdatePlayer(&player);
        camera.target = player.position;
        ClearBackground(BLACK);
        BeginDrawing();
        BeginMode2D(camera);
        PrintMap();
        DrawPlayer(&player);
        EndMode2D();
        EndDrawing();
    }

    // Avslutar programmet
    CloseWindow();
    return 0;
}