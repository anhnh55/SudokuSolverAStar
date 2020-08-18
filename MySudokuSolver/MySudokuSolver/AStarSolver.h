#pragma once

#include <set>
#include <queue>
#include "SudokuBoard.h"

class compare_f_value
{
public:
	bool operator()(const SudokuBoard b1, const SudokuBoard b2);
};

class compare_sudoku_board
{
public:
	bool operator()(SudokuBoard b1, SudokuBoard b2);
};

class AStarSolver
{
public:
	AStarSolver(void);
	~AStarSolver(void);
	priority_queue<SudokuBoard,vector<SudokuBoard>,compare_f_value> m_open_set; //set of node waiting to be expanded
	set<SudokuBoard,compare_sudoku_board> m_all_generated_board;
	set<SudokuBoard>::iterator m_it;
	void generateSuccessor(SudokuBoard);
	void generateSuccessorAt(SudokuBoard,int,int);
	void addBoard(SudokuBoard);
	SudokuBoard runAStar(SudokuBoard);
	float calculateHeuristicValue(SudokuBoard);
	int m_i_generated_states;
};

