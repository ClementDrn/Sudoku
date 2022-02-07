#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>

std::vector<std::vector<std::vector<unsigned short int>>>* ptr_cells(0);	// Pointer of cells
std::vector<std::vector<sf::Text*>>* ptr_nums(0);	// Pointer of nums
bool* ptr_updateWindow(0);
std::vector<double> durationsG;
std::vector<double> durationsS;

sf::Font Consolas;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist9(1, 9);

void essentialsInit(std::vector<std::vector<std::vector<unsigned short int>>>* cells, std::vector<std::vector<sf::Text*>>* nums, bool* updateWindow)
{
	ptr_cells = cells;
	ptr_nums = nums;
	ptr_updateWindow = updateWindow;
	if (!Consolas.loadFromFile("assets/RobotoMono-Regular.ttf"))
		std::cout << "Error when trying to load RobotoMono font.\n";
}

void essentialsDestructor()
{
	ptr_cells = 0;
	ptr_nums = 0;
}

void setCell(int short unsigned x, int short unsigned y, int short unsigned value = 0, bool reset = true)
{
	if (reset)
		(*ptr_cells)[x][y] = std::vector<unsigned short int>(1, value);
	else
		(*ptr_cells)[x][y][0] = value;
	sf::Text** numText = &(*ptr_nums)[x][y];
	bool isNumsNotNull(*numText);

	if (value == 0)		
	{
		if (isNumsNotNull)		// Let's remove the text
		{
			delete *numText;
			*numText = 0;
		}
	}
	else
	{
		if (isNumsNotNull)		// There is already a text
			(*numText)->setString(std::to_string(value));

		else	// We have to create a text
		{
			*numText = new sf::Text(std::to_string(value), Consolas, 100);
			sf::FloatRect bounds((*numText)->getLocalBounds());
			(*numText)->setOrigin({ bounds.left + bounds.width / 2, bounds.top + bounds.height / 2 });
			(*numText)->setPosition(sf::Vector2f(x * 100 + 50, y * 100 + 50));
		}

		sf::Color color;
		switch (value)
		{
		case 1:
			color = sf::Color(255, 0, 0); break;
		case 2:
			color = sf::Color(255, 170, 0); break;
		case 3:
			color = sf::Color(170, 255, 0); break;
		case 4:
			color = sf::Color(0, 255, 0); break;
		case 5:
			color = sf::Color(0, 255, 170); break;
		case 6:
			color = sf::Color(0, 170, 255); break;
		case 7:
			color = sf::Color(0, 0, 255); break;
		case 8:
			color = sf::Color(170, 0, 255); break;
		case 9:
			color = sf::Color(255, 0, 170); break;
		default:
			color = sf::Color::Black; break;
		}		
		(*numText)->setFillColor(color);
	}
	*ptr_updateWindow = true;
}

void clearBoard()
{
	for (int x(0); x < 9; x++)
	{
		for (int y(0); y < 9; y++)
			setCell(x, y);
	}
	(*ptr_updateWindow) = true;
}

bool backTracking(int &x, int &y, bool isGoingBack = 0)
{
	int solutionNumber(0);
	int short unsigned value(0);
	bool isConflicts(false);

	// Change the value of the cell
	// Random number
	if ((*ptr_cells)[x][y][0] == 0)
	{
		value = dist9(rng);
		setCell(x, y, value);	// Set cell
		(*ptr_cells)[x][y].push_back(value);	// Set a 2nd value to the vector to remember what was the first number of the cell
	}
	// Increment number
	else
	{
		value = (*ptr_cells)[x][y][0] + 1;
		if (value == 10)
			value = 1;

		if (value == (*ptr_cells)[x][y][1])
		{
			isConflicts = true;
			isGoingBack = true;
			setCell(x, y);
			(*ptr_cells)[x][y].push_back(0);
			if (x == 0)
			{
				if (y == -1)
					std::cout << x << ' ' << y << " End (no solution)\n";
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
			setCell(x, y, value, false);
			isGoingBack = false;
		}

	}

	// Look for conflicts
	// Row
	if (!isConflicts)
	{
		for (int i(0); i < 9; i++)
		{
			if (i == x)
				continue;
			if ((*ptr_cells)[i][y][0] == value)
			{
				isConflicts = true;
				break;
			}
		}
	}

	// Column
	if (!isConflicts)
	{
		for (int i(0); i < 9; i++)
		{
			if (i == y)
				continue;
			if ((*ptr_cells)[x][i][0] == value)
			{
				isConflicts = true;
				break;
			}
		}
	}

	// Block
	if (!isConflicts)
	{
		int xBlock(((int)(x / 3)) * 3), yBlock(((int)(y / 3)) * 3);
		for (int i(0); i < 3; i++)
		{
			for (int j(0); j < 3; j++)
			{
				if (i + xBlock == x && j + yBlock == y)
					continue;
				if ((*ptr_cells)[i + xBlock][j + yBlock][0] == value)
				{
					isConflicts = true;
					break;
				}
			}
		}
	}

	// Conclusion
	if (!isConflicts)
	{
		x++;
		if (x == 9)
		{
			x = 0;
			y++;
		}
	}

	return isGoingBack;
}