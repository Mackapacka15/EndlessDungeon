#include "common.h"

const int tileSize = 48;

const int playerSize = 32;

const int ScreenHeight = 900;
const int ScreenWidth = 1600;

const int tilemap1Width = 112;
const int tilemap2Width = 176;

Game_State_e state = GameStateGame;

Texture2D tilemap1;
Texture2D tilemap2;

Rectangle wallTileset[256];