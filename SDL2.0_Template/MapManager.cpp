#include "MapManager.h"

MapManager MapManager::Loader;

bool MapManager::LoadMapFromFile(char File[255], int x, int y)
{
   //create a temporary map
   Map tempMap;
   //make sure the user specified size is valid
   if (!tempMap.SetCustomSize(y, x))
   {
      return false;
   }

   //load the texture file onto the maps surface
   SDL_Surface* tempSurface = Surface::OnLoad(TEXTURE_FILE_NAME);
   tempMap.SetTileGraphic(tempSurface);

   //set the file the map is saved to
   tempMap.SetMapFile(File);

   //add the map to the list
   MapList.push_back(tempMap);
   return true;
}

bool MapManager::InitializeList()
{
   //opens a handle to the specified file for reading
   FILE* FileHandle = fopen(MAP_LIST_NAMES, "r");

   //ensure file exists/has been opened
   if (FileHandle == NULL)
   {
      return false;
   }

   //create variables to hold values from the file
   char MapFileName[255];
   int mapX = 0;
   int mapY = 0;

   //try to read maps from file
   do
   {
      mapX = 0;
      mapY = 0;
      fscanf(FileHandle, "%s", MapFileName);
      fscanf(FileHandle, "\n");
      fscanf(FileHandle, "%d:%d", &mapX, &mapY);
      fscanf(FileHandle, "\n");
   } 
   while (LoadMapFromFile(MapFileName, mapX, mapY));

   fclose(FileHandle);
   return true;
}

void MapManager::OutputList()
{
   //output the maplist
   for (size_t count = 0; count < MapList.size(); count++)
   {
      std::cout << count+1 << ". " << MapList[count].File << "\n";
   }
}

void MapManager::OnCleanUp()
{
   for (size_t count = 0; count < MapList.size(); count++)
   {
      MapList[count].OnCleanUp();
   }
}

void MapManager::WriteMapToFile(Map* newMap, bool updatelist)
{
   //open a file to write the map to
   std::ofstream map_file;
   map_file.open(newMap->File);

   //write the maptile's id and type
   for (int row = 0; row < (int)newMap->TileSet.size(); row++)
   {
      if (row == newMap->GetMapWidth() - 1)
         map_file << "\n";
      map_file << newMap->TileSet[row].ID << ":" << newMap->TileSet[row].Type << " ";
   }

   map_file.close();

   //update the list of maps if specified
   if (updatelist)
   {
      //open the maplist file for appending
      map_file.open(MAP_LIST_NAMES, std::ofstream::out | std::ofstream::app | std::ofstream::ate);
      map_file << newMap->File << "\n";
      map_file << newMap->GetMapWidth() << ":" << newMap->GetMapHeight() << "\n";
      map_file.close();
   }
}

void MapManager::NewMap(int x, int y)
{
   //create a new map
   Map newMap;

   //set the user specified size
   newMap.SetCustomSize(y, x);
   
   //local copy of the amount of maps
   size_t length = MapList.size();

   //used to hold the filename
   char filename[20];
   
   //create a new filename
   sprintf(filename, "map%d.txt", length+1);

   //set the file name
   newMap.SetMapFile(filename);
   int id = 0;

   //create and initialize the tiles
   for (int row = 0; row < y; row++)
   {
      for (int col = 0; col < x; col++)
      {
         MapTile tempTile;
         tempTile.ID = id;
         tempTile.Type = BLANK;
         newMap.TileSet.push_back(tempTile);
         id++;
      }
   }

   //load the texture file
   SDL_Surface* tempSurface = Surface::OnLoad(TEXTURE_FILE_NAME);
   newMap.SetTileGraphic(tempSurface);

   //add the map to the list
   MapList.push_back(newMap);

   //save the map
   MapManager::Loader.WriteMapToFile(&newMap, false);
}