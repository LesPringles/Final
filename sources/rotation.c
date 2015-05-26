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


void rotation_horizontale(t_display *display)
{
	

}

void rotation_verticale(t_display *display)
{
}




