#pragma once
#include <vector>

using namespace std;

class Cell
{
public:
	Cell(void);
	Cell(int,int,int);
	//~Cell(void);
	int m_i_posX;
	int m_i_posY;
	int m_i_value;
	bool m_b_isBlank;
	void showCandidates();
	vector<int> m_candidate_list;
};

