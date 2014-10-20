#ifndef _GAME_ENGINE_
#define _GAME_ENGINE_
#include <SDL2/SDL.h>
#include "Surface.h"
#include "MapProperties.h"
#include "Map.h"
#include "MapManager.h"

class GameEngine
{
public:
   //Summary:
   //Initializes and starts the  Game Engine
   GameEngine();
   
   //Summary:
   //Starts the game and calls member functions
   //Return: int: exit code
   int PlayGame();

   //Summary:
   //Checks if the game is running
   //Return: bool: true if it is running, else false
   bool GameRunning();

   //Summary:
   //Initializes SDL
   bool onInit();

   //Summary:
   //Processes a SDL event
   void onEvent(SDL_Event* e);

   //Summary:
   //Performs all logic of the game
   void onLoop();

   //Summary:
   //Refreshes the drawing surface
   void onRender();

   //Summary:
   //Cleans up variables
   void onCleanUp();

   //Summary:
   //Prompts the player to create or select a map
   //Input:
   //int &height: user specified height of the map
   //int &width: user specified width of the map
   //Return: users choice
   size_t PromptPlayer(int &height, int &width);

private:
   bool Running;              //if the game is running
   SDL_Window* MainWindow;    //the main window
   SDL_Surface* Canvas;       //the surface of the window
   SDL_Surface* Temp;         //temporary surface for loading textures
   size_t option;             //users choice
   int GameState;             //current state of the game
};

#endif