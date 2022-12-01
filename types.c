#include "common.h"

typedef enum
{
    GameStateMainMenu,
    GameStateGame,
    GameStateMenu,
    GameStateDead,
    GameStateInnitLevel,
} Game_State_e;

typedef struct Player_t
{
    Vector2 position;
    int dmg;
    int hp;
} Player_t;
