typedef enum TileType_e
{
    TILE_FLOOR,
    TILE_WALL,
    TILE_START,
    TILE_END,
    TILE_BEDROCK
} Tile_Type_e;

typedef struct Tile_t
{
    int neighbours;
    int hp;
    Tile_Type_e type;
} Tile_t;

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
