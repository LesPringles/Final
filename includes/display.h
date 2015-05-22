#ifndef DISPLAY_H_
# define DISPLAY_H_

# include <SDL/SDL.h>

# define WINX	1280
# define WINY	720

typedef enum	e_current_action
  {
    PRINT_PIXEL_CARRE = 0,
    PRINT_PIXEL_ROND,
    PRINT_LIGNE,
    PRINT_SQUARE,
    PRINT_RECT,
    PRINT_CIRCLE,
    PRINT_DISC,
    PRINT_ELLIPSE,
    PRINT_GOMME,
    FILL_POT,
    PICK_COLOR
  }		t_current_action;

typedef enum	e_button_state
  {
    PRESSED = 0,
    RELEASED,
    UNSET
  }		t_button_state;

typedef struct		s_layers_list
{
  SDL_Surface		*layer;
  SDL_Rect		*pos;
  struct s_layers_list	*next;
  struct s_layers_list	*prev;
}			t_layers_list;

typedef struct		s_display
{
  SDL_Surface		*screen;
  t_layers_list		*layers;
  t_current_action	old_action;
  t_current_action	action;
  t_button_state	button;
  int			color_index;
  Uint32		current_color;
}			t_display;

typedef int		(*t_action_fptr)(t_display *, void *);

typedef struct		s_action
{
  t_current_action      current_action;
  t_action_fptr		action_fptr;
}			t_action;

int		refresh_display();
int		init_sdl();

int		display_layers(t_display *display);
int		add_layer(t_layers_list **list, SDL_Surface *layer, SDL_Rect *pos);
int		do_action(t_display *display, void *param);
void		setPixelVerif(t_display *display, int x, int y, Uint32 coul);
void		setPixel(t_display *display, int x, int y, Uint32 coul);
int		getColorFromSurface(t_display *display, void *param);
void		getColorFromPalette(t_display *display);

int		save(SDL_Surface *surface, const char *filename);
int		new(SDL_Surface *surface);
int		undo(t_display *display);
int		redo(t_display *display);
int	 	rot(t_display *display);

#endif /* !DISPLAY_H_ */
