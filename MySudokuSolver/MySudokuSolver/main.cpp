#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <time.h>
#include "SudokuBoard.h"
#include "AStarSolver.h"

#define SUDOKU_TEST_FOLDER_PATH "Sudoku sets\\"

int main()
{
	clock_t count_time_start;
	clock_t count_time_end;

	float total_time = 0;
	FILE* input = NULL;
	char file_name[100] = {'\0'};

	strcpy(file_name, SUDOKU_TEST_FOLDER_PATH);
	strcat(file_name, "Grid 02.txt");

	input = fopen(file_name,"r");

	if(!input)
		printf("Can't open input file!");

	SudokuBoard test(input);
	AStarSolver solver;

	test.showBoard();
	test.scanAllCandidates();
	//solver.generateSuccessor(test);
	//solver.generateSuccessorAt(test,0,0);
	//solver.generateSuccessorAt(test,0,1);
	//solver.generateSuccessorAt(test,0,2);
	count_time_start = clock();
	solver.runAStar(test);
	count_time_end = clock();
	total_time = (count_time_end - count_time_start)/CLOCKS_PER_SEC;
	printf("total time: %f",total_time);
	system("pause");
	return 0;
}