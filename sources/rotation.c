#include "../includes/interface.h"

void rotate_surface(SDL_Surface *picture, double angle) 
{ 
	SDL_Surface *rotation = NULL;

	rotation = rotozoomSurface(picture, angle, 1.0, 0); 

	new(picture);
	SDL_BlitSurface(rotation, NULL, picture, NULL); 
	SDL_FreeSurface(rotation); 
	SDL_Flip(picture);
	//correction rotation noir???
}


void rotation_horizontale(t_display display)
{
		int x, y;

		Uint32 pixel_s, pixel;

		for (y = 0 ; y < 500; y ++)
        {
                for ( x = 0 ; x < WINX; x ++)
                {
                        pixel_s = *((Uint32*)(display.screen->pixels) + x + y * WINX);
												pixel 	=	*((Uint32*)(display.screen->pixels) + x + WINX * (WINY - 1 - y));
											
												setPixel(&display, x, WINY - 1 - y, pixel_s);	
												setPixel(&display, x, y, pixel);

                }
        }

	SDL_Flip(display.screen);
	SDL_Rect		pos_0;

    pos_0.x = 0;
    pos_0.y = 0;

	add_layer(&display.layers, display.screen, &pos_0);

}

void rotation_verticale(t_display display)
{
		int x, y;

    Uint32 pixel_s, pixel;

		for (y = 0 ; y < WINY; y ++)
        {
                for ( x = 0 ; x < WINX/2; x ++)
                {
                        pixel_s = *((Uint32*)(display.screen->pixels) + x + y * WINX);
												pixel 	=	*((Uint32*)(display.screen->pixels) + WINX - 1 - x + y * WINX);
											
												setPixel(&display, WINX - 1 - x, y, pixel_s);	
												setPixel(&display, x, y, pixel);

                }
        }

	SDL_Flip(display.screen);
	SDL_Rect		pos_0;

    pos_0.x = 0;
    pos_0.y = 0;

	add_layer(&display.layers, display.screen, &pos_0);


}




