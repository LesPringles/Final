#include "../includes/display.h"
#include "../includes/shapes.h"

static const t_action	actions[] =
  {
    {PRINT_PIXEL, &print_pixel},
    {PRINT_LIGNE, &print_ligne},
    {PRINT_SQUARE, &print_square},
    {PRINT_RECT, &print_rect},
    {PRINT_CIRCLE, &print_circle},
    {PRINT_DISC, &print_disc}
  };

int		do_action(t_display *display, void *param)
{
  unsigned int	i;

  for (i = 0; i < (sizeof(actions)); i++)
    if (display->action == actions[i].current_action)
      return actions[i].action_fptr(display, param);
  return (-1);
}
