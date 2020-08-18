#include "AStarSolver.h"
#include <vector>
using namespace std;

vector<int>::iterator temp_it1;
vector<int>::iterator temp_it2;

AStarSolver::AStarSolver(void)
{
	m_i_generated_states = 0;
}


AStarSolver::~AStarSolver(void)
{
}

void AStarSolver::addBoard(SudokuBoard b)
{
	printf("adding board\n");
	/*m_it = m_all_generated_board.find(b);
	if(m_it == m_all_generated_board.end())
	{
		m_all_generated_board.insert(b);
		m_open_set.push(b);
		m_i_generated_states++;
	}*/
	m_open_set.push(b);
	//else
	//{
	//	if(b.m_i_value_g < (*m_it).m_i_value_g) //same board but different g(n) value
	//	{
	//		m_all_generated_board.erase(m_it);
	//		m_all_generated_board.insert(b);
	//		m_open_set.push(b);
	//		m_i_generated_states++;
	//	}
	//}
}

void AStarSolver::generateSuccessor(SudokuBoard b)
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

void AStarSolver::generateSuccessorAt(SudokuBoard b,int i, int j)
{
	if(b.m_board[i][j].m_i_value == 0)//blank square
	{
		for(temp_it1 = b.m_board[i][j].m_candidate_list.begin() ; temp_it1 != b.m_board[i][j].m_candidate_list.end(); temp_it1++) //iterate through candidate list
		{
			//printf("here\n");
			SudokuBoard temp(b);
			temp.m_board[i][j].m_i_value = (*temp_it1);
			temp.m_i_remaining_blank--;
			//re-scan candidate for bank slots in the same row , column , grid with i,j
			for(int count = 0 ; count < 9 ; count ++)//scan row
			{
				//printf("here %d\n",count);
				if(temp.m_board[i][count].m_i_value == 0) //remove candidate with same value with (*temp_it1)
				{
					for(temp_it2 = temp.m_board[i][count].m_candidate_list.begin() ; temp_it2 != temp.m_board[i][count].m_candidate_list.end() ; temp_it2++)
					{
						if((*temp_it1) == (*temp_it2))
						{
					        //printf("here %d\n" , *temp_it2);
							temp.m_board[i][count].m_candidate_list.erase(temp_it2);
							temp.m_i_total_candidates--;
							break;
						}
					}
				}
			}
			for(int count = 0 ; count < 9 ; count ++)//scan column
			{
				if(temp.m_board[count][j].m_i_value == 0) //remove candidate with same value with (*temp_it1)
				{
					for(temp_it2 = temp.m_board[count][j].m_candidate_list.begin() ; temp_it2 != temp.m_board[count][j].m_candidate_list.end() ; temp_it2++)
					{
						if((*temp_it1) == (*temp_it2))
						{
							//printf("here %d\n" , *temp_it2);
							temp.m_board[count][j].m_candidate_list.erase(temp_it2);
							temp.m_i_total_candidates--;
							break;
						}
					}
				}
			}
			for(int count_row = (i/3)*3 ; count_row < (i/3)*3 + 3 ; count_row++)//search grid
			{
				for(int count_col = (j/3)*3 ; count_col < (j/3)*3 + 3; count_col++)
				{
					if(temp.m_board[count_row][count_col].m_i_value == 0 && (count_row != i || count_col != j))
					{
						for(temp_it2 = temp.m_board[count_row][count_col].m_candidate_list.begin() ; temp_it2 != temp.m_board[count_row][count_col].m_candidate_list.end() ; temp_it2++)
						{
							if((*temp_it1) == (*temp_it2))
							{
								//printf("here %d\n" , *temp_it2);
								temp.m_board[count_row][count_col].m_candidate_list.erase(temp_it2);
								temp.m_i_total_candidates--;
								break;
							}
						}
					}
				}
			}
			//temp.m_i_value_g = b.m_i_value_g + 1.0; //calculate g(n)
			//temp.scanAllCandidates();
			temp.m_i_value_h = calculateHeuristicValue(temp); //h(n)
			temp.m_i_value_f = temp.m_i_value_g + temp.m_i_value_h;//f(n)
			addBoard(temp);//add successor to open set
		}
	}
}

SudokuBoard AStarSolver::runAStar(SudokuBoard b)//b is starting board
{
	SudokuBoard current;

	b.m_i_value_g = 0;
	b.m_i_value_h = calculateHeuristicValue(b);
	b.m_i_value_f = b.m_i_value_g + b.m_i_value_h;

	m_open_set.push(b);

	while(!m_open_set.empty())
	{
		current = m_open_set.top();
		m_open_set.pop();

		if(current.isSolved())
		{
			printf("done!\n");
			printf("g: %f, generated states: %d\n",current.m_i_value_g, m_i_generated_states);
			current.showBoard();
			return current;
		}
		generateSuccessor(current);
		printf("current's candidate: %d\n", current.m_i_total_candidates);
		printf("size: %d\n", m_open_set.size());
	}
	return current;
}

float AStarSolver::calculateHeuristicValue(SudokuBoard b)
{
	return b.m_i_total_candidates;
}

bool compare_f_value::operator()(const SudokuBoard b1, const SudokuBoard b2)
{
	if(b2.m_i_value_f < b1.m_i_value_f) 
			return true;
	return false;
}

bool compare_sudoku_board::operator()(SudokuBoard b1, SudokuBoard b2)
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