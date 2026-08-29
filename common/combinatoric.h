#pragma once

#include <vector>
#include <array>
#include <limits>

template<typename Iterator>
bool next_combination(Iterator first, Iterator last, size_t n, size_t g = std::numeric_limits<size_t>::max()) {
	size_t k = std::distance(first, last);
	if (g > k-1) {
		g = k-1;
	}
	for (int i = (int)g; i >= 0; --i) {
		if (*(first+i) < n - k + i) {
			++*(first+i);
			for (size_t j = i + 1; j < k; ++j) {
				*(first+j) = *(first+(j-1)) + 1;
			}
			return true;
		}
	}
	*first = n;
	return false;
}

struct CombinatoricIterator {
	std::vector<size_t> indices;
	size_t n, k;

	CombinatoricIterator(size_t n, size_t k);
	~CombinatoricIterator();

	static CombinatoricIterator Composition(size_t v, size_t n);

	void set(size_t n, size_t k);
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
	bool nextComb(size_t g = std::numeric_limits<size_t>::max());

	bool nextPerm();
};

struct PartitionIterator {
	struct Partition {
		size_t index;
		std::vector<std::vector<int> > part;
	};

	bool allRequired;
	std::vector<int> elems;
	std::vector<Partition> stack;
	std::vector<size_t> lo;
	std::vector<size_t> hi;

	PartitionIterator(std::vector<int> elems, std::vector<size_t> lo, std::vector<size_t> hi, bool allRequired=false);
	~PartitionIterator();

	bool step(bool pop);

	bool done() const;

	std::vector<std::vector<int> > get() const;

	bool nextPart();
};

std::vector<std::vector<std::vector<int> > > allPartitions(std::vector<int> elems, std::vector<size_t> lo, std::vector<size_t> hi, bool allRequired=false);
