#include "GameEngine.h"
#include <Windows.h>

GameEngine::GameEngine()
{
   Running = true;
   MainWindow = NULL;
   Canvas = NULL;
   Temp = NULL;
   option = 0;
   GameState = NOT_STARTED;
}

int GameEngine::PlayGame()
{
   //initialize sdl
   if (onInit() == false)
   {
      return -1;
   }

   //checks if there are any maps to load from file
   if (MapManager::Loader.InitializeList() == false)
   {
      //opens a handle to the specified file for reading
      FILE* FileHandle = fopen(MAP_LIST_NAMES, "w");
      fclose(FileHandle);
   }

   //if creating a new map, player defines height and width
   int height, width;

   //prompt player for map choice
   option = PromptPlayer(height, width);

   //hide the console
   FreeConsole();

   //catches all input events
   SDL_Event e;

   //create or play a map
   if (option > MapManager::Loader.MapList.size())
   {
      // new map
      MapManager::Loader.NewMap(width, height);
      option = MapManager::Loader.MapList.size();
      GameState = SET_START;
   }
   else
   {
      GameState = PLAY;
   }

   //load the map
   MapManager::Loader.MapList[option - 1].OnLoad();

   //loop through game states
   while (Running)
   {
      while (SDL_PollEvent(&e))
      {
         onEvent(&e);
      }

      onLoop();
      onRender();
   }

   onCleanUp();

   return 0;
}

bool GameEngine::onInit()
{
   //Initialize SDL
   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      return false;
   }
   else
   {
      //Create window
      MainWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
      if (MainWindow == NULL)
      {
         return false;
      }
      else
      {
         //Get window surface
         Canvas = SDL_GetWindowSurface(MainWindow);
      }
   }
   return true;
}

void GameEngine::onEvent(SDL_Event* e)
{
   //take action depending on event
   switch (e->type)
   {
   case SDL_KEYDOWN:
      if (e->key.keysym.sym == SDLK_RETURN)
      {
         //lets the user define an end tile
         if (GameState == GAME_STATES::CREATE_MAP)
            GameState = GAME_STATES::SET_END;
      }
      else if (GameState == GAME_STATES::PLAY)
      {
         //quit the game
         Running = false;
      }
      break;
   case SDL_MOUSEBUTTONDOWN:
         //If the left mouse button was pressed
         if (e->button.button == SDL_BUTTON_LEFT)
         {
            //Get the mouse offsets
            int x = e->button.x;
            int y = e->button.y;
            MapManager::Loader.MapList[option - 1].OnClick(x, y, GameState);
         }
         break;
   }
}

void GameEngine::onLoop()
{
   //check if the map needs to be validated
   if (GameState == GAME_STATES::VALIDATE_MAP)
   {
      bool validMap = MapManager::Loader.MapList[option - 1].CallValidateMap();

      //if the map is valid save it and play, else reset it
      if (validMap)
      {
         GameState = GAME_STATES::PLAY;
         MapManager::Loader.MapList[option - 1].FillInBlank();
         MapManager::Loader.WriteMapToFile(&MapManager::Loader.MapList[option - 1], true);
      }
      else
      {
         GameState = GAME_STATES::SET_START;
         MapManager::Loader.MapList[option - 1].ResetTiles();
      }     
   }
}

void GameEngine::onRender()
{
   //renders changes
   MapManager::Loader.MapList[option-1].OnRender(Canvas, 0, 0);
   //Update the surface
   SDL_UpdateWindowSurface(MainWindow);
   
}

void GameEngine::onCleanUp()
{
   SDL_DestroyWindow(MainWindow);
   SDL_FreeSurface(Canvas);
   SDL_FreeSurface(Temp);
   SDL_QUIT;
}

size_t GameEngine::PromptPlayer(int &height, int &width)
{
   std::cout << "Please select a map to play:\n";
   MapManager::Loader.OutputList();
   std::cout << MapManager::Loader.MapList.size() + 1 << ". new map\n";
   size_t option = 0;
   do
   {
      std::cout << "Please select a valid map: ";
      std::cin >> option;
      if (std::cin.fail() || ((option < 0) && (option >= MapManager::Loader.MapList.size()+1)))
      {
         option = 0;
      }

      if (option == MapManager::Loader.MapList.size() + 1)
      {
         std::cout << "Please input a valid map width (between 5 and 20): ";
         std::cin >> width;
         std::cout << "Please input a valid map height (between 5 and 20): ";
         std::cin >> height;

         if (std::cin.fail() || ((width < 5) || (width > 20)) || ((height < 5) || (height > 20)))
         {
            width = 0;
            height = 0;
         }
      }
   } 
   while (option == 0 || width == 0 || height == 0);
   return option;
}