#pragma once
#include <SFML/Application.hpp>

#include "Sudoku.hpp"

#include <vector>


namespace Sudoku {

	class Application : public sf::Application
	{
	public:

		static Application* create();
		static Application* get();
		static void destroy();

		~Application();

		virtual void onEvent(const sf::Event& e) override;
		virtual void onUpdate(const sf::Time& ts) override;
		virtual void onRender() override;


	private:

		Application();


		static Application* s_instance;

		// Grid
		sf::RectangleShape m_grid[16];
		sf::Text m_textNumbers[9][9];

		// Sudoku
		Sudoku m_sudoku;
		int m_minClueCount = 25;

		bool m_updateWindow = true;

		sf::Font m_font;
	};

}
