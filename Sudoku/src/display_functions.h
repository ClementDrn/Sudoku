#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

std::vector<sf::RectangleShape*> createGrid();

void updateWin(sf::RenderWindow& window, std::vector<sf::RectangleShape*>& grid, std::vector<std::vector<sf::Text*>>& nums);