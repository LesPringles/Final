#include <SDL/SDL.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"

	Uint8 r_s, g_s, b_s, a_s;
	Uint8 r, g, b, a;
	Uint32 pixel;
	t_display *displayer;
	int M[WINX*WINY];

void ColorPick(int x, int y)
{
	pixel = *((Uint32*)(displayer->screen->pixels) + x + y * WINX);
	SDL_GetRGBA(pixel, displayer->screen->format, &r_s, &g_s, &b_s, &a_s);
	M[x + y * WINX] = 1;
}

void rec_pot(int x, int y)
{
	//M[x + y * WINX] = 0;
	pixel = *((Uint32*)(displayer->screen->pixels) + x + y * WINX);
	SDL_GetRGBA(pixel, displayer->screen->format, &r, &g, &b, &a);

	if(r_s == r)
	{
		setPixel(displayer, x, y, colors[displayer->color_index]);
		M[x + y * WINX] = 1;
		if(y-1>=0){
			if(!M[x + (y - 1) * WINX])
				rec_pot(x, y - 1);}
			if(y+1<WINY){	
			if(!M[x + (y + 1) * WINX])
				rec_pot(x, y + 1);}
			if(x+1<WINX){
			if(!M[x + 1 + y * WINX])
				rec_pot(x + 1, y);}
			if(x-1>=0){
			if(!M[x - 1 + y * WINX])
				rec_pot(x - 1, y);}

	}


}

int fill_pot(t_display *display, void *param)
{
	SDL_MouseButtonEvent	*mouse;
	displayer = display;
	if(display->button == RELEASED)
		return (0);
	int x, y, n;
	mouse = (SDL_MouseButtonEvent *)param;
	x = mouse->x;
	y = mouse->y;
	ColorPick(x, y);
	
	n = WINX * WINY;
	/*for(int i = 0; i < n; i++)
	{
		M[i] = 0;
	}
	rec_pot(x, y);*/
	int i = 0;
	while(i < n)
	{
	pixel = *((Uint32*)(displayer->screen->pixels) + i);
	SDL_GetRGBA(pixel, displayer->screen->format, &r, &g, &b, &a);

	if((r_s == r)&&(g_s == g)&&(b_s == b))
		*((Uint32*)(display->screen->pixels) + i) = colors[display->color_index];
	i++;
	}
	
	return SDL_Flip(display->screen);
}

