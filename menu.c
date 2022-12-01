#include "common.h"

int CreateButton(Vector2 position, int width, int height, char *text, int fontsize, Vector2 mousePos)
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
            return 1;
        }
    }
    return 0;
}

void UpdateAndDrawButtons(Player_t *player, Game_State_e *state, Vector2 mousePos)
{
    if (CreateButton((Vector2){0, 0}, 75, 20, "Next Level", 10, mousePos))
    {
        *state = GameStateInnitLevel;
    }

    if (CreateButton((Vector2){100, 0}, 75, 20, "Upgrade DMG", 10, mousePos) == 1)
    {
        player->dmg += 1;
        *state = GameStateInnitLevel;
    }
    if (CreateButton((Vector2){-100, 0}, 75, 20, "Upgrade HP", 10, mousePos) == 1)
    {
        player->hp += 25;
        *state = GameStateInnitLevel;
    }
}
