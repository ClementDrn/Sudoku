#pragma once
#include <vector>
#include "essentials.h"
#include "display_functions.h"
#include <ctime>

void createBoard(sf::RenderWindow& window, std::vector<sf::RectangleShape*>& grid, std::vector<std::vector<sf::Text*>>& nums)
{
	clearBoard();
	int x(0), y(0);
	int displayCount(0);
	double duration(0);

	// Timer
	std::clock_t timerStop;
	std::clock_t timerStart(std::clock());

	while (true)
	{
		backTracking(x, y);

		if (y == 9)
			break;

		displayCount++;
		if (displayCount == 1)
		{
			updateWin(window, grid, nums);
			displayCount = 0;
		}
	}

	// Stop timer
	timerStop = std::clock();
	duration = (timerStop - timerStart) / (double)CLOCKS_PER_SEC;
	durationsG.push_back(duration);

	double mean(0);
	for (auto& const time : durationsG)
		mean += time;
	mean /= durationsG.size();

	updateWin(window, grid, nums);
	std::cout << "A new board has been generated in " << duration << " seconds.\n";
	std::cout << "Mean from " << durationsG.size() << " generations: " << mean << std::endl;
}
/*
void setClues(unsigned int clueQuantity)
{
	// Randomly select clues
	std::vector<std::vector<unsigned short int>> clues(clueQuantity, { 9, 9 });
	unsigned short int count(0);
	while (count < clueQuantity)
	{
		unsigned short int x(dist9(rng) - 1), y(dist9(rng) - 1);	// Random in range (0, 8)
		bool alreadyExists(false);
		for (auto& const coos : clues)
		{
			if (coos[0] == x && coos[1] == y)
			{
				alreadyExists = true;
				break;
			}
		}
		if (!alreadyExists)
		{
			clues[count][0] = x;
			clues[count][1] = y;
			count++;
		}
	}

	// Remove other numbers
	for (int y(0); y < 9; y++)
	{
		for (int x(0); x < 9; x++)
		{
			// Check if it is a clue
			bool isClue(false);
			for (auto& const clue : clues)
			{
				if (clue[0] == x && clue[1] == y)
				{
					isClue = true;
					break;
				}
			}
			// ..if not..
			if (!isClue)
				setCell(x, y);	// Remove number from the board
		}
	}
	std::cout << clueQuantity << " clues are set\n";
}*/

void setClues(sf::RenderWindow& window, std::vector<sf::RectangleShape*>& grid, std::vector<std::vector<sf::Text*>>& nums, unsigned int minClueQuantity = 17)
{
	int clueQuantity(81);
	// Array of all positions
	unsigned positions[81][2];
	for (int i(0); i < 9; i++)
	{
		for (int j(0); j < 9; j++)
		{
			positions[i * 9 + j][0] = j;		// x position
			positions[i * 9 + j][1] = i;		// y position
			if ((*ptr_cells)[i][j].size() == 0)
				(*ptr_cells)[i][j].push_back(0);
			else
				(*ptr_cells)[i][j][1] = 0;
		}
	}

	// Shuffle positions
	std::shuffle(std::begin(positions), std::end(positions), rng);

	// Print
	for (int i(0); i < 9; i++)
	{
		for (int j(0); j < 9; j++)
		{
			std::cout << '(' << positions[i * 9 + j][0] << ", " << positions[i * 9 + j][1] << ") ";
		}
		std::cout << std::endl;
	}
	std::cout << "-------\n";
	for (int i(0); i < 9; i++)
	{
		for (int j(0); j < 9; j++)
		{
			std::cout << '(' << (*ptr_cells)[i][j][0] << ' ' << (*ptr_cells)[i][j][1] << ") , ";
		}
		std::cout << std::endl;
	}

	if (clueQuantity > minClueQuantity)
	{
		bool isUniqueSolution(false);
		for (int i(0); i < 81; i++)
		{
			std::cout << "Position " << i << "/80\n";
			int x(positions[i][0]), y(positions[i][1]);
			int removedNumber((*ptr_cells)[x][y][0]);
			setCell(x, y);
			(*ptr_cells)[x][y].push_back(0);
			for (int k(0); k < 9; k++)
			{
				for (int j(0); j < 9; j++)
				{
					if ((*ptr_cells)[k][j][0] != 0)
					{
						//(*ptr_cells)[k][j][1] = 0;	// ERROR
						(*ptr_nums)[k][j]->setFillColor(sf::Color::Black);
					}
					std::cout << '(' << (*ptr_cells)[k][j][0] << ' ' << (*ptr_cells)[k][j][1] << ") , ";
				}
				std::cout << std::endl;
			}

			bool isGoingBack(false);
			x = 0;
			y = 0;
			int solutionCounter(0);

			while (true)
			{
				// Look if the selected cell is a "clue" one
				if ((*ptr_cells)[x][y][0] != 0 && (*ptr_cells)[x][y][1] == 0)
				{
					if (isGoingBack)
					{
						if (x == 0)
						{
							if (y == 0)
							{
								std::cout << i << " End (no other solution)\n";
								break;
							}
							x = 8;
							y--;
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
							y++;
							if (y == 9)
							{
								std::cout << "+1 solution" << std::endl;
								y = 8;
								x = 8;
								if (++solutionCounter > 1)
									break;
								isGoingBack = true;
							}
							else
								x = 0;
						}
					}
					continue;
				}

				isGoingBack = backTracking(x, y, isGoingBack);

				if (y == 9)
				{
					std::cout << "+1 solution (2nd)" << std::endl;
					y = 8;
					x = 8;
					if (++solutionCounter > 1)
						break;
					isGoingBack = true;
				}

				// Every possibilities has been tested
				if (y == -1)
					break;

				//updateWin(window, grid, nums);

			}

			std::cout << solutionCounter << " solutions\n";
			if (solutionCounter != 1)
			{
				// Set back the number
				x = positions[i][0];
				y = positions[i][1];
				setCell(x, y, removedNumber);
				(*ptr_cells)[x][y].push_back(0);
			}
			else
			// A unique solution could be established
			{
				clueQuantity--;
				isUniqueSolution = true;
			}
			

			for (int k(0); k < i; k++)
			{
				x = positions[k][0];
				y = positions[k][1];
				if ((*ptr_cells)[x][y][1] != 0)
				{
					setCell(x, y);
					(*ptr_cells)[x][y].push_back(0);
				}
			}

			if (clueQuantity <= minClueQuantity)
				break;
		}
		if (!isUniqueSolution)
			std::cout << "A unique solution couldn't be established...\n";
	}


	/*
	// Randomly select clues
	std::vector<std::vector<unsigned short int>> clues(clueQuantity, { 9, 9 });
	unsigned short int count(0);
	while (count < clueQuantity)
	{
		unsigned short int x(dist9(rng) - 1), y(dist9(rng) - 1);	// Random in range (0, 8)
		bool alreadyExists(false);
		for (auto& const coos : clues)
		{
			if (coos[0] == x && coos[1] == y)
			{
				alreadyExists = true;
				break;
			}
		}
		if (!alreadyExists)
		{
			clues[count][0] = x;
			clues[count][1] = y;
			count++;
		}
	}

	// Remove other numbers
	for (int y(0); y < 9; y++)
	{
		for (int x(0); x < 9; x++)
		{
			// Check if it is a clue
			bool isClue(false);
			for (auto& const clue : clues)
			{
				if (clue[0] == x && clue[1] == y)
				{
					isClue = true;
					break;
				}
			}
			// ..if not..
			if (!isClue)
				setCell(x, y);	// Remove number from the board
		}
	}
	*/
	std::cout << clueQuantity << " clues are set\n";
}
