#ifndef _MAP_TILE_
#define _MAP_TILE_

#include "MapProperties.h"

//tiles can either have a tower built on them, or are paths for critters
enum TILE_TYPE
{
   BUILDABLE = 0,
   PATH,
   BLANK,
   START,
   END,
   NONE
};

class MapTile 
{
public:
   int ID;        //unique id that identifies a tile in a row (not unique if not used in rows)
   int Type;      //tyle type based off of the enum (0 Path, 1 Buildable)
   bool Render;   //if the tile should be rendered
   int Row;       //used to calculate pixels

public:
   //Summary:
   //Initializes a tile with no id or type
   MapTile();
};

#endif