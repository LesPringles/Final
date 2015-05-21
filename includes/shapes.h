#ifndef SHAPES_H_
# define SHAPES_H_

# include "display.h"

int	print_pixel(t_display *display, void *param);
int	print_ligne(t_display *display, void *param);
int	print_square(t_display *display, void *param);
int	print_rect(t_display *display, void *param);
int print_circle(t_display *display, void *param);
int print_disc(t_display *display, void *param);
int print_ellipse(t_display *display, void *param);
int print_gomme(t_display *display, void *param);
int fill_pot(t_display *dispaly, void *param);


#endif /* !SHAPES_H_ */
