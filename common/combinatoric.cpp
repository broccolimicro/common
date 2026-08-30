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
	if (indices.empty() or k > n) {
		return false;
	}

	for (auto i = indices.begin(); i != indices.end(); ++i) {
		++*i;
	}

	if (indices[0] > n - k) {
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

LatticeIterator::LatticeIterator(std::vector<size_t> width) : width(width) {
	indices.resize(width.size(), 0);
}

LatticeIterator::~LatticeIterator() {
}

void LatticeIterator::set(std::vector<size_t> width) {
	this->width = width;
	indices.resize(width.size(), 0);
}

std::vector<size_t>::const_iterator LatticeIterator::begin() const {
	return indices.begin();
}

std::vector<size_t>::const_iterator LatticeIterator::end() const {
	return indices.end();
}

size_t LatticeIterator::size() const {
	size_t result = 1;
	for (size_t w : width) {
		result *= w;
	}
	return result;
}

bool LatticeIterator::done() const {
	return indices.empty() or indices[0] >= width[0];
}

size_t LatticeIterator::operator[](size_t i) const {
	return indices[i];
}

const std::vector<size_t> &LatticeIterator::get() const {
	return indices;
}

bool LatticeIterator::next() {
	if (indices.empty() or indices[0] >= width[0]) {
		return false;
	}

	indices.back()++;
	for (size_t i = indices.size()-1; i > 0; i--) {
		if (indices[i] < width[i]) {
			break;
		}
		indices[i] = 0;
		indices[i-1]++;
	}

	return indices[0] < width[0];
}

PartitionIterator::PartitionIterator(size_t n, std::vector<size_t> lo, std::vector<size_t> hi, bool allRequired) {
	this->allRequired = allRequired;
	this->n = n;
	this->lo = lo;
	this->hi = hi;

	stack.push_back(Partition());
	stack.back().index = 0;
	for (size_t i = 0, j = std::max(lo.size(), hi.size()); i < j; i++) {
		stack.back().part.push_back({});
	}
	step(false);
}

PartitionIterator::~PartitionIterator() {
}

bool PartitionIterator::step(bool pop) {
	bool first = true;
	while (not stack.empty()) {
		if (not pop or not first) {
			bool found = true;
			for (size_t i = 0, j = std::min(lo.size(), stack.back().part.size()); i < j; i++) {
				if (stack.back().part[i].size() < lo[i]) {
					found = false;
					break;
				}
			}
			if (found and (not allRequired or stack.back().index >= n)) {
				return true;
			}
		}
		first = false;

		Partition curr = std::move(stack.back());
		stack.pop_back();
		if (curr.index >= n) {
			continue;
		}

		size_t index = curr.index;
		++curr.index;
		for (size_t i = 0; i < curr.part.size(); i++) {
			if (i >= hi.size() or curr.part[i].size() < hi[i] or hi[i] == 0) {
				stack.push_back(curr);
				stack.back().part[i].push_back(index);
			}
		}
		if (not allRequired) {
			stack.push_back(std::move(curr));
		}
	}

	return false;
}

bool PartitionIterator::done() const {
	return stack.empty();
}

const std::vector<std::vector<size_t> > &PartitionIterator::get() const {
	return stack.back().part;
}

bool PartitionIterator::nextPart() {
	return step(true);
}

std::vector<std::vector<std::vector<size_t> > > allPartitions(size_t n, std::vector<size_t> lo, std::vector<size_t> hi, bool allRequired) {
	struct Partition {
		size_t index;
		std::vector<std::vector<size_t> > part;
	};
	std::vector<Partition> stack;
	stack.push_back(Partition());
	stack.back().index = 0;
	for (size_t i = 0, j = std::max(lo.size(), hi.size()); i < j; i++) {
		stack.back().part.push_back({});
	}

	std::vector<std::vector<std::vector<size_t> > > result;
	while (not stack.empty()) {
		Partition curr = std::move(stack.back());
		stack.pop_back();

		bool found = true;
		for (size_t i = 0, j = std::min(lo.size(), curr.part.size()); i < j; i++) {
			if (curr.part[i].size() < lo[i]) {
				found = false;
				break;
			}
		}
		if (found and (not allRequired or curr.index >= n)) {
			result.push_back(curr.part);
		}
		if (curr.index >= n) {
			continue;
		}

		size_t index = curr.index;
		++curr.index;
		for (size_t i = 0; i < curr.part.size(); i++) {
			if (i >= hi.size() or curr.part[i].size() < hi[i] or hi[i] == 0) {
				stack.push_back(curr);
				stack.back().part[i].push_back(index);
			}
		}
		if (not allRequired) {
			stack.push_back(std::move(curr));
		}
	}

	return result;
}

