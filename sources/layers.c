#include <stdlib.h>
#include <SDL/SDL.h>

#include "../includes/display.h"

int		undo(t_display *display)
{
  if (!display->layers)
    return (0);
  if (!display->layers->next)
    return (0);
  display->layers = display->layers->next;
  if (SDL_BlitSurface(display->layers->layer,
		      NULL,
		      display->screen,
		      display->layers->pos) == -1)
    return (-1);
  return SDL_Flip(display->screen);
}

int		redo(t_display *display)
{
  if (!display->layers)
    return (0);
  if (!display->layers->prev)
    return (0);
  display->layers = display->layers->prev;
  if (SDL_BlitSurface(display->layers->layer,
		      NULL,
		      display->screen,
		      display->layers->pos) == -1)
    return (-1);
  return SDL_Flip(display->screen);
}

int		add_layer(t_layers_list **list, SDL_Surface *layer, SDL_Rect *pos)
{
  t_layers_list	*new_layer;

  if (*list)
    (*list)->prev = NULL;
  if ((new_layer = malloc(sizeof(t_layers_list))) == NULL)
    return -1;
  if ((new_layer->layer = SDL_CreateRGBSurface(0, WINX, WINY, 32, 0, 0, 0, 0)) == NULL)
    return -1;
  if (SDL_BlitSurface(layer, NULL, new_layer->layer, pos) == -1)
    return -1;
  if ((new_layer->pos = malloc(sizeof(SDL_Rect))) == NULL)
    return -1;
  memcpy(new_layer->pos, pos, sizeof(SDL_Rect));
  new_layer->next = *list;
  new_layer->prev = NULL;
  if ((*list) != NULL)
    (*list)->prev = new_layer;
  *list = new_layer;
  return 0;
}
