#include "Sudoku.hpp"

#include <chrono>
#include <iostream>

#include <SEL/Random.hpp>
#include <SEL/Vec2.hpp>


namespace Sudoku {

	static int internalSolve(std::array<std::array<uint8_t, 9>, 9>& board, uint8_t firstValues[9][9], bool useRandomness, uint32_t solutionCount = 1)
	{
		// A typical use is to ask for 1 solution only. If there is at least one, the board will be solved.
		// 
		// If multiple solutions are asked, but the asked number is to high, the function will return with the actual solution 
		// count but the board will not be solved as the algotrithm would not have finished for the last solution search.
		// This is useful to know the number of solutions.
		//
		// Without randomness, numbers are tested from 1 to 9. Randomness should always be used if the function is used to generate
		// a sudoku, otherwise the board output will always be the same.

		int x = 0;
		int y = 0;
		bool isSuccessful;
		uint32_t actualSolutionCount = 0;
		bool isGoingBack = false;
		bool isKnownNumber;
		uint8_t value;
		bool doesConflict;

		while (actualSolutionCount < solutionCount)
		{
			isSuccessful = false;

			while (!isSuccessful)
			{
				doesConflict = false;

				// Changes value of cell
				// Choose number if not set yet
				if (board[x][y] == 0)
				{
					if (useRandomness)
						value = sel::Random::uinteger() % 9 + 1;		// Number between 1 and 9
					else
						value = 1;

					board[x][y] = value;	// Sets cell
					firstValues[x][y] = value;	// firstValues remembers what was the first set number of the cell.
					isKnownNumber = false;
				}
				// Increments number otherwise
				else
				{
					// Changes only unknown numbers
					isKnownNumber = (firstValues[x][y] == 0);
					if (!isKnownNumber)
					{
						value = board[x][y] + 1;
						if (value == 10)
							value = 1;

						// If the cell has already tested every possible value, we go back.
						isGoingBack = (value == firstValues[x][y]);
					}

					if (isGoingBack)
					{
						doesConflict = true;
						if (!isKnownNumber)
						{
							firstValues[x][y] = 0;
							board[x][y] = 0;
						}

						if (x == 0)
						{
							// If we come back at the beginning, it means there is no solution.
							if (y == 0)
								break;

							x = 8;
							--y;
						}
						else
						{
							--x;
						}
					}
					// Sets the new value to test.
					else if (!isKnownNumber)
					{
						board[x][y] = value;
					}

				}

				if (!isKnownNumber)
				{
					// Look for conflicts
					// Row
					if (!doesConflict)
					{
						for (uint8_t i = 0; i < 9; i++)
						{
							if (i == x)
								continue;
							if (board[i][y] == value)
							{
								doesConflict = true;
								break;
							}
						}
					}

					// Column
					if (!doesConflict)
					{
						for (uint8_t i = 0; i < 9; i++)
						{
							if (i == y)
								continue;
							if (board[x][i] == value)
							{
								doesConflict = true;
								break;
							}
						}
					}

					// Block
					if (!doesConflict)
					{
						uint8_t xBlock = x / 3 * 3;
						uint8_t yBlock = y / 3 * 3;
						for (uint8_t i = 0; i < 3; i++)
						{
							for (uint8_t j = 0; j < 3; j++)
							{
								if (i + xBlock == x && j + yBlock == y)
									continue;
								if (board[i + xBlock][j + yBlock] == value)
								{
									doesConflict = true;
									break;
								}
							}
						}
					}
				}

				// Conclusion
				if (!doesConflict)
				{
					x++;
					if (x == 9)
					{
						x = 0;
						y++;

						if (y == 9)
							isSuccessful = true;
					}
				}
			}

			// If no other solution
			if (!isSuccessful)
			{
				break;
			}
			else
			{
				x = 8;
				y = 8;
				isGoingBack = true;		// The last number cannot be different, this will not forget any possible solution.
				++actualSolutionCount;
			}
		}

		return actualSolutionCount;
	}


	void Sudoku::initRandomness()
	{
		// Initialize randomness
		sel::Random::init();
	}

	void Sudoku::clear()
	{
		for (auto& column : m_board)
			column.fill(0);

		m_state = State::Empty;
	}

	void Sudoku::generate()
	{
		clear();

		uint8_t firstValues[9][9] = { 0 };

		if (internalSolve(m_board, firstValues, true))
			m_state = State::Solved;
	}

	uint8_t Sudoku::prepare(uint8_t minClueCount)
	{
		int clueCount = 81;
		// Array of all positions
		sel::Vec2<uint8_t> positions[81];
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				positions[i * 9 + j].x = j;		// x position
				positions[i * 9 + j].y = i;		// y position
			}
		}

		// Shuffle positions
		std::shuffle(std::begin(positions), std::end(positions), sel::Random::getEngine());

		uint32_t solutionCount = 1;
		uint8_t lastRemovedClue = 0;
		uint8_t firstValues[9][9];
		std::array<std::array<uint8_t, 9>, 9> boardCopy;

		for (int clueIndex = 0; clueIndex < 81; clueIndex++)
		{
			// Respects minimum clue count
			if (clueCount == minClueCount)
				break;

			// Removes a clue
			--clueCount;
			lastRemovedClue = m_board[positions[clueIndex].x][positions[clueIndex].y];
			m_board[positions[clueIndex].x][positions[clueIndex].y] = 0;

			// Reset variables
			for (int i = 0; i < 9; ++i)
				for (int j = 0; j < 9; ++j)
					firstValues[i][j] = 0;

			boardCopy = m_board;

			// Counts number of solutions
			solutionCount = internalSolve(boardCopy, firstValues, false, 2);

			if (solutionCount != 1)
			{
				if (solutionCount == 0)
					std::cout << "ERROR\n";
				else
					m_board[positions[clueIndex].x][positions[clueIndex].y] = lastRemovedClue;

				break;
			}
		}

		std::cout << "clues: " << clueCount << std::endl;

		return clueCount;
	}

	void Sudoku::solve()
	{
		uint8_t firstValues[9][9] = { 0 };

		if (internalSolve(m_board, firstValues, false))
			m_state = State::Solved;
	}

	bool Sudoku::checkCell(uint8_t x, uint8_t y)
	{
		uint8_t i;
		uint8_t value = m_board[x][y];

		// Checks row
		for (i = 0; i < 9; i++)
			if (m_board[i][y] == value)
				return false;

		// Checks column
		for (i = 0; i < 9; i++)
			if (m_board[x][i] == value)
				return false;

		// Checks block
		uint8_t blockPositionX = x / 3;
		uint8_t blockPositionY = y / 3;

		for (i = blockPositionX; i < blockPositionX + 3; i++)
		{
			for (uint8_t j = blockPositionY; j < blockPositionY + 3; j++)
			{
				// Skips the cell that is checked itself.
				if (x == i && y == j)
					continue;

				if (m_board[i][j] == value)
					return false;
			}
		}

		// No conflict
		return true;
	}

}
