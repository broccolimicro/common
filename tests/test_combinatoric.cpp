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

TEST(CombinatoricIteratorTest, CompositionFullEnumeration) {
	size_t v = 5, n = 3;
	size_t count = 0;

	vector<vector<size_t>> expected = {
		{1, 1, 3},
		{1, 2, 2},
		{1, 3, 1},
		{2, 1, 2},
		{2, 2, 1},
		{3, 1, 1}
	};

	for (CombinatoricIterator it = CombinatoricIterator::Composition(v, n);
		not it.done();
		it.nextComb()) {

		vector<size_t> composition = it.getComposition();

		cout << ::to_string(composition) << endl;

		EXPECT_EQ(composition.size(), n);

		size_t sum = 0;
		for (size_t value : composition) {
			EXPECT_GT(value, 0u);
			sum += value;
		}

		EXPECT_EQ(sum, v);
		EXPECT_EQ(composition, expected[count]);

		count++;
	}

	EXPECT_EQ(count, nChooseK(v - 1, n - 1));
}

TEST(CombinatoricIteratorTest, CompositionSingleElement) {
	size_t v = 7;
	CombinatoricIterator it = CombinatoricIterator::Composition(v, 1);

	ASSERT_FALSE(it.done());

	vector<size_t> composition = it.getComposition();

	ASSERT_EQ(composition.size(), 1u);
	EXPECT_EQ(composition[0], v);

	EXPECT_FALSE(it.nextComb());
	EXPECT_TRUE(it.done());
}

TEST(PartitionIteratorTest, FullEnumeration) {
	vector<int> elems = {0, 1, 2, 3, 4};
	vector<size_t> lo = {1, 1, 1};
	vector<size_t> hi = {2, 2, 2};

	PartitionIterator it(elems, lo, hi, true);

	size_t count = 0;

	while (not it.done()) {
		vector<vector<int>> partition = it.get();

		cout << ::to_string(partition) << endl;

		ASSERT_EQ(partition.size(), 3u);

		vector<bool> seen(elems.size(), false);

		for (size_t group = 0; group < partition.size(); ++group) {
			EXPECT_GE(partition[group].size(), lo[group]);
			EXPECT_LE(partition[group].size(), hi[group]);

			for (int element : partition[group]) {
				ASSERT_GE(element, 0);
				ASSERT_LT((size_t)element, elems.size());
				EXPECT_FALSE(seen[element]);
				seen[element] = true;
			}
		}

		for (bool elementSeen : seen) {
			EXPECT_TRUE(elementSeen);
		}

		++count;
		it.nextPart();
	}

	EXPECT_EQ(count, 90u);
}

/*TEST(PartitionIteratorTest, SingleGroup) {
	PartitionIterator it(5, 1);

	ASSERT_FALSE(it.done());

	vector<vector<size_t>> expected = {
		{0, 1, 2, 3, 4}
	};

	EXPECT_EQ(it.get(), expected);

	EXPECT_FALSE(it.nextPart());
	EXPECT_TRUE(it.done());
}

TEST(PartitionIteratorTest, EveryElementOwnGroup) {
	PartitionIterator it(4, 4);

	ASSERT_FALSE(it.done());

	vector<vector<size_t>> expected = {
		{0},
		{1},
		{2},
		{3}
	};

	EXPECT_EQ(it.get(), expected);

	size_t count = 1;
	while (it.nextPart()) {
		count++;
	}

	EXPECT_EQ(count, factorial(4));
	EXPECT_TRUE(it.done());
}

TEST(PartitionIteratorTest, InvalidPartitionSizes) {
	PartitionIterator zeroGroups(4, 0);
	EXPECT_TRUE(zeroGroups.done());

	PartitionIterator tooManyGroups(3, 4);
	EXPECT_TRUE(tooManyGroups.done());
}*/

TEST(PartitionTest, GroupSizeConstraints) {
	vector<int> elems = {0, 1, 2, 3};
	vector<size_t> lo = {1, 1};
	vector<size_t> hi = {2, 2};

	vector<vector<vector<int>>> result =
		allPartitions(elems, lo, hi, true);

	// All 4 elements must be used, and both groups must have
	// between 1 and 2 elements. Therefore each group has size 2.
	EXPECT_EQ(result.size(), 6u);

	for (const auto& partition : result) {
		ASSERT_EQ(partition.size(), 2u);

		EXPECT_EQ(partition[0].size(), 2u);
		EXPECT_EQ(partition[1].size(), 2u);

		vector<bool> seen(elems.size(), false);

		for (const auto& group : partition) {
			for (int elem : group) {
				ASSERT_GE(elem, 0);
				ASSERT_LT((size_t)elem, elems.size());
				EXPECT_FALSE(seen[elem]);
				seen[elem] = true;
			}
		}

		for (bool value : seen) {
			EXPECT_TRUE(value);
		}
	}
}

TEST(PartitionTest, OptionalElements) {
	vector<int> elems = {0, 1, 2};
	vector<size_t> lo = {1, 1};
	vector<size_t> hi = {2, 2};

	vector<vector<vector<int>>> result =
		allPartitions(elems, lo, hi, false);

	// Every returned partition must satisfy the group constraints.
	// Since elements are optional, the total number of assigned
	// elements can range from 2 through 3.
	ASSERT_FALSE(result.empty());

	for (const auto& partition : result) {
		ASSERT_EQ(partition.size(), 2u);

		EXPECT_GE(partition[0].size(), 1u);
		EXPECT_LE(partition[0].size(), 2u);
		EXPECT_GE(partition[1].size(), 1u);
		EXPECT_LE(partition[1].size(), 2u);

		size_t count = partition[0].size() + partition[1].size();
		EXPECT_GE(count, 2u);
		EXPECT_LE(count, 3u);

		vector<bool> seen(elems.size(), false);

		for (const auto& group : partition) {
			for (int elem : group) {
				ASSERT_GE(elem, 0);
				ASSERT_LT((size_t)elem, elems.size());
				EXPECT_FALSE(seen[elem]);
				seen[elem] = true;
			}
		}
	}
}

TEST(PartitionIteratorTest, MatchesAllPartitions) {
	vector<int> elems = {0, 1, 2, 3};
	vector<size_t> lo = {1, 1};
	vector<size_t> hi = {2, 2};

	for (bool allRequired : {false, true}) {
		vector<vector<vector<int>>> expected =
			allPartitions(elems, lo, hi, allRequired);

		PartitionIterator it(elems, lo, hi, allRequired);

		vector<vector<vector<int>>> actual;

		while (not it.done()) {
			actual.push_back(it.get());
			it.nextPart();
		}

		EXPECT_EQ(actual, expected);
	}
}
