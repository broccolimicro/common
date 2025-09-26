#pragma once

#include <vector>

struct CombinatoricIterator {
	std::vector<size_t> indices;
	size_t n, k;
	bool valid;

	CombinatoricIterator(size_t n, size_t k);
	~CombinatoricIterator();

	std::vector<size_t>::const_iterator begin() const;
	std::vector<size_t>::const_iterator end() const;

	size_t size() const;
	bool done() const;

	size_t operator[](size_t i) const;

	// resulting indices are ordered
	bool nextShift();

	// resulting indices are ordered
	bool nextComb();


	bool nextPerm();
};


