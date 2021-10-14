//header file for the puzzleObject class
//Author: David Hilderman
//created 5/15/21
//implemetation ideas: overload << and >> operators for using class with cout


#include <iostream>
#include <string>
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

public:
    void consolePrint() {
        std::cout << "Row: " << m_row << std::endl;
        std::cout << "Column: " << m_column << std::endl;
        std::cout << "Value: " << m_value << std::endl;
        std::cout << "Block: " << m_block << std::endl;
    }
};

class CellGroup {
    friend class PuzzleObject;
private:
    CellObject* m_cellPosition[9]{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    bool m_emptyCellList[9]{ 0,0,0,0,0,0,0,0,0 };

public:
};

class PuzzleObject {					//comment 
    
private:
	CellObject m_cellGrid[9][9];
	CellGroup m_row[9];
	CellGroup m_column[9];
	CellGroup m_block[9];
    int m_valueCount[10]{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };					//list of each instance of a value within the puzzle, maintained with each solved cell
    int m_greatestValueList[9]{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };			    //sorted list of which values have the most instances, updated once per cycle through all 9 values

public:
	PuzzleObject(int t_inputValues[9][9]) {
        int blockPositionHelper = 1;                                        //helps to set each cells address in the correct position within each block
        int helper = 0;
        int arrayCounter = 0;
        for (int rowCounter = 0; rowCounter < 9; rowCounter++)              //steps through each row for intialization
        {
            for (int colCounter = 0; colCounter < 9; colCounter++)          //steps through each column for intialization
            {
                m_cellGrid[rowCounter][colCounter].m_value = t_inputValues[rowCounter][colCounter];
                m_cellGrid[rowCounter][colCounter].m_row = rowCounter + 1;
                m_cellGrid[rowCounter][colCounter].m_column = colCounter + 1;

                if (m_cellGrid[rowCounter][colCounter].m_row <= 3)          //sets block value for each cell
                {
                    if (m_cellGrid[rowCounter][colCounter].m_column <= 3) {
                        m_cellGrid[rowCounter][colCounter].m_block = 1;
                    }
                    else if (m_cellGrid[rowCounter][colCounter].m_column >= 4 && m_cellGrid[rowCounter][colCounter].m_column <= 6) {
                        m_cellGrid[rowCounter][colCounter].m_block = 2;
                    }
                    else {
                        m_cellGrid[rowCounter][colCounter].m_block = 3;
                    }
                }
                else if (m_cellGrid[rowCounter][colCounter].m_row >= 4 && m_cellGrid[rowCounter][colCounter].m_row <= 6)
                {
                    if (m_cellGrid[rowCounter][colCounter].m_column <= 3) {
                        m_cellGrid[rowCounter][colCounter].m_block = 4;
                    }
                    else if (m_cellGrid[rowCounter][colCounter].m_column >= 4 && m_cellGrid[rowCounter][colCounter].m_column <= 6) {
                        m_cellGrid[rowCounter][colCounter].m_block = 5;
                    }
                    else {
                        m_cellGrid[rowCounter][colCounter].m_block = 6;
                    }
                }
                else
                {
                    if (m_cellGrid[rowCounter][colCounter].m_column <= 3) {
                        m_cellGrid[rowCounter][colCounter].m_block = 7;
                    }
                    else if (m_cellGrid[rowCounter][colCounter].m_column >= 4 && m_cellGrid[rowCounter][colCounter].m_column <= 6) {
                        m_cellGrid[rowCounter][colCounter].m_block = 8;
                    }
                    else {
                        m_cellGrid[rowCounter][colCounter].m_block = 9;
                    }
                }

                arrayCounter++;
                helper = t_inputValues[rowCounter][colCounter];
                m_valueCount[helper] += 1;      //counts the number of each value that is present in the intial puzzle

                m_row[rowCounter].m_cellPosition[colCounter] = &m_cellGrid[rowCounter][colCounter];
                m_column[colCounter].m_cellPosition[rowCounter] = &m_cellGrid[rowCounter][colCounter];
                m_block[m_cellGrid[rowCounter][colCounter].m_block].m_cellPosition[blockPositionHelper - 1] = &m_cellGrid[rowCounter][colCounter];

                blockPositionHelper++;                                      //value steps in 3 number increments    1,2,3      4,5,6       7,8,9
                if (blockPositionHelper % 3 == 1) {
                    blockPositionHelper -= 3;
                }

                m_block[m_cellGrid[rowCounter][colCounter].m_block - 1].m_cellPosition[0] = 
                    &m_cellGrid[rowCounter][colCounter];                    //puts address of current cell in the correct spot in the correct block
            }
            blockPositionHelper += 3;                                       //pushes value into next set
            if (blockPositionHelper == 10) {                                //resets value back into first set
                blockPositionHelper -= 9;
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
        switch (t_searchObject)
        {
        case 'R':
            for (int col = 0; col < 9; col++)
            {
                if (m_cellGrid[t_objectNumber][col].m_value == t_searchValue)
                    return &m_cellGrid[t_objectNumber][col];
            }
            break;
        case 'C':
            for (int rowLoop = 0; rowLoop < 9; rowLoop++)
            {
                if (m_cellGrid[rowLoop][t_objectNumber].m_value == t_searchValue)
                    return &m_cellGrid[rowLoop][t_objectNumber];
            }
            break;
        case 'B':
            for (int blockLoop = 0; blockLoop < 3; blockLoop++) {
                if (m_block[t_objectNumber].m_cellPosition[blockLoop]->m_value = t_searchValue) {
                    return m_block[t_objectNumber].m_cellPosition[blockLoop];
                }
            }
            break;
        default:
            break;
        }
        throw std::string("Value not found");
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
    void printValueCount() {
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
};