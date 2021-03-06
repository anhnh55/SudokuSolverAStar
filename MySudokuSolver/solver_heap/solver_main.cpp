#include <stdio.h>
#include <queue>
#include <set>
#include <vector>
#include <time.h>
#include <string>
#include <math.h>
#define SUDOKU_TEST_FOLDER_PATH "Sudoku sets\\"

using namespace std;

struct Cell
{
	int m_i_value;
	bool m_candidate_list[10];
	int m_i_cell_candidate;
};

struct SudokuBoard
{
	Cell m_board[9][9];

	int m_i_value_f;
	int m_i_value_h;
	int m_i_value_g;

	int m_i_total_candidates;
	int m_i_remaining_blank;
};


struct compare_f_value
{
	bool operator()(const SudokuBoard b1, const SudokuBoard b2)
	{
		if(b1.m_i_value_f < b2.m_i_value_f)
			return true;
		if(b1.m_i_value_f == b2.m_i_value_f)
		{
			if(b1.m_i_remaining_blank < b2.m_i_remaining_blank)
				return true;
			if(b1.m_i_remaining_blank == b2.m_i_remaining_blank)
			{
				if (b1.m_i_total_candidates < b2.m_i_total_candidates)
					return true;
				if (b1.m_i_total_candidates == b2.m_i_total_candidates)
				{
					for(int i = 0 ; i < 9 ; i++)
					{
						for(int j = 0 ; j < 9 ; j++)
						{
							if(b1.m_board[i][j].m_i_value != b2.m_board[i][j].m_i_value)
								return b1.m_board[i][j].m_i_value < b2.m_board[i][j].m_i_value;
						}
					}
				}
				if (b1.m_i_total_candidates > b2.m_i_total_candidates)
				{
					return false;
				}
			}
		}
		return false;
	}
};

struct compare_sudoku_board
{
	bool operator()(SudokuBoard b1, SudokuBoard b2)
	{
		for(int i = 0 ; i < 9 ; i++)
		{
			for(int j = 0 ; j < 9 ; j++)
			{
				if(b1.m_board[i][j].m_i_value != b2.m_board[i][j].m_i_value)
					return (b1.m_board[i][j].m_i_value < b2.m_board[i][j].m_i_value);
			}
		}
		return false;
	}
};

struct compare_char
{
	bool operator()(const char* b1,const char* b2)
	{
		for(int i = 0 ; i < 81 ; i ++)
		{
			if(b1[i] != b2[i])
				return b1[i] < b2[i];
		}
		return false;
	}
};

SudokuBoard start_board;
set<SudokuBoard,compare_f_value>*open_set;
set<char*,compare_char>*all_generated_board; //save all generated state
//set<SudokuBoard>::iterator set_it;
int generated_states = 0;
clock_t time_begin;
clock_t time_end;
int count_iter = 0;

char* toString(SudokuBoard);

void readData(SudokuBoard *, FILE *);
void showData(SudokuBoard);
void showCandidatesAt(SudokuBoard,int,int);
bool isSolved(SudokuBoard);
bool checkRow(SudokuBoard,int);
bool checkColumn(SudokuBoard,int);
bool checkGrid(SudokuBoard,int,int);
bool checkValid(SudokuBoard);
void scanCandidatesAt(SudokuBoard*,int,int);//search and add candidate to cell at x,y
void scanAllCandidates(SudokuBoard*);
void generateSuccessorAt(SudokuBoard,int,int);
void generateSuccessor(SudokuBoard);
float calculateHeuristicValue(SudokuBoard);
void addBoard(SudokuBoard);
SudokuBoard runAStar(SudokuBoard);
void output(SudokuBoard);
int main ()
{
	clock_t count_time_start;
	clock_t count_time_end;

	double total_time = 0.0;
	FILE* input = NULL;
	FILE* output = NULL;
	char file_name[100] = {'\0'};

	strcpy(file_name, SUDOKU_TEST_FOLDER_PATH);
	strcat(file_name, "b 01.txt");

	input = fopen(file_name,"r");
	output = fopen("result.txt","a");
	puts(file_name);
	if(!input)
		printf("Can't open input file!");

	readData(&start_board,input);
	showData(start_board);
	scanAllCandidates(&start_board);
	open_set = new set<SudokuBoard,compare_f_value>();
	all_generated_board = new set<char*,compare_char>();
	/*char* temp = toString(start_board);
	for(int i = 0 ; i < 81 ; i++)
		printf("%d ",temp[i]);*/

	//float ave_candidate  = (float)(start_board.m_i_total_candidates)/float(start_board.m_i_remaining_blank);
	//float refinement_value = sqrt(ave_candidate)/(float)(start_board.m_i_remaining_blank/2);
	//fprintf(output,"total %d blank %d ave_can %f, refinement %f\n", start_board.m_i_total_candidates, start_board.m_i_remaining_blank, ave_candidate,refinement_value);

	system("pause");
	
	
	//printf("%d %d\n",sizeof(Cell), sizeof(SudokuBoard));
	/*SudokuBoard test1,test2,test3;
	test1 = start_board;
	test1.m_i_value_f = 1;
	test1.m_i_total_candidates = 5;
	test1.m_i_remaining_blank = 6;
	test1.m_board[0][0].m_i_value = 0;
	test2 = start_board;
	test2.m_i_value_f = 1;
	test2.m_i_total_candidates = 5;
	test2.m_i_remaining_blank = 6;
	test2.m_board[0][0].m_i_value = 1;
	test3 = start_board;
	test3.m_i_value_f = 1;
	test3.m_i_total_candidates = 5;
	test3.m_i_remaining_blank = 6;
	test3.m_board[0][0].m_i_value = 2;
	open_set->insert(test1);
	open_set->insert(test2);
	open_set->insert(test3);
	for(set_it = open_set->begin() ; set_it != open_set->end() ; ++set_it)
	{
		printf("%d %d %d\n",(*set_it).m_i_value_f, (*set_it).m_i_total_candidates, (*set_it).m_i_remaining_blank);
	}*/
	count_time_start = clock();
	runAStar(start_board);
	count_time_end = clock();
	total_time = (count_time_end - count_time_start)/(float)CLOCKS_PER_SEC;
	printf("total time: %f, generated states: %d iteration %d\n",total_time,generated_states,count_iter);
	system("pause");
	return 0;
}

void readData(SudokuBoard* b, FILE *input)
{
	char temp_char;
	b->m_i_remaining_blank = 0;
	b->m_i_total_candidates = 0;

	for (int i = 0 ; i < 9 ; i++)
	{
		for (int j = 0 ; j < 9 ; j++)
		{
			if(j<8)
			{
				fscanf(input,"%c,",&temp_char);
				b->m_board[i][j].m_i_value = temp_char - '0';
				if(b->m_board[i][j].m_i_value > 9)
				{
					b->m_board[i][j].m_i_value = 0;
					b->m_i_remaining_blank++;
				}
			}
			else
			{
				fscanf(input,"%c,\n",&temp_char);
				b->m_board[i][j].m_i_value = temp_char - '0';
				if(b->m_board[i][j].m_i_value > 9)
				{
					b->m_board[i][j].m_i_value = 0;
					b->m_i_remaining_blank++;
				}
			}
		}
	}
	printf("%d\n", b->m_i_remaining_blank);
}

void showData(SudokuBoard b)
{
	for (int i = 0 ; i < 9 ; i++)
	{
		for (int j = 0 ; j < 9 ; j++)
		{
			printf("%d ", b.m_board[i][j].m_i_value);
		}
		printf("\n");
	}
	printf("\n\n");
}

bool isSolved(SudokuBoard b)
{
	if(b.m_i_remaining_blank > 0)
		return false;
	else
	{
		for(int i = 0 ; i < 9 ; i++)
		{
			if(!checkRow(b,i) || !checkColumn(b,i)) 
				return false;
		}

		for(int i = 0 ; i < 9 ; i+=3)
		{
			for(int j = 0 ; j < 9; j+=3)
			{
				if(!checkGrid(b,i,j))
					return false;
			}
		}
		return true;
	}
}

bool checkValid(SudokuBoard b)
{
	for(int i = 0 ; i < 9 ; i++)
	{
		if(!checkRow(b,i) || !checkColumn(b,i)) 
			return false;
	}

	for(int i = 0 ; i < 9 ; i+=3)
	{
		for(int j = 0 ; j < 9; j+=3)
		{
			if(!checkGrid(b,i,j))
				return false;
		}
	}
	return true;
}
bool checkRow(SudokuBoard b,int row)
{
	bool found[10] = {false};

	for(int i = 0 ; i < 9 ; i++)//check duplicate
	{
		if(found[b.m_board[row][i].m_i_value] == false && b.m_board[row][i].m_i_value != 0)
			found[b.m_board[row][i].m_i_value] = true;
		else if(found[b.m_board[row][i].m_i_value] == true)//duplicated
			return false;
	}

	for(int i = 1 ; i <= 9 ; i++)//check value range
	{
		if(found[i] == false)
			return false;
	}

	return true;
}

bool checkColumn(SudokuBoard b, int col)
{
	bool found[10] = {false};

	for(int i = 0 ; i < 9 ; i++)//check duplicate
	{
		if(found[b.m_board[i][col].m_i_value] == false && b.m_board[i][col].m_i_value != 0)
			found[b.m_board[i][col].m_i_value] = true;
		else if(found[b.m_board[i][col].m_i_value] == true)//duplicated
			return false;
	}

	for(int i = 1 ; i <= 9 ; i++)//check value range
	{
		if(found[i] == false)
			return false;
	}

	return true;
}

bool checkGrid(SudokuBoard b, int pX, int pY)
{
	bool found[10] = {false};

	for(int i = pX ; i < pX + 3 ; i++)//check duplicate
		for(int j = pY; j < pY + 3 ; j++)
		{
			if(found[b.m_board[i][j].m_i_value] == false && b.m_board[i][j].m_i_value != 0)
				found[b.m_board[i][j].m_i_value] = true;
			else if(found[b.m_board[i][j].m_i_value] == true)//duplicated
				return false;
		}

	for(int i = 1 ; i<= 9 ; i++)//check value range
	{
		if(found[i] == false)
			return false;
	}

	return true;
}

void scanCandidatesAt(SudokuBoard* b,int pX,int pY)
{
	bool found[10] = {false};

	if(b->m_board[pX][pY].m_i_value != 0)
		return;

	else
	{
		for(int i = 0 ; i < 9 ; i++)//search row
		{
			if(b->m_board[i][pY].m_i_value != 0)
				found[b->m_board[i][pY].m_i_value] = true;
		}
		for(int i = 0 ; i < 9 ; i++)//search column
		{
			if(b->m_board[pX][i].m_i_value != 0)
				found[b->m_board[pX][i].m_i_value] = true;
		}
		for(int i = (pX/3)*3 ; i < (pX/3)*3 + 3 ; i++)//search grid
			for(int j = (pY/3)*3 ; j < (pY/3)*3 + 3; j++)
		{
			{
				if(b->m_board[i][j].m_i_value != 0)
					found[b->m_board[i][j].m_i_value] = true;
			}
		}
		for(int i = 1; i <= 9 ; i++)//candidates
		{
			if(found[i] == false)
			{
				b->m_board[pX][pY].m_candidate_list[i] = 1;
				b->m_i_total_candidates++;
				b->m_board[pX][pY].m_i_cell_candidate++;
			}
		}
	}
}

void scanAllCandidates(SudokuBoard *b)
{
	//printf("scan candidate\n");
	b->m_i_total_candidates = 0;
	for(int i = 0 ; i < 9 ; i++)
	{
		for(int j = 0; j < 9 ; j++)
		{
			b->m_board[i][j].m_i_cell_candidate = 0;
			scanCandidatesAt(b,i,j);
			//showCandidatesAt(*b,i,j);
		}
	}
	//printf("total candidate %d\n",b->m_i_total_candidates);
}

float calculateHeuristicValue(SudokuBoard b)
{
	return (float)b.m_i_total_candidates;
}

void addBoard(SudokuBoard b)
{
	set<char*>::iterator set_it;
	char* temp = toString(b);
	set_it = all_generated_board->find(temp);

	if(set_it == all_generated_board->end())
	{
		all_generated_board->insert(temp);
		open_set->insert(b);
		generated_states++;
	}

	//printf("total set time: %f total queue time: %f\n", set_time, queue_time);
	//else
	//{
	//	printf("detect\n");
	//	//if(b.m_i_value_g < (*set_it).m_i_value_g) //same board but different g(n) value
	//	//{
	//	//	all_generated_board.erase(set_it);
	//	//	all_generated_board.insert(b);
	//	//	open_set.push(b);
	//	//	generated_states++;
	//	//}
	//}
}

void generateSuccessorAt(SudokuBoard b,int i, int j)
{
	int temp_it1;
	int temp_it2;
	if(b.m_board[i][j].m_i_value == 0)//generate successor if blank square
	{
		for(temp_it1 = 1 ; temp_it1 <= 9 ; temp_it1++) //iterate through candidate list
		{
			//time_begin = clock();
			if(b.m_board[i][j].m_candidate_list[temp_it1] == 1) //if candidate
			{
				SudokuBoard temp = b;
				int count_gn = 0;
				temp.m_board[i][j].m_i_value = temp_it1;
				temp.m_i_total_candidates -= temp.m_board[i][j].m_i_cell_candidate; //because temp.m_board[i][j] is not blank
				temp.m_i_remaining_blank--;
				bool legal_successor = true;
				for(int count = 0 ; count < 9 ; count ++)//scan row
				{
					if(temp.m_board[i][count].m_i_value == 0 )
					{
						if(temp.m_board[i][count].m_candidate_list[temp_it1] == 1 && temp.m_board[i][count].m_i_cell_candidate > 1)
						{
							temp.m_board[i][count].m_candidate_list[temp_it1] = 0;
							temp.m_i_total_candidates--;
							temp.m_board[i][count].m_i_cell_candidate--;
						}
						else if(temp.m_board[i][count].m_candidate_list[temp_it1] == 1 && temp.m_board[i][count].m_i_cell_candidate == 1)
						{
							legal_successor = false;
							break;
						}
						count_gn++;
					}
				}
				for(int count = 0 ; count < 9 ; count ++)//scan col
				{
					if(temp.m_board[count][j].m_i_value == 0 )
					{
						if(temp.m_board[count][j].m_candidate_list[temp_it1] == 1 && temp.m_board[count][j].m_i_cell_candidate > 1)
						{
							temp.m_board[count][j].m_candidate_list[temp_it1] = 0;
							temp.m_i_total_candidates--;
							temp.m_board[count][j].m_i_cell_candidate--;
						}
						else if(temp.m_board[count][j].m_candidate_list[temp_it1] == 1 && temp.m_board[count][j].m_i_cell_candidate == 1)
						{
							legal_successor = false;
							break;
						}
						count_gn++;
					}
				}
				for(int count_row = (i/3)*3 ; count_row < (i/3)*3 + 3 ; count_row++)//search grid
				{
					for(int count_col = (j/3)*3 ; count_col < (j/3)*3 + 3; count_col++)
					{
						if(temp.m_board[count_row][count_col].m_i_value == 0 )
						{
							if(count_row != i && count_col != j)
							{
								count_gn++;
							}
							if(temp.m_board[count_row][count_col].m_candidate_list[temp_it1] == 1 && temp.m_board[count_row][count_col].m_i_cell_candidate > 1)
							{
								temp.m_board[count_row][count_col].m_candidate_list[temp_it1] = 0;
								temp.m_i_total_candidates--;
								temp.m_board[count_row][count_col].m_i_cell_candidate --;
							}
							else if(temp.m_board[count_row][count_col].m_candidate_list[temp_it1] == 1 && temp.m_board[count_row][count_col].m_i_cell_candidate == 1)
							{
								legal_successor = false;
								break;
							}
						}
					}
				}
				
				temp.m_i_value_g = count_gn; //calculate g(n)
				//temp.m_i_value_g = temp.m_i_total_candidates;
				//temp.m_i_value_g = b.m_i_value_g + 1;
				////printf("gn %f \n",temp.m_i_value_g);
				////temp.m_i_value_h = calculateHeuristicValue(temp); //h(n)
				temp.m_i_value_h = b.m_board[i][j].m_i_cell_candidate;
				//temp.m_i_value_h  = temp.m_i_total_candidates;
				////printf("hn %f \n",temp.m_i_value_h);
				temp.m_i_value_f = temp.m_i_value_g + temp.m_i_value_h;//f(n)
				//time_end = clock();

				//time_generate += (time_end - time_begin)/CLOCKS_PER_SEC;
				//scanAllCandidates(&temp);
				if(legal_successor)
				{
					//time_begin = clock();
					addBoard(temp);//add successor to open set
					//time_end = clock();
					//time_add +=  (time_end - time_begin)/CLOCKS_PER_SEC;
				}
				/*showData(temp);
				for(int i = 0 ; i < 9 ; i++)
					for(int j = 0 ; j < 9 ; j++)
						showCandidatesAt(temp,i,j);
				printf("\n\n");*/
			}
		}
	}
}


void generateSuccessor(SudokuBoard b)
{
	printf("generating\n");
	for(int i = 0 ; i < 9 ; i++)
	{
		for(int j = 0; j < 9 ; j++)
		{
			generateSuccessorAt(b,i,j);
			//printf("%d %d\n",i,j);
		}
	}
}

SudokuBoard runAStar(SudokuBoard b)
{
	
	SudokuBoard current;
	set<SudokuBoard>::iterator set_it;
	b.m_i_value_g = b.m_i_remaining_blank;
	b.m_i_value_h = b.m_i_total_candidates;
	b.m_i_value_f = b.m_i_value_g + b.m_i_value_h;

	open_set->insert(b);

	while(!open_set->empty())
	{
		//current = open_set.top();
		set_it = open_set->begin();
		//open_set.pop();
		current = *set_it;
		open_set->erase(set_it);
		count_iter++;
		if(isSolved(current))
		{
			printf("done!\n");
			//printf("g: %f, generated states: %d\n",current.m_i_value_g, generated_states);
			showData(current);
			output(current);
			return current;
		}

		//if(current.m_i_total_candidates <= 2)
		//{
		//	showData(current);
		//	system("pause");
		//}
		printf("current's candidate: %d, f %d, state %d iter %d\n", current.m_i_total_candidates,current.m_i_value_f,generated_states,count_iter);
		//printf("size: %d\n", open_set.size());
		generateSuccessor(current);
		//printf("total generate: %f total add: %f\n", time_generate, time_queue);
	}
	return current;
}

void showCandidatesAt(SudokuBoard b, int x, int y)
{
	printf("candidate at %d %d : ",x,y);
	for(int i = 1; i <= 9 ; i++)
	{
		if(b.m_board[x][y].m_candidate_list[i] == 1)
		{
			printf("%d ",i);
		}
	}
	printf("\n");
}

void output(SudokuBoard b)
{
	FILE *output;
	output = fopen("result.txt","w");
	for(int i = 0 ; i < 9 ; i++)
	{
		for(int j = 0 ; j < 9 ; j++)
		{
			fprintf(output,"%d ",b.m_board[i][j].m_i_value);
		}
		fprintf(output,"\n");
	}
}

char* toString(SudokuBoard b)
{
	char* temp = new char[81];
	int count = 0;
	for(int i = 0 ; i < 9 ; i++)
	{
		for(int j = 0 ; j < 9 ; j++)
		{
			temp[count] = (char)b.m_board[i][j].m_i_value;
			//printf("%d %d\n",b.m_board[i][j].m_i_value,temp[count]);
			count++;
			//printf("%d ",count);
		}
	}
	return temp;
}