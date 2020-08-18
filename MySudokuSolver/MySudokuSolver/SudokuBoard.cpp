#include "SudokuBoard.h"

SudokuBoard::SudokuBoard(void)
{
	for(int i = 0; i < 9 ; i++)
	{
		for(int j = 0 ; j < 9; j++)
		{
			m_board[i][j].m_i_value = -1;
			m_board[i][j].m_i_posX = i;
			m_board[i][j].m_i_posY = j;
			m_board[i][j].m_b_isBlank= false;
		}
	}
}

SudokuBoard::SudokuBoard(FILE* input)
{
	char temp_char;
	m_i_remaining_blank = 0;
	m_i_total_candidates = 0;
	for (int i = 0 ; i < 9 ; i++)
	{
		for (int j = 0 ; j < 9 ; j++)
		{
			fscanf(input,"%c,",&temp_char);
			m_board[i][j].m_i_value = temp_char - '0';
			if(m_board[i][j].m_i_value > 9)
			{
				m_board[i][j].m_i_value = 0;
				m_board[i][j].m_b_isBlank= true;
				m_i_remaining_blank++;
			}
			else
				m_board[i][j].m_b_isBlank= false;

			m_board[i][j].m_i_posX = i;
			m_board[i][j].m_i_posY = j;
			//printf("%d ", m_board[i][j].m_i_value);
		}
		//printf("\n");
	}
	m_i_value_f = 0;
	m_i_value_g = 0;
	m_i_value_h = 0;
}

void SudokuBoard::showBoard() const
{
	for (int i = 0 ; i < 9 ; i++)
	{
		for (int j = 0 ; j < 9 ; j++)
		{
			printf("%d ", m_board[i][j].m_i_value);
		}
		printf("\n");
	}
	printf("\n\n");
}
//SudokuBoard::~SudokuBoard(void)
//{
//}

bool SudokuBoard::isSolved()
{
	if(m_i_remaining_blank > 0)
		return false;
	else
	{
		for(int i = 0 ; i < 9 ; i++)
		{
			if(!checkRow(i) || !checkColumn(i)) 
				return false;
		}

		for(int i = 0 ; i < 9 ; i+=3)
		{
			for(int j = 0 ; j < 9; j+=3)
			{
				if(!checkGrid(i,j))
					return false;
			}
		}
		return true;
	}
}

bool SudokuBoard::checkRow(int row)
{
	bool found[10] = {false};

	for(int i = 0 ; i < 9 ; i++)//check duplicate
	{
		if(found[m_board[row][i].m_i_value] == false)
			found[m_board[row][i].m_i_value] = true;
		else if(found[m_board[row][i].m_i_value] == true)//duplicated
			return false;
	}

	for(int i = 1 ; i<= 9 ; i++)//check value range
	{
		if(found[i] == false)
			return false;
	}

	return true;
}

bool SudokuBoard::checkColumn(int col)
{
	bool found[10] = {false};

	for(int i = 0 ; i < 9 ; i++)//check duplicate
	{
		if(found[m_board[i][col].m_i_value] == false)
			found[m_board[i][col].m_i_value] = true;
		else if(found[m_board[i][col].m_i_value] == true)//duplicated
			return false;
	}

	for(int i = 1 ; i<= 9 ; i++)//check value range
	{
		if(found[i] == false)
			return false;
	}

	return true;
}

bool SudokuBoard::checkGrid(int pX,int pY)//top left x,y
{
	bool found[10] = {false};

	for(int i = pX ; i < pX + 3 ; i++)//check duplicate
		for(int j = pY; j < pY + 3 ; j++)
		{
			if(found[m_board[i][j].m_i_value] == false)
				found[m_board[i][j].m_i_value] = true;
			else if(found[m_board[i][j].m_i_value] == true)//duplicated
				return false;
		}

	for(int i = 1 ; i<= 9 ; i++)//check value range
	{
		if(found[i] == false)
			return false;
	}

	return true;
}

void SudokuBoard::scanCandidatesAt(int pX, int pY)
{
	bool found[10] = {false};

	if(m_board[pX][pY].m_i_value != 0)
		return;

	else
	{
		m_board[pX][pY].m_candidate_list.clear();

		for(int i = 0 ; i < 9 ; i++)//search row
		{
			if(m_board[i][pY].m_i_value != 0)
				found[m_board[i][pY].m_i_value] = true;
		}
		for(int i = 0 ; i < 9 ; i++)//search column
		{
			if(m_board[pX][i].m_i_value != 0)
				found[m_board[pX][i].m_i_value] = true;
		}
		for(int i = (pX/3)*3 ; i < (pX/3)*3 + 3 ; i++)//search grid
			for(int j = (pY/3)*3 ; j < (pY/3)*3 + 3; j++)
			{
				if(m_board[i][j].m_i_value != 0)
					found[m_board[i][j].m_i_value] = true;
			}
		
		for(int i = 1; i <= 9 ; i++)//candidates
		{
			if(found[i] == false)
			{
				m_board[pX][pY].m_candidate_list.push_back(i);
				m_i_total_candidates++;
			}
		}
	}
}

void SudokuBoard::scanAllCandidates()
{
	printf("scan candidate\n");
	m_i_total_candidates = 0;
	for(int i = 0 ; i < 9 ; i++)
	{
		for(int j = 0; j < 9 ; j++)
		{
			scanCandidatesAt(i,j);
			//m_board[i][j].showCandidates();
		}
	}
	printf("total candidate %d\n",m_i_total_candidates);
}