#include "Application.hpp"

#include <iostream>


namespace Sudoku {

    Application* Application::s_instance = nullptr;
    
    
    static sf::Color getCorrespondingColor(uint8_t value)
    {
        switch (value)
        {
            case 1:
                return sf::Color(255, 0, 0);
            case 2:
                return sf::Color(255, 170, 0);
            case 3:
                return sf::Color(170, 255, 0);
            case 4:
                return sf::Color(0, 255, 0);
            case 5:
                return sf::Color(0, 255, 170);
            case 6:
                return sf::Color(0, 170, 255);
            case 7:
                return sf::Color(0, 0, 255);
            case 8:
                return sf::Color(170, 0, 255);
            case 9:
                return sf::Color(255, 0, 170);
            default:
                return sf::Color::Black;
        }
    }


    Application::Application()
        : sf::Application("Sudoku", { .winWidth = 900, .winHeight = 900, .clearColor = sf::Color::White })
    {
        // Initialize sudoku randomness
        Sudoku::initRandomness();

        // Creates font
        if (!m_font.loadFromFile("assets/RobotoMono-Regular.ttf"))
            std::cerr << "Error when trying to load RobotoMono font.\n";

        // Creates sudoku grid
        int i, j;
        float linePosition;
        sf::Color color(180, 180, 180);
        for (i = 0; i < 8; i++)
        {
            linePosition = (float)(((i * 3) % 8) + 1) * 100;		    // Creates lines in that order: 1, 4, 7, 2, 5, 8, 3, 6 (finishes with the darker lines).
            j = 2 * i;

            m_grid[j] = sf::RectangleShape({ 900.0f, 1.0f });
            m_grid[j].setPosition({ 0.0f, linePosition });

            m_grid[j + 1] = sf::RectangleShape({ 1.0f, 900.0f });
            m_grid[j + 1].setPosition({ linePosition, 0.0f });

            if (linePosition == 300.0f)       // From the line 3, lines are darker (3 and 6)
                color = sf::Color::Black;

            m_grid[j].setFillColor(color);
            m_grid[j + 1].setFillColor(color);
        }

        // Initialize text numbers
        for (i = 0; i < 9; i++)
        {
            for (j = 0; j < 9; j++)
            {
                m_textNumbers[i][j] = sf::Text("0", m_font, 100);
                m_textNumbers[i][j].setPosition({ (float)(i * 100 + 20), (float)(j * 100 - 15) });
            }
        }
    }


    Application* Application::create()
    {
        if (s_instance)
            delete s_instance;

        s_instance = new Application();

        return s_instance;
    }

    Application* Application::get()
    {
        return s_instance;
    }

    void Application::destroy()
    {
        if (s_instance)
            delete s_instance;
    }

    Application::~Application()
    {
        s_instance = nullptr;
    }


    void Application::onEvent(const sf::Event& e)
	{
        // The window has been closed
        if (e.type == sf::Event::Closed)
            m_window->close();

        // The window has been resized
        if (e.type == sf::Event::Resized)
        {
            m_window->setSize({ e.size.width, e.size.height });
            m_window->setView(sf::View(sf::FloatRect({ 0.0f, 0.0f }, { (float)e.size.width, (float)e.size.height })));
        }

        // A key has been pressed
        if (e.type == sf::Event::KeyPressed)
        {
            // The escape key
            if (e.key.code == sf::Keyboard::Escape)
                m_window->close();

            // Board Generator "G"
            if (e.key.code == sf::Keyboard::G)
            {
                m_sudoku.generate();
            }

            // Leave clues "P"
            if (e.key.code == sf::Keyboard::P)
            {
                m_sudoku.prepare(25);
            }

            // New blank board "N"
            if (e.key.code == sf::Keyboard::N)
            {
                m_sudoku.clear();
            }

            // Solve the Sudoku "S"
            if (e.key.code == sf::Keyboard::S)
            {
                m_sudoku.solve();
            }
        }
	}

	void Application::onUpdate(const sf::Time& ts)
	{

	}
	
	void Application::onRender()
	{
        int i, j;
        uint8_t number;
        constexpr const char charNumbers[] = "0123456789";

        // Displays grid
        for (i = 0; i < 16; i++)
            m_window->draw(m_grid[i]);

        auto board = m_sudoku.getBoard();

        // Displays text numbers
        for (i = 0; i < 9; i++)
        {
            for (j = 0; j < 9; j++)
            {
                number = board[i][j];
                //std::cout << (int)number << std::endl;
                if (number != 0)
                {
                    if (m_textNumbers[i][j].getString()[0] != charNumbers[number])
                    {
                        m_textNumbers[i][j].setString(charNumbers[number]);
                        m_textNumbers[i][j].setFillColor(getCorrespondingColor(number));
                    }

                    m_window->draw(m_textNumbers[i][j]);
                }
            }
        }
	}

}
