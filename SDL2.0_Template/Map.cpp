#include "Map.h"

Map::Map()
{
   TileGraphic = NULL;
   CustomHeight = 0;
   CustomWidth = 0;
   File = NULL;
   StartTile = NULL;
   EndTile = NULL;
}

bool Map::OnLoad()
{
   //File must be set first
   if (File == NULL)
      return false;

   //reinitializes the vector
   TileSet.clear();

   //opens a handle to the specified file for reading
   FILE* FileHandle = fopen(File, "r");

   //ensure file exists/has been opened
   if (FileHandle == NULL) 
      return false;

   //if a custom height and width have been set then use those
   if (CustomHeight == 0 && CustomWidth == 0)
   {
      CustomHeight = MAP_HEIGHT;
      CustomWidth = MAP_WIDTH;
   }

   //loop through rows
   for (int row = 0; row < CustomHeight; row++)
   {
      //loop through columns
      for (int col = 0; col < CustomWidth; col++) 
      {
         MapTile temp;
         //scan one set of tile attributes
         fscanf(FileHandle, "%d:%d ", &temp.ID, &temp.Type);
         //store the row
         temp.Row = row;
         //add it to the list of tiles
         TileSet.push_back(temp);
      }
      //increment to the next row
      fscanf(FileHandle, "\n");
   }

   //close the file handle
   fclose(FileHandle);
   return true;
}

void Map::OnRender(SDL_Surface* destination, int x, int y)
{
   //if the graphic doesn't exist do not draw it
   if (TileGraphic == NULL) 
      return;

   int MapWidth = TileGraphic->w / TILE_SIZE;
   int MapHeight = TileGraphic->h / TILE_SIZE;

   int ID = 0;

   //if a custom height and width have been set then use those
   if (CustomHeight == 0 && CustomWidth == 0)
   {
      CustomHeight = MAP_HEIGHT;
      CustomWidth = MAP_WIDTH;
   }

   //loop through rows
   for (int row = 0; row < CustomHeight; row++)
   {
      ///loop through columns
      for (int col = 0; col < CustomWidth; col++)
      {
         if (TileSet[ID].Render)
         {
            //if the tile is blank don't draw it
            if (TileSet[ID].Type == NONE)
            {
               //increment to next tile in row
               ID++;
               continue;
            }

            //calculate the top left coordinate of the current tile
            int TileTopLeftX = x + (col * TILE_SIZE);
            int TileTopLeftY = y + (row * TILE_SIZE);

            //calculate the bottom right coordinate of the current tile
            int TileBottomRightX = (TileSet[ID].Type % MapWidth) * TILE_SIZE;
            int TileBottomRIghtY = (TileSet[ID].Type / MapWidth) * TILE_SIZE;

            //draw the tile to the appropriate part of the map
            Surface::OnDraw(destination, TileGraphic, TileTopLeftX, TileTopLeftY, TileBottomRightX, TileBottomRIghtY, TILE_SIZE, TILE_SIZE);

            //don't redraw tile unless changed
            TileSet[ID].Render = false;
         }
         //increment to next tile in the row
         ID++;
      }
   }
}

bool Map::SetCustomSize(int height, int width)
{
   //Makes sure the map size cannot be initiallized to a false value
   if ((height > 0 && height <= MAP_HEIGHT) && (width > 0 && width <= MAP_WIDTH))
   {
      CustomHeight = height;
      CustomWidth = width;
      return true;
   }
   return false;
}

void Map::SetMapFile(char mapfile[])
{
   int count = 0;
   //count length of filename
   while (mapfile[count] != NULL)
   {
      count++;
   }
   
   //allocate memory for the pointer and initialize it
   File = new char[count];
   File[count] = '\0';

   //copy the file name 
   for (int i = 0; i < count; i++)
   {
      File[i] = mapfile[i];
   }
}

int Map::GetMapHeight()
{
   //if a custom height and width have been set then use those
   if (CustomHeight == 0)
   {
     return MAP_HEIGHT;
   }
   return CustomHeight;
}

int Map::GetMapWidth()
{
   //if a custom height and width have been set then use those
   if (CustomWidth == 0)
   {
      CustomWidth = MAP_WIDTH;
   }
   return CustomWidth;
}

void Map::SetTileGraphic(SDL_Surface* e)
{
   TileGraphic = e;
}

bool Map::CallValidateMap()
{
   //create a vector to remember which tiles have been visited
   std::vector<int> visited;
   return ValidateMap(StartTile, visited);
}

bool Map::ValidateMap(MapTile* temp, std::vector<int> &visited)
{
   //if the end tile has been found
   if (temp->Type == TILE_TYPE::END)
      return true;

   //check if the tile is a path tile or the start tile
   if (temp->Type == TILE_TYPE::PATH || temp->Type == TILE_TYPE::START)
   {
      //initialize local variables
      int row = temp->Row;
      int id = temp->ID;
      int col = NULL;
      int width = GetMapWidth();
      int x = NULL;  
      int y = NULL;  

      //set this tile as visited
      visited.push_back(id);

      //used to validate the tiles around this tile
      bool top = false;
      bool bottom = false;
      bool left = false;
      bool right = false;

      //calculate the col
      col = id - (width * row);

      //calculate pixel coordinates of current tile
      x = col * TILE_SIZE;
      y = row * TILE_SIZE;

      //check if a tile exists on TOP of current tile
      if (VerifyBounds(x, (y - TILE_SIZE)))
      {
         //retreive its id
         int idTop = (((y - TILE_SIZE) / TILE_SIZE) * GetMapWidth()) + (x / TILE_SIZE);
         //check if it has been visited
         if (!CheckVisited(idTop, visited))
            top = ValidateMap(&TileSet[idTop], visited);
      }

      //check if a BOTTOM tile exists 
      if (VerifyBounds(x, (y + TILE_SIZE)))
      {
         //retreive its id
         int idBottom = (((y + TILE_SIZE) / TILE_SIZE) * GetMapWidth()) + (x / TILE_SIZE);
         //check if it has been visited
         if (!CheckVisited(idBottom, visited))
            bottom = ValidateMap(&TileSet[idBottom], visited);
      }

      //check if a tile exists to the LEFT
      if (VerifyBounds((x - TILE_SIZE), y))
      {
         //retreive its id
         int idLeft = ((y / TILE_SIZE) * GetMapWidth()) + ((x - TILE_SIZE) / TILE_SIZE);
         //check if it has been visited
         if (!CheckVisited(idLeft, visited))
            left = ValidateMap(&TileSet[idLeft], visited);
      }

      //check if a tile exists to the RIGHT
      if (VerifyBounds((x + TILE_SIZE), y))
      {
         //retreive its id
         int idRight = ((y / TILE_SIZE) * GetMapWidth()) + ((x + TILE_SIZE) / TILE_SIZE);
         //check if it has been visited
         if (!CheckVisited(idRight, visited))
            right = ValidateMap(&TileSet[idRight], visited);
      }

      //return if any of the branches found the end
      return (right || left || bottom || top);
   }

   //not a valide tile
   return false;
}

bool Map::CheckVisited(int id, std::vector<int> &visited)
{
   //loop through visited tiles
   for (int i = 0; i < (int)visited.size(); i++)
   {
      if (visited[i] == id)
         return true;
   }
   return false;
}

bool Map::VerifyBounds(int x, int y)
{
   //checks if the x pixel coordinates are within the map bounderies
   if (x < 0 || x >= (GetMapWidth() * TILE_SIZE))
   {
      return false;
   }

   //checks if the y pixel coordinates are within the map bounderies
   if (y < 0 || y >= (GetMapHeight() * TILE_SIZE))
   {
      return false;
   }

   return true;
}

void Map::ResetTiles()
{
   //initialize a blank grid
   for (int i = 0; i < (int)TileSet.size(); i++)
   {
      TileSet[i].Type = BLANK;
      TileSet[i].Render = true;
   }
}

void Map::FillInBlank()
{
   //populate buildable tiles
   for (int i = 0; i < (int)TileSet.size(); i++)
   {
      if (TileSet[i].Type == TILE_TYPE::BLANK)
      {
         TileSet[i].Type = TILE_TYPE::BUILDABLE;
         TileSet[i].Render = true;
      }
   }
}

void Map::OnClick(int x, int y, int &game_state)
{
   //check if the click was in bounds of map
   if (VerifyBounds(x, y))
   {
      //calculate row and column of click
      int row = y / TILE_SIZE;
      int col = x / TILE_SIZE;

      //translate row column into id
      int id = (row * GetMapWidth()) + col;
      TileSet[id].Render = true;

      //take action depending on state of game
      switch (game_state)
      {
         //set the start of the map
         case  GAME_STATES::SET_START:
            TileSet[id].Type = TILE_TYPE::START;
            game_state = GAME_STATES::CREATE_MAP;
            StartTile = &TileSet[id];
            break;
         //add a path tile to the map
         case GAME_STATES::CREATE_MAP:
            if (TileSet[id].Type == BLANK)
               TileSet[id].Type = PATH;
            break;
         //set the end of the map
         case GAME_STATES::SET_END:
            TileSet[id].Type = TILE_TYPE::END;
            game_state = GAME_STATES::VALIDATE_MAP;
            EndTile = &TileSet[id];
            break;
      }
   } 
}