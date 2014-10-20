#ifndef _MAP_PROPERTIES_
#define _MAP_PROPERTIES_

const int MAP_HEIGHT = 20;
const int MAP_WIDTH = 20;
const int TILE_SIZE = 32;
const int WINDOW_WIDTH = 720;
const int WINDOW_HEIGHT = 650;
const char MAP_LIST_NAMES[9] = "list.txt";
const char TEXTURE_FILE_NAME[10] = "img32.bmp";

enum GAME_STATES
{
   NOT_STARTED = 0,
   CREATE_MAP,
   SET_START,
   SET_END,
   VALIDATE_MAP,
   PLAY
};
#endif