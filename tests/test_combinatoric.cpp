#include <common/combinatoric.h>
#include <common/standard.h>
#include <common/text.h>
#include <gtest/gtest.h>

// Helper: factorial
static size_t factorial(size_t k) {
	size_t result = 1;
	for (size_t i = 2; i <= k; i++) result *= i;
	return result;
}

// Helper: n choose k
static size_t nChooseK(size_t n, size_t k) {
	if (k > n) return 0;
	size_t res = 1;
	for (size_t i = 1; i <= k; ++i) {
		res = res * (n - k + i) / i;
	}
	return res;
}

TEST(CombinatoricIteratorTest, NextShiftFullEnumeration) {
	size_t n = 7, k = 3;
	size_t count = 0;
	for (CombinatoricIterator it(n, k); not it.done(); it.nextShift()) {
		cout << ::to_string(it.indices) << endl;
		EXPECT_EQ(it[0], count);
		EXPECT_EQ(it[it.size()-1], count + k - 1);
		count++;
	}

	// Expect n-k+1 results
	EXPECT_EQ(count, n - k + 1);
}

TEST(CombinatoricIteratorTest, NextCombFullEnumeration) {
	size_t n = 7, k = 3;
	size_t count = 0;
	for (CombinatoricIterator it(n, k); not it.done(); it.nextComb()) {
		cout << ::to_string(it.indices) << endl;
		for (size_t i = 1; i < it.size(); i++) {
			EXPECT_GT(it[i], it[i-1]);
		}
		count++;
	}

	EXPECT_EQ(count, nChooseK(n, k));
}

TEST(CombinatoricIteratorTest, NextPermFullEnumeration) {
	size_t n = 5, k = 3;
	size_t count = 0;
	for (CombinatoricIterator it(n, k); not it.done(); it.nextPerm()) {
		cout << ::to_string(it.indices) << endl;
		vector<size_t> perm = it.indices;
		sort(perm.begin(), perm.end());
		for (size_t i = 1; i < perm.size(); i++) {
			EXPECT_GT(perm[i], perm[i-1]);
		}
		count++;
	}
	EXPECT_EQ(count, nChooseK(n, k) * factorial(k));
}

