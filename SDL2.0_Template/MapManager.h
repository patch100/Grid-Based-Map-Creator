#ifndef _MAP_MANAGER_
#define _MAP_MANAGER_

#include <vector>
#include "Map.h"
#include "MapProperties.h"
#include <iostream>
#include <fstream>

class MapManager
{
public:
   static MapManager Loader;  //static class for map manipulation
   std::vector<Map> MapList;  //list of saved maps
   Map* LoadedMap;            //currently loaded map

public:
   //Summary:
   //initialize the list of maps from file
   //Return: if any maps exist
   bool InitializeList();

   //Summary:
   //outputs a list of maps
   void OutputList();

   //Summary:
   //delete pointers
   void OnCleanUp();

   //Summary
   //create a new map with the given width and height
   //input:
   //int x: width of map
   //int y: height of map
   void NewMap(int x, int y);

   //Summary:
   //delete the given map
   //input:
   //Map* deleteMap: map to be deleted
   void DeleteMap(Map* deleteMap);

   //Summary:
   //write the specified map to file, and possible update the maplist file
   //input:
   //Map* newMap: pointer to the new map
   //bool updatelist: option to update list file
   void WriteMapToFile(Map* newMap, bool updatelist);

private:
   //Summary:
   //load the given map from file
   //input: 
   //char* File: name of file
   //int x: width of map
   //int y: height of map
   bool LoadMapFromFile(char* File, int x, int y);
};

#endif