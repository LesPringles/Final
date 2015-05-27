#include <math.h>

#include "../includes/display.h"
#include "../includes/couleurs.h"

#define MAX(a, b) (((a)>(b))?(a):(b))
#define ABS(a) (((a)<0)?-(a):(a))
 
#define SQRT_3  1.732

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


/*void dot(t_display *display, int x, int y, Uint32 color)
{
    SDL_Rect r = {x, y, 1, 1};
    SDL_FillRect(display->screen, &r, color);
}
 
void drawLine(t_display *display, int x1, int y1, int x2, int y2, Uint32 color)
{
    double x = x1, y = y1;
    double inc_x = x2 - x1, inc_y = y2 - y1;
    int m = MAX(ABS(inc_x), ABS(inc_y));
    inc_x /= m;
    inc_y /= m;
 
    for(; m >= 0; m--)
    {
        dot(display, (unsigned int)x, (unsigned int)y, color);
        x += inc_x;
        y += inc_y;
    }
}*/
 
#define LEVEL 5
 
void fractale(t_display *display, int x1, int y1, int x2, int y2, int level, int col)
{
    if(level > 0)
    {
        int x3 = (2*x1 + x2)/3;
        int y3 = (2*y1 + y2)/3;
        int x5 = (x1 + 2*x2)/3;
        int y5 = (y1 + 2*y2)/3;
        int x4 = x3 + (x5 - x3)/2 + (y5 - y3)*SQRT_3/2;
        int y4 = y3 - (x5 - x3)*SQRT_3/2 + (y5 - y3)/2;
        fractale(display, x1, y1, x3, y3, level-1, col);
        fractale(display, x3, y3, x4, y4, level-1, col+1);
        fractale(display, x4, y4, x5, y5, level-1, col+1);
        fractale(display, x5, y5, x2, y2, level-1, col);
    }
    else
    {
        drawLine(display, x1, y1, x2, y2, SDL_MapRGB(display->screen->format,
            255*col/LEVEL, 255*level/LEVEL, 255 - 255*col/LEVEL));
    }
}
