#include "common.h"
#include "globals.c"

#include "worldgen.c"
#include "player.c"
#include "menu.c"

void LevelDone(Player_t *player)
{
    Rectangle exitRec = {.height = tileSize, .width = tileSize, .y = _end.y, .x = _end.x};

    Rectangle playerRec = {.x = player->position.x, .y = player->position.y, .height = tileSize, .width = tileSize};

    if (CheckCollisionRecs(playerRec, exitRec))
    {
        state = GameStateMenu;
    }
}

void NewWorld(Player_t *player)
{
    int status = CreateWord();
    // Sert till så det finns en värld
    while (status != 1)
    {
        printf("%d\n", status);
        status = CreateWord();
    }
    player->position = _start;
}

void InitTextures(void)
{
    for (int y = 0; y < 7; y++)
    {
        for (int x = 0; x < 7; x++)
        {
            tileset1[x + y * 7] = (Rectangle){.height = 16, .width = 16, .x = x * 16, .y = y * 16};
        }
    }
    for (int y = 0; y < 11; y++)
    {
        for (int x = 0; x < 7; x++)
        {
            tileset2[x + y * 11] = (Rectangle){.height = 16, .width = 16, .x = x * 16, .y = y * 16};
        }
    }

    //wallTileset[0b00000111] = 1;
    //wallTileset[0b00011000] = 10;
}

int main(void)
{
    Player_t player = {.dmg = 1, .hp = 100, .position = (Vector2){ScreenWidth / 2.0f, ScreenHeight / 2.0f}};

    Camera2D camera = {0};
    camera.offset = (Vector2){ScreenWidth / 2.0f, ScreenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    camera.target = player.position;
    NewWorld(&player);
    SetTargetFPS(60);
    InitWindow(ScreenWidth, ScreenHeight, "MineHero");

    tilemap1 = LoadTexture("./Tileset1.png");
    tilemap2 = LoadTexture("./Tileset2.png");

    SetTextureFilter(tilemap1, TEXTURE_FILTER_POINT);

    // Ritar ut kartan
    // Dev_PrintMap();
    while (!WindowShouldClose())
    {
        // Update
        clock_t updateStart = clock();

        Vector2 mousePos = mousePos = GetScreenToWorld2D((Vector2){GetMouseX(), GetMouseY()}, camera);
        switch (state)
        {
        case GameStateGame:
            camera.target = player.position;
            LevelDone(&player);
            UpdatePlayer(&player);
            break;
        case GameStateMenu:
            mousePos = GetScreenToWorld2D((Vector2){GetMouseX(), GetMouseY()}, camera);
            break;
        case GameStateInnitLevel:
            NewWorld(&player);
            state = GameStateGame;
            break;
        default:
            break;
        }
        clock_t updateEnd = clock();
        // Draw
        clock_t drawStart = clock();
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        switch (state)
        {
        case GameStateGame:
            DrawMap();
            DrawPlayer(&player);
            break;
        case GameStateMenu:
            camera.target = (Vector2){0, 0};
            UpdateAndDrawButtons(&player, &state, mousePos);
            break;
        default:
            break;
        }

        EndMode2D();
        // DrawText(TextFormat("Camera X: %.2f\tY: %.2f", camera.target.x, camera.target.y), 8, 8, 16, WHITE);
        // DrawText(TextFormat("Camera X: %.2f\tY: %.2f", camera.offset.x, camera.offset.y), 8, 32, 16, WHITE);
        clock_t drawEnd = clock();
        EndDrawing();

        // printf("Update time: %f\n", ((double)updateEnd - updateStart) / CLOCKS_PER_SEC);
        // printf("Draw time: %f\n", ((double)drawEnd - drawStart) / CLOCKS_PER_SEC);
    }

    // Avslutar programmet
    CloseWindow();
    return 0;
}