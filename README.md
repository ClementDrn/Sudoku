# Sudoku

This is a SFML application that allows you to generate Sudoku boards and then solve them.

## Installation

Clone the projet but do not forget to include submodules with `--recursive`.

```bash
git clone --recursive https://github.com/ClementDrn/Sudoku
```

Generate the projects, by executing the `WinVS-GenerateProjects.bat` batch file under the `Scripts/` directory.

Now you have the Sudoku VS project which is linked to the SFML project. You'll need to build both once in Visual Studio.

## Usage

Key inputs allow you to interact with the application:
* `N` "New": Erases the current sudoku board.
* `G` "Generate": Generates a valid and completed board.
* `H` "Hints": Hides numbers and leaves only a few hints so there is only a possible solution.
* `ESCAPE`: Exits application.
