#include <SDL/SDL.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "../includes/display.h"

static char	*getFileName(char *name)
{
  unsigned int	i;
  int		len;
  char		*tmp;

  len = strlen(name);
  tmp = malloc(strlen(name) + 2);
  strcpy(tmp, name);
  for (i = 0; (access(tmp, F_OK) == 0); i++)
    {
      tmp[len - 4] = (char)i + '0';
      tmp[len - 3] = '.';
      tmp[len - 2] = 'b';
      tmp[len - 1] = 'm';
      tmp[len] = 'p';
      tmp[len + 1] = '\0';
    }
  return (tmp);
}

int		save(SDL_Surface *surface, char *filename)
{
  if (SDL_SaveBMP(surface, getFileName(filename)) == -1)
    return -1;
  return 0;
}
