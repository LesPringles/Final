CC=gcc
CFLAGS= -W -Wall -Wextra -std=gnu99 $(GTK)
GTK= `pkg-config --cflags --libs gtk+-2.0`
SDL += -lSDL -lSDL_mixer -lSDL_gfx -lm

EXEC=apero

SRCS = ./sources/interface.c	\
		./sources/layers.c		\
	  ./sources/sdl_init.c		\
	  ./sources/print_pixel.c	\
	  ./sources/print_ligne.c	\
	  ./sources/print_square.c	\
	  ./sources/print_rect.c	\
	  ./sources/print_circle.c	\
	  ./sources/print_disc.c	\
	  ./sources/print_ellipse.c	\
	  ./sources/gomme.c			\
	  ./sources/do_action.c		\
	  ./sources/save.c			\
	  ./sources/new.c			\
	  ./sources/rotation.c 		\
	  ./sources/zoom.c 			\
	  ./sources/filtres.c		\
	  ./sources/convolution.c	\
	  ./sources/tracing.c   	\
	  ./sources/fill_pot.c

OBJS = $(SRCS:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(GTK) $(SDL)

clean:
	rm -f *.o
	rm -f $(OBJS)
	rm -f $(EXEC)

re: clean all
