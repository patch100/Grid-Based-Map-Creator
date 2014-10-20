#ifndef _SURFACE_
#define _SURFACE_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Surface 
{
public:
   //Summary:
   //Constructor
   Surface();

   //Summary:
   //Loads an image onto a new surface
   //Input:
   //char[]* File: name of the bitmap file
   //Return: SDL_Surface*: a new surface with the bitmap image 
   static SDL_Surface* OnLoad(const char* File);
   
   //Summary:
   //Using the given coordinates, draws the source surface onto the destination surface
   //Input:
   //SDL_Surface* destination: surface to be drawn to
   //SDL_Surface* source: surface that will be drawn
   //int x: x coordinate
   //int y: y coordinate
   //Return: bool: if the function worked or not
   static bool OnDraw(SDL_Surface* destination, SDL_Surface* source, int x, int y);
   
   //Summary:
   //Using the given dimensions and coordinates, draws the source surface onto the destination surface
   //Used when only a section of the surface needs to be drawn
   //Input:
   //SDL_Surface* destination: surface to be drawn to
   //SDL_Surface* source: surface that will be drawn
   //int x: x coordinate
   //int y: y coordinate
   //int x2: section x coordinate
   //int y2 section y coordinate
   //int width: the width of the section
   //int height: the height of the section
   //Return: bool: if the function worked or not
   static bool OnDraw(SDL_Surface* destination, SDL_Surface* source, int x, int y, int x2, int y2, int width, int height);
};

#endif