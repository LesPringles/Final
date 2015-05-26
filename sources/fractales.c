#include <math.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"

void fdessinfractale(t_display *display, int zoom, int Xrepereinit, int Yrepereinit, double paramCx, double paramCy)
{
	SDL_Surface *pixel;
    pixel = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);
	int px=0, py=0;
	int borderonly = 0;

    px=0;
    while (px<WINX)
	{
    	py=0;
    	while (py<WINY)
		{
            position.x=px;
            position.y=py;

            Xinit=0;Yinit=0;
            Xinit=(int)(Xrepereinit-(WINX/2));
            Yinit=(int)(Yrepereinit-(WINY/2));
            Xrepere=4*((px+Xinit)/((double)(zoom*WINX)));
            Yrepere=4*((py+Yinit)/((double)(zoom*WINY)));
            Zx=Xrepere;
            Zy=Yrepere;

            if(paramCx!=0&&paramCy!=0){Cx=paramCx;Cy=paramCy;}
            else{Cx=Xrepere;Cy=Yrepere;};

            n=0;

            while ( n<255 && (Zx*Zx+Zy*Zy)<=4 ){copie=Zx;
                                              Zx=Zx*Zx-Zy*Zy+Cx;
                                              Zy=2*copie*Zy+Cy;
                                              n++;
                                             };
            if( n == 255 && borderonly == 1) 
			{
				SDL_FillRect(pixel, NULL, SDL_MapRGB(pixel->format, 255, 255 , 255));
			}
            else
			{
				SDL_FillRect(pixel, NULL, SDL_MapRGB(pixel->format, n, n , 0));
			}

            SDL_BlitSurface(pixel, NULL, display->screen, &position);
            py++;
    	}

    px++;
    }

SDL_Flip(display->screen);
}
