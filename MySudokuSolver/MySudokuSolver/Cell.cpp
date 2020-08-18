#include "Cell.h"

Cell::Cell(void)
{
	m_i_value = -1;
	m_i_posX = -1;
	m_i_posY = -1;
	m_b_isBlank = false;
}

Cell::Cell(int pValue, int pX, int pY)
{
	m_i_value = pValue;
	m_i_posX = pX;
	m_i_posY = pY;
	if(pValue == 0)
		m_b_isBlank = true;
	else 
		m_b_isBlank = false;
}

void Cell::showCandidates()
{
	if(m_i_value != 0) return;

	vector<int>::iterator it;
	printf("Candidates at %d %d: ", m_i_posX, m_i_posY);
	for(it = m_candidate_list.begin() ; it != m_candidate_list.end() ; ++it)
	{
		printf("%d ", *it);
	}
	printf("\n");
}

//Cell::~Cell(void)
//{
//}
