#include "common.h"

const int tileSize = 10;

#include "worldgen.c"
#include "player.c"

const int ScreenHeight = 900;
const int ScreenWidth = 1600;

typedef enum
{
    GameStateMainMenu,
    GameStateGame,
    GameStateDead,
} Game_State_e;

Game_State_e state = GameStateGame;

void NewWorld(Player_t *player)
{
    player->position = _start;
}

void LevelDone(Player_t *player)
{
    Rectangle exitRec = {.height = tileSize, .width = tileSize, .y = _end.y, .x = _end.x};

    Rectangle playerRec = {.x = player->position.x, .y = player->position.y, .height = tileSize, .width = tileSize};

    if (CheckCollisionRecs(playerRec, exitRec))
    {
        state = GameStateDead;
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
        // Update
        clock_t updateStart = clock();
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
        clock_t updateEnd = clock();
        // Draw
        clock_t drawStart = clock();
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
        clock_t drawEnd = clock();
        EndDrawing();

        printf("Update time: %f\n", ((double)updateEnd - updateStart) / CLOCKS_PER_SEC);
        printf("Draw time: %f\n", ((double)drawEnd - drawStart) / CLOCKS_PER_SEC);
    }

    // Avslutar programmet
    CloseWindow();
    return 0;
}