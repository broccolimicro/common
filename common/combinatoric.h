#pragma once

#include <vector>
#include <array>
#include <limits>

template<typename Iterator>
bool next_combination(Iterator first, Iterator last, size_t n, size_t g = std::numeric_limits<size_t>::max()) {
	size_t k = std::distance(first, last);
	if (g >= k) {
		g = k-1;
	}
	for (int i = (int)g; i >= 0; --i) {
		if (*(first+i) < n - k + i) {
			++*(first+i);
			for (size_t j = i + 1; j < k; ++j) {
				*(first+j) = *(first+j-1) + 1;
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

struct LatticeIterator {
	std::vector<size_t> indices;
	std::vector<size_t> width;

	LatticeIterator(std::vector<size_t> width);
	~LatticeIterator();

	void set(std::vector<size_t> width);
	std::vector<size_t>::const_iterator begin() const;
	std::vector<size_t>::const_iterator end() const;

	size_t size() const;
	bool done() const;

	size_t operator[](size_t i) const;

	const std::vector<size_t> &get() const;

	bool next();
};

struct PartitionIterator {
	struct Partition {
		size_t index;
		std::vector<std::vector<size_t> > part;
	};

	bool allRequired;
	size_t n;
	std::vector<Partition> stack;
	std::vector<size_t> lo;
	std::vector<size_t> hi;

	PartitionIterator(size_t n, std::vector<size_t> lo, std::vector<size_t> hi, bool allRequired=false);
	~PartitionIterator();

	bool step(bool pop);

	bool done() const;

	const std::vector<std::vector<size_t> > &get() const;

	bool nextPart();
};

std::vector<std::vector<std::vector<size_t> > > allPartitions(size_t n, std::vector<size_t> lo, std::vector<size_t> hi, bool allRequired=false);
