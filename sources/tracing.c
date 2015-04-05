#include "../includes/display.h"

void setPixel(t_display *display, int x, int y, Uint32 coul)
{
  *((Uint32*)(display->screen->pixels) + x + y * WINX) = coul;
}


void setPixelVerif(t_display *display, int x, int y, Uint32 coul)
{
  if (x >= 0 && x < WINX &&
      y >= 0 && y < WINY)
    setPixel(display, x, y, coul);
}
