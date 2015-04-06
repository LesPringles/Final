#include "../includes/interface.h"

SDL_Surface* filtre_noir_blanc(t_display display)
{
	
		int x, y;
 
        Uint8 r_s, g_s, b_s, a_s;
 
        Uint32 pixel_s;
 
        SDL_LockSurface(display.screen);
 
        for (y = 0 ; y < display.screen->h; y ++)
        {
                for ( x = 0 ; x < display.screen->w; x ++)
                {
                        pixel_s = *((Uint32*)(display.screen->pixels) + x + y * WINX);
 
                        SDL_GetRGBA(pixel_s, display.screen->format, &r_s, &g_s, &b_s, &a_s);
 
                        Uint8 Moyenne = (r_s + g_s + b_s) / 3;
 
                        if(Moyenne < 128)
                        {
                            r_s = 0;
                            g_s = 0;
                            b_s = 0;
                        }
                        else if(Moyenne >= 128)
                        {
                            r_s = 255;
                            g_s = 255;
                            b_s = 255;
                        }
 
                        pixel_s = SDL_MapRGBA(display.screen->format, r_s, g_s, b_s, a_s);
 
                        setPixel(&display, x, y, pixel_s);
 
                }
        }

        SDL_UnlockSurface(display.screen);
				
				SDL_BlitSurface(display.screen, NULL, display.screen, NULL);

				SDL_Flip(display.screen);

		return display.screen;

}

SDL_Surface* filtre_gris(t_display display)
{
	int x, y;
 
    Uint8 r_s, g_s, b_s, a_s;
 
    Uint32 pixel_s;
 
    SDL_LockSurface(display.screen);

	
	for (y = 0 ; y < display.screen->h; y ++)
        {
                for ( x = 0 ; x < display.screen->w; x ++)
                {
					
					pixel_s = *((Uint32*)(display.screen->pixels) + x + y * WINX);
 
                    SDL_GetRGBA(pixel_s, display.screen->format, &r_s, &g_s, &b_s, &a_s);
 
                    Uint8 Moyenne = (r_s * 0.3 + g_s * 0.59 + b_s * 0.11);

                            r_s = Moyenne;
                            g_s = Moyenne;
                            b_s = Moyenne;
                    
                      pixel_s = SDL_MapRGBA(display.screen->format, r_s, g_s, b_s, a_s);
						setPixel(&display, x, y, pixel_s);

				}

		}


  	SDL_UnlockSurface(display.screen);

		SDL_BlitSurface(display.screen, NULL, display.screen, NULL);

		SDL_Flip(display.screen);

		return display.screen;
}

SDL_Surface* filtre_inverser(t_display display)
{
	int x, y;
 
    Uint8 r_s, g_s, b_s, a_s;
 
    Uint32 pixel_s;
 
    SDL_LockSurface(display.screen);

	
	for (y = 0 ; y < display.screen->h; y ++)
        {
                for ( x = 0 ; x < display.screen->w; x ++)
                {
					
					pixel_s = *((Uint32*)(display.screen->pixels) + x + y * WINX);
 
                    SDL_GetRGBA(pixel_s, display.screen->format, &r_s, &g_s, &b_s, &a_s);
 

                            r_s = 255 - r_s;
                            g_s = 255 - g_s;
                            b_s = 255 - b_s;
                    
                      pixel_s = SDL_MapRGBA(display.screen->format, r_s, g_s, b_s, a_s);
						setPixel(&display, x, y, pixel_s);

				}

		}


  	SDL_UnlockSurface(display.screen);

		SDL_BlitSurface(display.screen, NULL, display.screen, NULL);

		SDL_Flip(display.screen);

		return display.screen;
}
