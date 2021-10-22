//header file for the puzzleObject class
//Author: David Hilderman
//created 5/15/21
//implemetation ideas: overload << and >> operators for using class with cout


#include <iostream>
#include <string>
#include <algorithm>
#pragma once

class CellObject {						//contains all the data for a single square in a sudoku puzzle
    friend class PuzzleObject;
    friend class CellGroup;
private:
    int m_value{ 0 };
    int m_row{ 0 };
    int m_column{ 0 };
    int m_block{ 0 };
    bool m_possibleValues[9]{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    void consolePrint() {
        std::cout << "Row: " << m_row << std::endl;
        std::cout << "Column: " << m_column << std::endl;
        std::cout << "Value: " << m_value << std::endl;
        std::cout << "Block: " << m_block << std::endl;
    }
    void initialize(int t_value, int t_row, int t_column, int t_block)
    {
        m_value = t_value;
        m_row = t_row + 1;
        m_column = t_column + 1;
        m_block = t_block;
    }
};

class CellGroup {
    friend class PuzzleObject;
private:
    CellObject* m_cellPosition[9]{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    bool m_emptyCellList[9]{ 1,1,1,1,1,1,1,1,1 };

    void assignCell(int t_position, CellObject* t_address)
    {
        m_cellPosition[t_position] = t_address;
        if (t_address->m_value != 0) 
        {
            m_emptyCellList[t_position] = false;
        }
    }
    CellObject* locate(int t_sValue)            //searches CellGroup for a Value
    {
        if (t_sValue < 1 || t_sValue > 9)
        {
            throw "Search input error";         //prevents input error
        }
        for (int lcv = 0; lcv < 9; lcv++)
        {
            if (m_cellPosition[lcv]->m_value == t_sValue)
            {
                return m_cellPosition[lcv];     //returns address of cell with value
            }
        }
        return nullptr;                         //return null if no matching value is found
    }
};

class PuzzleObject { 
    
private:
	CellObject m_cellGrid[9][9];
	CellGroup m_rowList[9];
	CellGroup m_columnList[9];
	CellGroup m_blockList[9];
    const int m_suppBlockList[9][4]{1,2,3,6, 0,2,4,7, 0,1,5,8, 0,4,5,6, 1,3,5,7, 2,3,4,8, 0,3,7,8, 1,4,6,8, 2,5,6,7};   //list of each blocks neighbors
    int m_valueCount[10]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };					//list of each instance of a value within the puzzle, maintained with each solved cell
    int m_greatestValueList[9]{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };			    //sorted list of which values have the most instances, updated once per cycle through all 9 values

public:
	PuzzleObject(int t_inputValues[9][9]) {
        int whichBlock = 0;                                                 //helps to set each cells' block value
        int blockPosition = 0;                                              //helps to set each cells address in the correct position within each block
        for (int rowCounter = 0; rowCounter < 9; rowCounter++)              //steps through each row for intialization
        {
            for (int colCounter = 0; colCounter < 9; colCounter++)          //steps through each column for intialization
            {
                if (rowCounter <= 2)          //sets block value for each cell
                {
                    if (colCounter <= 2) {
                        whichBlock = 1;
                    }
                    else if (colCounter >= 3 && colCounter <= 5) {
                        whichBlock = 2;
                    }
                    else {
                        whichBlock = 3;
                    }
                }
                else if (rowCounter >= 3 && rowCounter <= 5)
                {
                    if (colCounter <= 2) {
                        whichBlock = 4;
                    }
                    else if (colCounter >= 3 && colCounter <= 5) {
                        whichBlock = 5;
                    }
                    else {
                        whichBlock = 6;
                    }
                }
                else
                {
                    if (colCounter <= 2) {
                        whichBlock = 7;
                    }
                    else if (colCounter >= 3 && colCounter <= 5) {
                        whichBlock = 8;
                    }
                    else {
                        whichBlock = 9;
                    }
                }

                m_cellGrid[rowCounter][colCounter].initialize(t_inputValues[rowCounter][colCounter], rowCounter, colCounter, whichBlock);     //sets up cell with value, row, column, and block data
                m_rowList[rowCounter].assignCell(colCounter, &m_cellGrid[rowCounter][colCounter]);
                m_columnList[colCounter].assignCell(rowCounter, &m_cellGrid[rowCounter][colCounter]);
                m_blockList[whichBlock - 1].assignCell(blockPosition, &m_cellGrid[rowCounter][colCounter]);

                m_valueCount[t_inputValues[rowCounter][colCounter]]++;      //counts the number of each value that is present in the intial puzzle

                blockPosition++;                                      //value steps in 3 number increments    0,1,2      3,4,5       6,7,8
                if (blockPosition % 3 == 0) {
                    blockPosition -= 3;
                }
            }
            blockPosition += 3;                                       //pushes value into next set
            if (blockPosition == 9) {                                 //resets value back into first set
                blockPosition -= 9;
            }
        }
        greatestValueSort();
	}

    CellObject* search(char t_searchObject, int t_objectNumber, int t_searchValue)
    {
        t_searchObject = toupper(t_searchObject);
        if (!((t_searchObject == 'R') || (t_searchObject == 'C') || (t_searchObject == 'B')))
        {
            throw std::string("Search type error!!");
        }
        if (t_objectNumber < 0 || t_objectNumber > 8)
        {
            throw std::string("Object Number error!!");             //prevents index error
        }
        switch (t_searchObject)
        {
        case 'R':
            return m_rowList[t_objectNumber].locate(t_searchValue);
            break;
        case 'C':
            return m_columnList[t_objectNumber].locate(t_searchValue);
            break;
        case 'B':
            return m_blockList[t_objectNumber].locate(t_searchValue);
            break;
        default:
            break;
        }
        throw std::string("Value not found");                   //should never be thrown
    }

    void greatestValueSort()
    {
        int valueListCopy[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (int lcv = 1; lcv < 10; lcv++)
            valueListCopy[lcv] = m_valueCount[lcv];

        for (int outer = 1; outer < 10; outer++)
        {
            int highest = valueListCopy[1];
            int index = 1;
            for (int inner = 2; inner < 10; inner++)
            {
                if (valueListCopy[inner] > highest)
                {
                    highest = valueListCopy[inner];
                    index = inner;
                }
            }
            m_greatestValueList[outer - 1] = index;
            valueListCopy[index] = 0;
        }
    }

    void consoleCellPrint(int t_row, int t_column) {                        //takes a row and column value of 1 - 9
        m_cellGrid[t_row - 1][t_column - 1].consolePrint();
    }
    void consolePrint() {
        std::cout << std::endl;
        for (int rowCounter = 0; rowCounter < 9; rowCounter++) {
            for (int colCounter = 0; colCounter < 9; colCounter++) {
                if (m_cellGrid[rowCounter][colCounter].m_value > 0) {
                    std::cout << m_cellGrid[rowCounter][colCounter].m_value << " ";
                }
                else {
                    std::cout << "- ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    int solveBlockSupport (int t_solvingBlockID, int t_solveValue)     //clean up needed, variable names unclear
    {
        bool emptyCell[9]{};
        int totalEmptyCell = 0;
        int solveLocation = 0;
        std::copy(m_blockList[t_solvingBlockID].m_emptyCellList, m_blockList[t_solvingBlockID].m_emptyCellList + 9, emptyCell);

        for (int neighborBlock = 0; neighborBlock < 4; neighborBlock++)
        {
            int supportBlock = m_suppBlockList[t_solvingBlockID][neighborBlock];
            CellObject* supportCell = m_blockList[supportBlock].locate(t_solveValue);
            if (supportCell != nullptr)
            {
                for (int crossCheck = 0; crossCheck < 9; crossCheck++)
                {
                    if (emptyCell[crossCheck])
                    {
                        if (m_blockList[t_solvingBlockID].m_cellPosition[crossCheck]->m_row == supportCell->m_row ||
                            m_blockList[t_solvingBlockID].m_cellPosition[crossCheck]->m_column == supportCell->m_column)
                        {
                            emptyCell[crossCheck] = 0;
                        }
                    }
                }
            }
        }
        for (int solveCell = 0; solveCell < 9; solveCell++)
        {
            if (emptyCell[solveCell])
            {
                totalEmptyCell++;
                solveLocation = solveCell;
            }
        }
        if (totalEmptyCell == 1)
        {
            m_blockList[t_solvingBlockID].m_cellPosition[solveLocation]->m_value = t_solveValue;
            m_blockList[t_solvingBlockID].m_emptyCellList[solveLocation] = false;
            m_valueCount[t_solveValue]++;
            consolePrint();
            std::cout << "Solved cell r " << m_blockList[t_solvingBlockID].m_cellPosition[solveLocation]->m_row << " c " << m_blockList[t_solvingBlockID].m_cellPosition[solveLocation]->m_column << std::endl;
            return 1;
        }
        else 
        {
            return 0;
        }
    }
    void solve()
    {
        int solveCount = 0;
        for (int greatestValueLoop = 0; greatestValueLoop < 9; greatestValueLoop++)
        {
            int solveValue = m_greatestValueList[greatestValueLoop];
            for (int solveCursorBlock = 0; solveCursorBlock < 9; solveCursorBlock++)
            {
                CellObject* solveCursor = m_blockList[solveCursorBlock].locate(solveValue);
                if (solveCursor == nullptr)
                {
                    solveCount += solveBlockSupport(solveCursorBlock, solveValue);
                }
            }
        }
        greatestValueSort();
    }
    /*void printValueCount() {
        for (int lcv = 0; lcv < 10; lcv++) {
            std::cout << m_valueCount[lcv] << " ";
        }
        std::cout << std::endl;
    }
    void printGreatestValueList() {
        for (int lcv = 0; lcv < 9; lcv++) {
            std::cout << m_greatestValueList[lcv] << " ";
        }
        std::cout << std::endl;
    }
    */ // test functions
};