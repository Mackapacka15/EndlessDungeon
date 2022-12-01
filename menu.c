#include "common.h"

typedef enum
{
    GameStateMainMenu,
    GameStateGame,
    GameStateMenu,
    GameStateDead,
    GameStateInnitLevel,
} Game_State_e;

Game_State_e DrawButton(Vector2 position, int width, int height, char *text, int fontsize, Vector2 mousePos, Game_State_e stateToReturn)
{
    int x = position.x - (width / 2);
    int y = position.y - (height / 2);
    Rectangle button = {.x = x, .y = y, .width = width, .height = height};
    DrawRectangleRec(button, GREEN);
    // Auto Centrering
    DrawText(text, x + ((width - MeasureText(text, fontsize)) / 2), y + ((height - fontsize) / 2), fontsize, WHITE);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        // printf("MousePos: %f, %f \n", mousePos.x, mousePos.y);
        if (CheckCollisionPointRec(mousePos, button))
        {
            return stateToReturn;
        }
    }
    return GameStateMenu;
}