int i, j, k;
itn y = 0;

for (i = 0; i <= 255; i++)
{
	for (j = 0; j <= 255; j++)
	{
		for (k = 0; k <= 255; k++)
		{
			while (y < WINY)
			{
			coul = SDL_MapRGBA(display->screen->format, i, j, k, 0);
			setPixelVerif(display, x, y, coul);
			y++;
			}
		}
	}
}
