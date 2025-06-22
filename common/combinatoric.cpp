#include "combinatoric.h"

#include <algorithm>

CombinatoricIterator::CombinatoricIterator(int n, int k) : n(n), k(k) {
	indices.resize(k);
	for (int i = 0; i < k; ++i) {
		indices[i] = i;
	}
}

CombinatoricIterator::~CombinatoricIterator() {
}

std::vector<int>::iterator CombinatoricIterator::begin() {
	return indices.begin();
}

std::vector<int>::iterator CombinatoricIterator::end() {
	return indices.begin()+k;
}

bool CombinatoricIterator::nextShift() {
	if (indices[0] >= n-k) {
		return false;
	}
	
	for (auto i = indices.begin(); i != indices.end(); i++) {
		(*i)++;
	}
	return true;
}

bool CombinatoricIterator::nextComb() {
	if (indices[0] >= n-k) {
		return false;
	}

	// Find the rightmost index that can be incremented
	for (int i = k - 1; i >= 0; --i) {
		if (indices[i] < n - k + i) {
			++indices[i];
			for (int j = i + 1; j < k; ++j) {
				indices[j] = indices[j - 1] + 1;
			}
			return true;
		}
	}

	return false;
}

bool CombinatoricIterator::nextPerm() {
	if (std::next_permutation(indices.begin(), indices.end())) {
		return true;
	}

	std::sort(indices.begin(), indices.end());
	return nextComb();
}

