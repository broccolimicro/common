#pragma once

#include <vector>

struct CombinatoricIterator {
	std::vector<int> indices;
	int n, k;

	CombinatoricIterator(int n, int k);
	~CombinatoricIterator();

	std::vector<int>::iterator begin();
	std::vector<int>::iterator end();

	bool nextShift();
	bool nextComb();
	bool nextPerm();
};


