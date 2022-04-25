
#define OLC_PGE_APPLICATION
#include "gameOfLife.h"

int main()
{
	GameOfLife game;
	if (game.Construct(320, 240, 4, 4))
	{
		game.Start();
	}

	return 0;
}