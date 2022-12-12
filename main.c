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

    wallTileset[0b11111111] = GetTileMapRect(-1, -1, tilemap2Width);
    // Klara
    // Uppe Höger hörn
    wallTileset[0b01101000] = GetTileMapRect(3, 1, tilemap2Width);
    wallTileset[0b00010110] = GetTileMapRect(3, 1, tilemap2Width);
    wallTileset[0b10010110] = GetTileMapRect(3, 1, tilemap2Width);
    wallTileset[0b10010111] = GetTileMapRect(3, 1, tilemap2Width);
    wallTileset[0b10010110] = GetTileMapRect(3, 1, tilemap2Width);
    wallTileset[0b00010111] = GetTileMapRect(3, 1, tilemap2Width);

    // Uppe Vänster hörn
    wallTileset[0b00101011] = GetTileMapRect(0, 1, tilemap2Width);
    wallTileset[0b00001011] = GetTileMapRect(0, 1, tilemap2Width);
    wallTileset[0b00001111] = GetTileMapRect(0, 1, tilemap2Width);
    wallTileset[0b00101111] = GetTileMapRect(0, 1, tilemap2Width);

    // Nere Höger hörn
    wallTileset[0b11110100] = GetTileMapRect(3, 3, tilemap2Width);
    wallTileset[0b11010100] = GetTileMapRect(3, 3, tilemap2Width);
    wallTileset[0b11010000] = GetTileMapRect(3, 3, tilemap2Width);
    wallTileset[0b11110000] = GetTileMapRect(3, 3, tilemap2Width);

    // Nere Vänster hörn
    wallTileset[0b01101000] = GetTileMapRect(0, 2, tilemap2Width);
    wallTileset[0b11101000] = GetTileMapRect(0, 2, tilemap2Width);
    wallTileset[0b11101001] = GetTileMapRect(0, 2, tilemap2Width);
    wallTileset[0b01101001] = GetTileMapRect(0, 2, tilemap2Width);

    // Nere mitten
    wallTileset[0b11111000] = GetTileMapRect(1, 2, tilemap2Width);
    wallTileset[0b11111100] = GetTileMapRect(1, 2, tilemap2Width);
    wallTileset[0b11111001] = GetTileMapRect(1, 2, tilemap2Width);
    wallTileset[0b11111101] = GetTileMapRect(1, 2, tilemap2Width);

    // Uppe mitten
    wallTileset[0b00011111] = GetTileMapRect(1, 1, tilemap2Width);
    wallTileset[0b00111111] = GetTileMapRect(1, 1, tilemap2Width);
    wallTileset[0b10011111] = GetTileMapRect(1, 1, tilemap2Width);
    wallTileset[0b10111111] = GetTileMapRect(1, 1, tilemap2Width);

    // Mitten Höger
    wallTileset[0b11010110] = GetTileMapRect(3, 2, tilemap2Width);
    wallTileset[0b11110110] = GetTileMapRect(3, 2, tilemap2Width);
    wallTileset[0b11010111] = GetTileMapRect(3, 2, tilemap2Width);
    wallTileset[0b11110111] = GetTileMapRect(3, 2, tilemap2Width);

    // Nere Vänster prick
    wallTileset[0b11111011] = GetTileMapRect(2, 2, tilemap2Width);

    // Test

    int hej[3][3] = {
        {1, 1, 1},
        {1, 9, 1},
        {0, 1, 1},
    };
    // wallTileset[0b00011000] = 10;
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

    InitTextures();
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