#pragma once
#include "Cell.h"
#include <stdio.h>
class SudokuBoard
{
public:
	SudokuBoard(void);
	SudokuBoard(FILE*);
	//~SudokuBoard(void);
	void readData(FILE*);
	Cell m_board[9][9];
	bool isSolved();
	void scanCandidatesAt(int,int);//search and add candidate to cell at x,y
	void scanAllCandidates();
	void showBoard() const;
	int m_i_remaining_blank;
	bool checkRow(int);
	bool checkColumn(int);
	bool checkGrid(int,int);

	float m_i_value_f;
	float m_i_value_h;
	float m_i_value_g;

	int m_i_total_candidates;
};

