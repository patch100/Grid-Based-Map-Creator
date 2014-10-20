#ifndef _MAP_
#define _MAP_

#include "MapProperties.h"

#include <SDL2/SDL.h>
#include <vector>
#include "MapTile.h"
#include "Surface.h"

class Map 
{
public:
   SDL_Surface* TileGraphic;     //texture file for the map
   char* File;                   //file the map is saved to
   std::vector<MapTile> TileSet; //list of tiles in the map
private:
   int CustomWidth;              //user specified width
   int CustomHeight;             //user specified height
   MapTile* StartTile;           //start of the map
   MapTile* EndTile;             //end of the map

public:
   //Summary:
   //Initializes the map with blank surface
   Map();

   //Summary:
   //Loads a map into the vector TileSet from a text file
   //Input:
   //char[]* File: name of the text file
   //Return: true if it loaded correctly, else false
   bool OnLoad();

   //Summary:
   //Using the given coordinates, draws the source surface onto the destination surface
   //Input:
   //SDL_Surface* destination: surface to be drawn to
   //int x: x coordinate
   //int y: y coordinate
   void OnRender(SDL_Surface* destination, int x, int y);

   //Summary:
   //Cleans up the Maps pointers and surfaces
   void OnCleanUp()
   {
      if (TileGraphic != NULL)
         SDL_FreeSurface(TileGraphic);
      TileSet.clear();
   }

   //Summary:
   //Sets the size of the map to given height and width
   //Input:
   //int height: new height of map
   //int width: new width of map
   //Return: if the given sizes were valid
   bool SetCustomSize(int height, int width);

   //Summary:
   //Sets the file that the map uses to initialize itself
   void SetMapFile(char* mapFile);

   //Summary:
   //Logic when a tile is clicked
   //Input:
   //int x: x pixel coordinate
   //int y: y pixel coordinate
   //int game_state: state of the game
   void OnClick(int x, int y, int &game_state);

   //Summary:
   //Sets the texture for the map
   //Input:
   //SDL_Surface* e: pointer to the surface
   void SetTileGraphic(SDL_Surface* e);

   //Summary:
   //calls the recursive method to validate the user made map
   //Return: if the map is valid
   bool CallValidateMap();

   //Summary:
   //Linear recursive method for checking tile paths
   //Input:
   //MapTile* temp: tile that is being validated
   //std::vector<int> &visited: list of tiles that have been validated
   //Return: if the path was valid
   bool ValidateMap(MapTile* temp, std::vector<int> &visited);

   //Summary:
   //retreives the maps height
   //Return: height of the map
   int GetMapHeight();

   //Summary:
   //retreives the maps width
   //Return: width of the map
   int GetMapWidth();

   //Summary:
   //verifies the given coordinates are within the map
   //Input:
   //int x: x pixel coordinate
   //int y: y pixel coordinate
   //Return: if the coordinates are within bounds
   bool VerifyBounds(int x, int y);

   //Summary:
   //Sets all the tiles to blank 
   void ResetTiles();

   //Summary:
   //Checks if the given tile has already been visited
   //Input:
   //int id: the id of the given tile
   //std::vector<int> &visited: list of visited tiles
   //Return: if the tile has been visited
   bool CheckVisited(int id, std::vector<int> &visited);

   //Summary:
   //Sets remaining blank tiles to BUILDABLE
   void FillInBlank();
};

#endif
