#include "display_functions.h"

using namespace std;
using namespace sf;

vector<RectangleShape*> createGrid()
{
	vector<RectangleShape*> grid;
	for (int i = 0; i < 8; i++)
	{
		unsigned int j((((i * 3) % 8) + 1) * 100);		// Creates lines in that order: 1, 4, 7, 2, 5, 8, 3, 6 (finishes with the darker lines).
		RectangleShape* row = new RectangleShape(Vector2f(900, 1));
		row->setPosition({ 0, (float)j });
		RectangleShape* col = new RectangleShape(Vector2f(1, 900));
		col->setPosition({ (float)j, 0 });

		Color color;
		if (!(j % 3))
			color = Color::Black;
		else
			color = Color(180, 180, 180);
		
		row->setFillColor(color);
		col->setFillColor(color);

		grid.push_back(row);
		grid.push_back(col);
	}
	return grid;
}

void updateWin(sf::RenderWindow& window, std::vector<sf::RectangleShape*>& grid, std::vector<std::vector<sf::Text*>>& nums)
{
	// Clear Window
	window.clear(Color::White);

	// Draw
	for (auto& const line : grid)
		window.draw(*line);
	for (int i(0); i < 9; i++)
	{
		for (auto& const textNum : nums[i])
		{
			if (textNum)
				window.draw(*textNum);
		}
	}

	// Display Window
	window.display();
}
