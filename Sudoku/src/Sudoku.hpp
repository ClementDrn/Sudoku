#pragma once

#include <array>


namespace Sudoku {

	class Sudoku
	{
	public:

		enum class State
		{
			Empty,
			Prepared,
			Edited,
			Solved
		};

		Sudoku() = default;

		static void initRandomness();

		const std::array<std::array<uint8_t, 9>, 9>& getBoard() const { return m_board; }
		
		void clear();
		void generate();
		uint8_t prepare(uint8_t minClueCount);
		void solve();

		void setCell(uint8_t x, uint8_t y, uint8_t value) { m_board[x][y] = value; }
		bool checkCell(uint8_t x, uint8_t y);

		State getState() const { return m_state; }


	private:

		std::array<std::array<uint8_t, 9>, 9> m_board = { 0 };
		State m_state = State::Empty;
	};

}
