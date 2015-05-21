#include <SDL/SDL.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"


Uint32 ColorPick(t_display *display, int x, int y)
{
	Uint8 r_s, g_s, b_s, a_s;
	Uint32 pixel_s;
	pixel_s = *((Uint32*)(display->screen->pixels) + x + y * WINX);
	SDL_GetRGBA(pixel_s, display->screen->format, &r_s, &g_s, &b_s, &a_s);
	pixel_s = SDL_MapRGBA(display->screen->format, r_s, g_s, b_s, a_s);

	return pixel_s;
}

void rec_pot(t_display *display, Uint32 pixel_s, int x, int y, int M[])
{
	M[x + y * WINX] = 0;
	Uint8 r_s, g_s, b_s, a_s;
	Uint8 r, g, b, a;
	Uint32 pixel;

SDL_GetRGBA(pixel_s, display->screen->format, &r_s, &g_s, &b_s, &a_s);
	
	pixel = *((Uint32*)(display->screen->pixels) + x + y * WINX);
	SDL_GetRGBA(pixel, display->screen->format, &r, &g, &b, &a);

	if((r_s == r)&&(g_s == g)&&(b_s == b))
	{
		setPixelVerif(display, x, y, colors[display->color_index]);
			if(y-1>=0){
			if(M[x + (y - 1) * WINX])
				rec_pot(display, pixel_s, x, y - 1, M);}
			if(y+1<WINY){	
			if(M[x + (y + 1) * WINX])
				rec_pot(display, pixel_s, x, y + 1, M);}
			if(x+1<WINX){
			if(M[x + 1 + y * WINX])
				rec_pot(display, pixel_s, x + 1, y, M);}
			if(x-1>=0){
			if(M[x - 1 + y * WINX])
				rec_pot(display, pixel_s, x - 1, y, M);}

	}


}

int fill_pot(t_display *display, void *param)
{
	SDL_MouseButtonEvent	*mouse;

	if(display->button == RELEASED)
		return (0);
	Uint32 pixel_s;
	int x, y, n;
	mouse = (SDL_MouseButtonEvent *)param;
	x = mouse->x;
	y = mouse->y;
	pixel_s = ColorPick(display, x, y);
	
	n = WINX * WINY;
	int M[n];
	for(int i = 0; i < n; i++)
	{
		M[i] = 1;
	}
	rec_pot(display, pixel_s, x, y, M);
	
	return SDL_Flip(display->screen);
}

