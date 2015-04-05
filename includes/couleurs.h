#ifndef COULEURS_H_
# define COULEURS_H_

# include <SDL/SDL.h>

typedef enum	e_color_index
  {
    RED = 0,
    BLUE,
    GREEN,
    YELLOW,
    PINK,
    PURPLE,
    BLACK,
    WHITE,
    GREY,
    SILVER	//!\\ NEVER ADD COLORS AFTER SILVER
  }		t_color_index;

static const Uint32	colors[] =
  {
    //    A   R   G   B
    // 0x XX  XX  XX  XX

    0x00ee1111, // RED
    0x001111ee, // BLUE
    0x0011ee11, // GREEN
    0x00eeee11, // YELLOW
    0x00ff69b4, // PINK
    0x005e2d79, // PURPLE
    0x00000000, // BLACK
    0x00ffffff, // WHITE
    0x00444444, // GREY
    0x00aaaaaa  // SILVER
  };

# define NB_COLORS	SILVER

#endif /* !COULEURS_H_ */
