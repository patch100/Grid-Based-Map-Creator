#include "Surface.h"

Surface::Surface()
{

}

SDL_Surface* Surface::OnLoad(const char* File)
{
   //create surfaces to laod the image on
   SDL_Surface* temp = NULL;
   SDL_Surface* loaded_image = NULL;

   //try to load the image from file
   if ((temp = IMG_Load(File)) == NULL)
   {
      return NULL;
   }

   //convert the surface
   loaded_image = SDL_ConvertSurface(temp, temp->format, SDL_SWSURFACE);
   SDL_FreeSurface(temp);

   return loaded_image;
}

bool Surface::OnDraw(SDL_Surface* destination, SDL_Surface* source, int x, int y)
{
   //make sure the surfaces aren't null
   if (destination == NULL || source == NULL)
   {
      return false;
   }
   
   //specfy an area to update
   SDL_Rect position;
   position.x = x;
   position.y = y;

   //draw the surface onto the destination surface
   SDL_BlitSurface(source, NULL, destination, &position);
   return true;
}

bool Surface::OnDraw(SDL_Surface* destination, SDL_Surface* source, int x, int y, int x2, int y2, int width, int height)
{
   //make sure neither of the surfaces are null
   if (destination == NULL || source == NULL)
   {
      return false;
   }

   //specify an area to update
   SDL_Rect destination_position;
   destination_position.x = x;
   destination_position.y = y;

   //specify the area to copy
   SDL_Rect source_position;
   source_position.x = x2;
   source_position.y = y2;
   source_position.w = width;
   source_position.h = height;

   //draw the source surface to the destination surface
   SDL_BlitSurface(source, &source_position, destination, &destination_position);
   return true;
}