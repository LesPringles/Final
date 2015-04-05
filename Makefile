CC=gcc
CFLAGS= -W -Wall -Wextra -std=gnu99
GTK= `pkg-config --cflags --libs gtk+-2.0`
SDL += -lSDL -lSDL_mixer -lm

EXEC=apero

SRCS = ./sources/layers.c		\
	  ./sources/sdl_init.c		\
	  ./sources/print_pixel.c	\
	  ./sources/print_ligne.c	\
	  ./sources/print_square.c	\
	  ./sources/print_rect.c	\
	  ./sources/print_circle.c	\
	  ./sources/print_disc.c	\
	  ./sources/do_action.c		\
	  ./sources/save.c		\
	  ./sources/new.c		\
	  ./sources/tracing.c

OBJS = $(SRCS:.c=.o)

all: $(EXEC)

$(EXEC): interface.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(GTK) $(SDL)


interface.o: interface.c
	$(CC) $(CFLAGS) -o $@ -c $^ $(GTK)

clean:
	rm -f *.o
	rm -f $(OBJS)
	rm -f $(EXEC)

re: clean all
