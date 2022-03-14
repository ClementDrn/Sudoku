#include "Application.hpp"

#include <iostream>
#include <SEL/Random.hpp>

int main()
{
    Sudoku::Application::create();

    Sudoku::Application::get()->run();

    Sudoku::Application::destroy();
}
