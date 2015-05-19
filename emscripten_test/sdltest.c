#include "SDL.h"
#include <stdio.h>

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

int main(int argc, char *argv[])
{
 SDL_Surface *screen;
 int quit = 0;
 SDL_Event event;
 int x, y;
 Uint32 yellow;

 // Initialize defaults, Video and Audio
 if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1))
 {
  printf("Could not initialize SDL: %s.\n", SDL_GetError());
  return -1;
 }

 screen = SDL_SetVideoMode(800, 600, 24, SDL_SWSURFACE | SDL_FULLSCREEN);
 if ( screen == NULL )
 {
  fprintf(stderr, "Couldn't set 800x600x24 video mode: %s\n", SDL_GetError());
  return -2;
 }

 // Map the color yellow to this display (R=0xff, G=0xFF, B=0x00)
 //yellow = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
 yellow = SDL_MapRGB(screen->format, 0xff, 0x00, 0x00);
 
 // Make the dot at the center of the screen
 x = screen->w / 2;
 y = screen->h / 2;

 while( !quit )
 {
  // Poll for events
  while( SDL_PollEvent( &event ) )
  {
   switch( event.type )
   {
    case SDL_KEYUP:
     if(event.key.keysym.sym == SDLK_ESCAPE)
      quit = 1;
      break;
     if(event.key.keysym.sym == SDLK_F1)
      SDL_WM_ToggleFullScreen(screen); // Only on X11
      break;
    case SDL_QUIT:
     quit = 1;
     break;
    default:
     break;
   }
  }

  // Lock the screen for direct access to the pixels
  if ( SDL_MUSTLOCK(screen) )
  {
   if ( SDL_LockSurface(screen) < 0 )
   {
    fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
    return -3;
   }
  }

  // Plot the Pixel
  putpixel(screen, x, y, yellow);

  // Unlock Surface if necessary
  if ( SDL_MUSTLOCK(screen) )
  {
   SDL_UnlockSurface(screen);
  }

  // Update just the part of the display that we've changed
  SDL_UpdateRect(screen, x, y, 1, 1);
 }

 SDL_Quit();
 
 return 0;
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
 int bpp = surface->format->BytesPerPixel;
 // Here p is the address to the pixel we want to set
 Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

 switch(bpp)
 {
  case 1:
   *p = pixel;
   break;
  case 2:
   *(Uint16 *)p = pixel;
   break;
  case 3:
   if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
   {
    p[0] = (pixel >> 16) & 0xff;
    p[1] = (pixel >> 8) & 0xff;
    p[2] = pixel & 0xff;
   } else {
    p[0] = pixel & 0xff;
    p[1] = (pixel >> 8) & 0xff;
    p[2] = (pixel >> 16) & 0xff;
   }
   break;
  case 4:
   *(Uint32 *)p = pixel;
   break;
 }
}
