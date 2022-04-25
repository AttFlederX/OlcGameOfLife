#include "gameOfLife.h"

GameOfLife::GameOfLife()
{
	sAppName = "OLC Conway's Game of Life";
}

bool GameOfLife::OnUserCreate()
{
	nFieldWidth = ScreenWidth();
	nFieldHeight = ScreenHeight() - nFieldVerticalOffset;

	initializeField();

	Clear(olc::BLACK);
	drawField();

	fGenerationRefreshTime = GENERATION_LIFESPAN;

	return true;
}

bool GameOfLife::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::BLUE);

	if (isInGenerationMode)
	{
		if (GetKey(olc::Key::S).bPressed)
		{
			isInGenerationMode = false;
		}
		else
		{
			fRunningTime += fElapsedTime;

			if (fRunningTime > fGenerationRefreshTime)
			{
				refreshGeneration();
				fGenerationRefreshTime = fRunningTime + GENERATION_LIFESPAN;
			}
		}
	}
	else
	{
		if (GetKey(olc::Key::G).bPressed)
		{
			isInGenerationMode = true;
		}
		else
		{
			if (GetMouse(olc::Mouse::LEFT).bPressed)
			{
				olc::vi2d mousePos = GetMousePos();

				if (mousePos.y > nFieldVerticalOffset)
				{
					field[(mousePos.y / nCellSize) - nFieldVerticalOffset][mousePos.x / nCellSize] = 
						!field[(mousePos.y / nCellSize) - nFieldVerticalOffset][mousePos.x / nCellSize];
				}
			}
		}
	}

	drawField();
	drawInfo();

	return true;
}

void GameOfLife::initializeField()
{
	field = new bool*[nFieldHeight];
	for (int i = 0; i < nFieldHeight; i++)
	{
		field[i] = new bool[nFieldWidth];
		for (int j = 0; j < nFieldWidth; j++)
		{
			field[i][j] = false;
		}
	}
}

void GameOfLife::drawField()
{
	for (int i = 0; i < nFieldHeight; i++)
	{
		for (int j = 0; j < nFieldWidth; j++)
		{
			FillRect(j * nCellSize, (nFieldVerticalOffset + i) * nCellSize, nCellSize, nCellSize,
			         field[i][j] ? olc::GREEN : olc::BLACK);
		}
	}
}

void GameOfLife::drawInfo()
{
	if (isInGenerationMode)
	{
		DrawString(4, 4, "Generation #" + std::to_string(nGeneration) + " - press S to stop");
	}
	else
	{
		DrawString(4, 4, "Setup mode - press G to start");
	}
}

void GameOfLife::refreshGeneration()
{
	std::cout << "Generation #" << ++nGeneration << std::endl;

	bool** newField = new bool*[nFieldHeight];
	for (int i = 0; i < nFieldHeight; i++)
	{
		newField[i] = new bool[nFieldWidth];
		for (int j = 0; j < nFieldWidth; j++)
		{
			newField[i][j] = refreshCell(field, j, i);
		}
	}

	// cleanup
	for (int i = 0; i < nFieldHeight; i++)
	{
		delete field[i];
	}
	delete field;

	field = newField;
}

bool GameOfLife::refreshCell(bool** field, int x, int y)
{
	int nNeighbors = 0;
	if (x > 0)
	{
		if (y > 0)
		{
			nNeighbors += field[y - 1][x - 1];
		}

		nNeighbors += field[y][x - 1];

		if (y < nFieldHeight - 1)
		{
			nNeighbors += field[y + 1][x - 1];
		}
	}

	if (y > 0)
	{
		nNeighbors += field[y - 1][x];
	}
	if (y < nFieldHeight - 1)
	{
		nNeighbors += field[y + 1][x];
	}

	if (x < nFieldWidth - 1)
	{
		if (y > 0)
		{
			nNeighbors += field[y - 1][x + 1];
		}

		nNeighbors += field[y][x + 1];

		if (y < nFieldHeight - 1)
		{
			nNeighbors += field[y + 1][x + 1];
		}
	}

	// cell with 3 neighbors is always alive
	if (nNeighbors == 3)
	{
		return true;
	}
	// cell with < 2 or > 3 neighbors is never alive
	if (nNeighbors < 2 || nNeighbors > 3)
	{
		return false;
	}

	return field[y][x];
}
