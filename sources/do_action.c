#include "../includes/display.h"
#include "../includes/shapes.h"

static const t_action	actions[] =
  {
    {PRINT_PIXEL_CARRE, &print_pixel_carre},
	{PRINT_PIXEL_ROND, &print_pixel_rond},
    {PRINT_LIGNE, &print_ligne},
    {PRINT_SQUARE, &print_square},
    {PRINT_RECT, &print_rect},
    {PRINT_CIRCLE, &print_circle},
    {PRINT_DISC, &print_disc},
	{PRINT_ELLIPSE, &print_ellipse},
  	{PRINT_GOMME, &print_gomme},
	{FILL_POT, &fill_pot}
  };

int		do_action(t_display *display, void *param)
{
  unsigned int	i;

  for (i = 0; i < (sizeof(actions)); i++)
    if (display->action == actions[i].current_action)
      return actions[i].action_fptr(display, param);
  return (-1);
}
