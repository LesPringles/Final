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

SDL_Surface *retournementHorizontal(SDL_Surface *surface) {
	int y, i;
	Uint8 *pixel1, *pixel2;

	SDL_LockSurface(surface);

	// on prend chaque ligne une par une
	for (y = 0; y < surface->h; y++) {
		// pixel1 pointe vers le premier pixel de la ligne
		pixel1 = (Uint8*)surface->pixels + y * surface->pitch;
		// pixel2 pointe après le dernier pixel de la ligne
		pixel2 = pixel1 + surface->w * surface->format->BytesPerPixel;

		// on fait se rapprocher les pointeurs, et dès qu'ils se touchent (au milieu), c'est fini pour cette ilgne
		while (pixel1 < pixel2) {
			pixel2 -= surface->format->BytesPerPixel;

			// on échange les valeurs *pixel1 et *pixel2 avec une petite boucle
			for (i = 0; i < surface->format->BytesPerPixel; i++) {
				Uint8 tmp = *(pixel2 + i);

    			pixel2 += i;
				pixel1 += i;	
    			pixel2 = pixel1;	
    			pixel1 = &tmp;


			}

			pixel1 += surface->format->BytesPerPixel;
		}
	}

	SDL_UnlockSurface(surface);
	return surface;
}

void copierEtRetourner(SDL_Surface* s) {
	// la fonction SDL_ConvertSurface permet de dupliquer facilement une surface
	//SDL_Surface* nouv = SDL_ConvertSurface(s, s->format, s->flags);
	SDL_Surface *rot = NULL;
	
	if (s != NULL) 
		rot = retournementHorizontal(s);
	
	new(s);
	SDL_BlitSurface(rot, NULL, s, NULL); 
	SDL_FreeSurface(rot); 
	SDL_Flip(s);
}
