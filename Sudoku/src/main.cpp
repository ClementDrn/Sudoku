#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "display_functions.h"
#include "essentials.h"
#include "maker.h"
#include "solver.h"

using namespace std;
using namespace sf;


static enum class State
{
    Blank,
    Generated,
    Hinted,
    Solved
};


int main()
{
    std::cout << "HEY\n";
    // Window constructor
    RenderWindow window(VideoMode(900, 900), "Sudoku", Style::None);
    Vector2u winSize(window.getSize());
   
    // Grid constructor
    vector<RectangleShape*> grid(createGrid());

    // Cells constructor
    vector<vector<vector<unsigned short int>>> cells(9, vector<vector<unsigned short int>>(9, { 0 }));  // Grid containing all lists of numbers
    vector<vector<Text*>> nums(9, { 0, 0, 0, 0, 0, 0, 0, 0, 0 });   // Grid containing all number texts

    // Other variables constructor
    bool updateWindow(true);

    State state(State::Blank);

    // Essentials constructor
    essentialsInit(&cells, &nums, &updateWindow);

    // Main Loop
    while (window.isOpen())
    {
        // Events
        Event event;
        while (window.pollEvent(event))
        {
            // The window has been closed
            if (event.type == Event::Closed)
                window.close();

            // The window has been resized
            if (event.type == sf::Event::Resized)
            {
                winSize.x = event.size.width;
                winSize.y = event.size.height;
                window.setView(View(FloatRect({ 0.f, 0.f }, { (float)winSize.x, (float)winSize.y })));
            }

            // A key was pressed
            if (event.type == Event::KeyPressed)
            {
                // The escape key
                if (event.key.code == Keyboard::Escape)
                    window.close();

                // Board Generator "G"
                if (event.key.code == Keyboard::G)
                {
                    createBoard(window, grid, nums);
                    state = State::Generated;
                }

                // Leave clues "H"
                if (event.key.code == Keyboard::H && state == State::Generated)
                {
                    setClues(window, grid, nums, 25);
                    state = State::Hinted;
                }

                // New blank board "N"
                if (event.key.code == Keyboard::N)
                {
                    clearBoard();
                    state = State::Blank;
                }

                // Solve the Sudoku "S"
                if (event.key.code == Keyboard::S && state == State::Hinted)
                {
                    solveSudoku(window, grid, nums);
                    state = State::Solved;
                }
            }
        }

        if (updateWindow)
        {
            updateWin(window, grid, nums);

            // Sets "updateWindow" to false
            updateWindow = false;
        }
    }

    // Destructor
    essentialsDestructor();
    for (auto& const ptr : grid)
        delete ptr;
    for (int i(0); i < 9; i++)
    {
        for (auto& const ptr : nums[i])
        {
            if (ptr)
                delete ptr;
            ptr = 0;
        }
    }

    return 0;
}
