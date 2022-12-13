#include "common.h"

const int tileSize = 48;

const int playerSize = 32;

const Color backgroundColor = {6, 13, 29, 255};

const int ScreenHeight = 900;
const int ScreenWidth = 1600;

const int tilemap1Width = 112;
const int tilemap2Width = 176;

Game_State_e state = GameStateGame;

const int _height = 90;
const int _width = 160;

// Chans att bli en vägg från början
const int wallChanse = 31;

// Hur många tiles som behöver vara väggar för att byta sig själv 3x3
const int maxAdjTiles = 4;

// Hur många tiles som behöver vara väggar för att byta sig själv 5x5
const int maxAdjTiles2 = 15;

// Hur många generationer av kartan ska genereras
const int generations = 5;

int grid[_height][_width];
int gridTemp[_height][_width];

Tile_t tileGrid[_height][_width];

Vector2 _start;
Vector2 _end;

Texture2D tilemap1;
Texture2D tilemap2;

Rectangle wallTileset[256];