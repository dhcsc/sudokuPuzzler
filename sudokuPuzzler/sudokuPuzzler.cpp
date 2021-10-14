// sudokuPuzzler.cpp
// Author: Dave Hilderman
// Created 5/15/21
// Edited  5/15/21
// Third times the charm

#include <iostream>
#include <string>
#include "puzzleObject.hpp"

using namespace std;

int main()
{
    int input[9][9] = { 8, 0, 0, 0, 1, 0, 5, 6, 0, 5, 0, 7, 0, 0, 0, 8, 0, 0, 0, 6, 0, 0, 5, 3, 0, 9, 0, 0,
        0, 1, 0, 6, 0, 9, 8, 4, 4, 0, 0, 0, 7, 0, 0, 0, 5, 9, 8, 5, 0, 3, 0, 2, 0, 0, 0, 9, 0, 7, 4, 0, 0, 2,
        0, 0, 0, 2, 0, 0, 0, 6, 0, 9, 0, 5, 8, 0, 9, 0, 0, 0, 7 };

    PuzzleObject puzzle(input);

    //puzzle.consoleCellPrint(6, 1);
    //test Git

    puzzle.consolePrint();

    puzzle.solveBlockSupport(0, 2);
    return 0;
}

