#pragma once
#include <vector>
#include "essentials.h"
#include "display_functions.h"
#include <ctime>

void solveSudoku(sf::RenderWindow& window, std::vector<sf::RectangleShape*>& grid, std::vector<std::vector<sf::Text*>>& nums)
{
	for (int x(0); x < 9; x++)
	{
		for (int y(0); y < 9; y++)
		{
			if ((*ptr_cells)[x][y][0] != 0)
			{
				(*ptr_cells)[x][y][1] = 0;
				(*ptr_nums)[x][y]->setFillColor(sf::Color::Black);
			}
		}
	}

	int x(0), y(0);
	int displayCount(0);
	double duration(0);
	bool isGoingBack = false;

	// Timer
	std::clock_t timerStop;
	std::clock_t timerStart(std::clock());

	while (true)
	{
		if ((*ptr_cells)[x][y][0] != 0 && (*ptr_cells)[x][y][1] == 0)
		{
			if (isGoingBack)
			{
				if (x == 0)
				{
					if (y == 0)
						std::cout << "End (no solution)\n";
					else
					{
						x = 8;
						y--;
					}
				}
				else
				{
					x--;
				}
			}
			else
			{
				x++;
				if (x == 9)
				{
					x = 0;
					y++;
					if (y == 9)
					{
						break;
					}
				}
			}
			continue;
		}

		isGoingBack = backTracking(x, y, isGoingBack);

		if (y == 9)
			break;

		//displayCount++;
		//if (displayCount == 1)
		//{
		//	updateWin(window, grid, nums);
		//	displayCount = 0;
		//}

	}
	// Stop timer
	timerStop = std::clock();
	duration = (timerStop - timerStart) / (double)CLOCKS_PER_SEC;
	durationsS.push_back(duration);

	double mean(0);
	for (auto& const time : durationsS)
		mean += time;
	mean /= durationsS.size();

	updateWin(window, grid, nums);
	std::cout << "The sudoku has been solved in " << duration << " seconds.\n";
	std::cout << "Mean from " << durationsS.size() << " answers: " << mean << std::endl;
}
