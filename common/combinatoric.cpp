#include "combinatoric.h"

#include <algorithm>

CombinatoricIterator::CombinatoricIterator(size_t n, size_t k) : n(n), k(k) {
	indices.resize(k);
	for (size_t i = 0; i < k; ++i) {
		indices[i] = i;
	}
}

CombinatoricIterator::~CombinatoricIterator() {
}

CombinatoricIterator CombinatoricIterator::Composition(size_t v, size_t n) {
	CombinatoricIterator result(v-1, n-1);
	result.indices.push_back(0);
	return result;
}

void CombinatoricIterator::set(size_t n, size_t k) {
	this->n = n;
	this->k = k;
	indices.resize(k);
	for (size_t i = 0; i < k; ++i) {
		indices[i] = i;
	}
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
	return indices.empty() or indices[0] >= n;
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
	for (auto i = indices.begin(); i != indices.end(); i++) {
		(*i)++;
	}
	if (indices.empty() or indices[0] > n-k) {
		indices[0] = n;
		return false;
	}
	return true;
}

bool CombinatoricIterator::nextComb(size_t g) {
	return next_combination(indices.begin(), indices.begin()+k, n, g);
}

bool CombinatoricIterator::nextPerm() {
	if (std::next_permutation(indices.begin(), indices.begin()+k)) {
		return true;
	}

	std::sort(indices.begin(), indices.begin()+k);
	return nextComb();
}

PartitionIterator::PartitionIterator(size_t n, std::vector<std::array<size_t, 2> > bounds) : bounds(bounds) {
	/*size_t remainder = 0;
	for (size_t i = this->bounds.size(); i > 0; i--) {
		if (this->bounds[i][1] > n-remainder) {
			this->bounds[i][1] = n-remainder;
		}
		if (this->bounds[i][0] >= this->bounds[1]) {
			return;
		}
		remainder += this->bounds[i][0];
	}*/

	int remaining = n;
	for (const auto &bound : bounds) {
		assign.push_back(CombinatoricIterator(remaining, bound[0]));
		remaining -= bound[0];
		if (remaining < 0) {
			assign[0].indices[0] = n;
			return;
		}
	}
}

PartitionIterator::~PartitionIterator() {
}

bool PartitionIterator::done() const {
	return assign.empty() or assign[0].done();
}

std::vector<std::vector<size_t> > PartitionIterator::get() const {
	std::vector<std::vector<size_t> > result(assign.size());
	if (done()) {
		return result;
	}

	std::vector<size_t> remainder(assign[0].n);
	for (size_t i = 0; i < assign[0].n; i++) {
		remainder[i] = i;
	}

	result[0] = assign[0].get();
	for (size_t j = result[0].size(); j > 0; j--) {
		remainder.erase(remainder.begin()+result[0][j-1]);
	}

	for (size_t i = 1; i < assign.size(); i++) {
		std::vector<size_t> index = assign[i].get();
		for (size_t j = 0; j < index.size(); j++) {
			result[i].push_back(remainder[index[j]]);
		}
		for (size_t j = index.size(); j > 0; j--) {
			remainder.erase(remainder.begin()+index[j-1]);
		}
	}
	return result;
}

bool PartitionIterator::nextPart() {
	for (size_t i = assign.size(); i > 0; i--) {
		assign[i-1].nextComb();

		if (assign[i-1].done()) {
			if ((bounds[i-1][1] <= bounds[i-1][0] or assign[i-1].k+1 < bounds[i-1][1]) and assign[i-1].k+1 <= assign[i-1].n) {
				assign[i-1].set(assign[i-1].n, assign[i-1].k+1);
			}
		}

		int remaining = assign[i-1].n;
		remaining -= assign[i-1].k;
		if (remaining < 0) {
			assign[i-1].indices[0] = assign[i-1].n;
		} else {
			for (size_t j = i; j < assign.size(); j++) {
				assign[j].set(remaining, bounds[j][0]);
				remaining -= assign[j].k;
				if (remaining < 0) {
					assign[i-1].indices[0] = assign[i-1].n;
					break;
				}
			}
		}

		if (not assign[i-1].done()) {
			return true;
		}
	}

	return false;
}

