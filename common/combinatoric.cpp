#include "combinatoric.h"

#include <algorithm>

CombinatoricIterator::CombinatoricIterator(size_t n, size_t k) : n(n), k(k) {
	indices.resize(k);
	for (size_t i = 0; i < k; ++i) {
		indices[i] = i;
	}
	valid = n >= k;
}

CombinatoricIterator::~CombinatoricIterator() {
}

CombinatoricIterator CombinatoricIterator::Composition(size_t v, size_t n) {
	return CombinatoricIterator(v-1, n-1);
}

std::vector<size_t>::const_iterator CombinatoricIterator::begin() const {
	return indices.begin();
}

std::vector<size_t>::const_iterator CombinatoricIterator::end() const {
	return indices.begin()+k;
}

size_t CombinatoricIterator::size() const {
	return k;
}

bool CombinatoricIterator::done() const {
	return not valid;
}

size_t CombinatoricIterator::operator[](size_t i) const {
	return indices[i];
}

std::vector<size_t> CombinatoricIterator::get(bool sorted) const {
	std::vector<size_t> result(indices.begin(), indices.begin() + k);
	if (sorted) {
		sort(result.begin(), result.end());
	}
	return result;
}

std::vector<size_t> CombinatoricIterator::getComposition() const {
	std::vector<size_t> result(k+1);
	if (k > 0) {
		result[0] = indices[0] + 1;
		for (size_t i = 1; i < k; ++i) {
			result[i] = indices[i] - indices[i-1];
		}
		result[k] = n-indices[k-1];
	} else {
		result[0] = n+1;
	}
	return result;
}

bool CombinatoricIterator::nextShift() {
	if (indices[0] >= n-k) {
		valid = false;
		return false;
	}
	
	for (auto i = indices.begin(); i != indices.end(); i++) {
		(*i)++;
	}
	return true;
}

bool CombinatoricIterator::nextComb() {
	if (k == 0 or indices[0] >= n-k) {
		valid = false;
		return false;
	}

	// Find the rightmost index that can be incremented
	for (int i = (int)k - 1; i >= 0; --i) {
		if (indices[i] < n - k + i) {
			++indices[i];
			for (size_t j = i + 1; j < k; ++j) {
				indices[j] = indices[j - 1] + 1;
			}
			return true;
		}
	}

	valid = false;
	return false;
}

bool CombinatoricIterator::nextPerm() {
	if (std::next_permutation(indices.begin(), indices.end())) {
		return true;
	}

	std::sort(indices.begin(), indices.end());
	return nextComb();
}

