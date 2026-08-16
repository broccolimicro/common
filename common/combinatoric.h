#pragma once

#include <vector>

struct CombinatoricIterator {
	std::vector<size_t> indices;
	size_t n, k;
	bool valid;

	CombinatoricIterator(size_t n, size_t k);
	~CombinatoricIterator();

	static CombinatoricIterator Composition(size_t v, size_t n);

	std::vector<size_t>::const_iterator begin() const;
	std::vector<size_t>::const_iterator end() const;

	size_t size() const;
	bool done() const;

	size_t operator[](size_t i) const;

	std::vector<size_t> get(bool sorted=false) const;
	std::vector<size_t> getComposition() const;

	// resulting indices are ordered
	bool nextShift();

	// resulting indices are ordered
	// Use this for compositions as well
	bool nextComb();

	bool nextPerm();
};

struct PartitionIterator {
	std::vector<size_t> assign;
	size_t n, k;
	bool valid;

	PartitionIterator(size_t n, size_t k);
	~PartitionIterator();

	bool done() const;

	std::vector<std::vector<size_t>> get() const;

	bool nextPart();

	bool step();
	bool isValid() const;
};
