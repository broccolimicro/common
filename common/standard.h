/*

 * standard.h
 *
 * Common is a collection of functions not specific to the compiler that
 * we found useful to define. Note that our #defines for user flags are
 * also stored in common.h as it is accessed by everyone.
 */

#ifndef common_standard_h
#define common_standard_h

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <sstream>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <vector>
#include <set>
#include <list>
#include <map>
#include <string>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <iterator>
#include <deque>
#include <limits>
#include "hash.h"

using namespace std;

template <typename type>
map<type, int> count_elements(const vector<type> &v1, map<type, int> &result = map<type, int>())
{
	for (typename vector<type>::const_iterator i = v1.begin(); i != v1.end(); i++)
	{
		typename map<type, int>::iterator it = result.find(*i);
		if (it != result.end())
			it->second++;
		else
			result.insert(pair<type, int>(*i, 1));
	}

	return result;
}

template <typename type>
int vector_intersection_size(const vector<type> &v1, const vector<type> &v2)
{
	int result = 0;
	typename vector<type>::const_iterator i, j;
	for (i = v1.begin(), j = v2.begin(); i != v1.end() && j != v2.end();)
	{
		if (*j > *i)
			i++;
		else if (*i > *j)
			j++;
		else
		{
			result++;
			i++;
			j++;
		}
	}

	return result;
}

template <typename type>
bool vector_intersects(const vector<type> &v1, const vector<type> &v2)
{
	typename vector<type>::const_iterator i, j;
	for (i = v1.begin(), j = v2.begin(); i != v1.end() && j != v2.end();)
	{
		if (*j > *i)
			i++;
		else if (*i > *j)
			j++;
		else
			return true;
	}

	return false;
}

template <typename type>
int vector_intersection_size(const vector<type> &v1, const vector<type> &v2, const vector<type> &v3)
{
	int result = 0;
	typename vector<type>::const_iterator i, j, k;
	for (i = v1.begin(), j = v2.begin(), k = v3.begin(); i != v1.end() && j != v2.end() && k != v3.end();)
	{
		if ((*i <= *j && *i < *k) || (*i < *j && *i <= *k))
			i++;
		else if ((*j <= *i && *j < *k) || (*j < *i && *j <= *k))
			j++;
		else if ((*k <= *j && *k < *i) || (*k < *j && *k <= *i))
			k++;
		else
		{
			result++;
			i++;
			j++;
			k++;
		}
	}

	return result;
}

template <typename type>
vector<type> vector_intersection(const vector<type> &v1, const vector<type> &v2)
{
	vector<type> result;
	typename vector<type>::const_iterator i, j;
	for (i = v1.begin(), j = v2.begin(); i != v1.end() && j != v2.end();)
	{
		if (*j > *i)
			i++;
		else if (*i > *j)
			j++;
		else
		{
			result.push_back(*i);
			i++;
			j++;
		}
	}

	return result;
}

template <typename type>
vector<type> vector_intersection(const vector<vector<type> > &v)
{
	vector<type> result;
	vector<int> i(v.size(), 0);

	bool done = false;
	while (!done)
	{
		vector<int> ind(1, 0);
		type value = v[0][i[0]];
		for (int j = 1; j < (int)v.size(); j++)
		{
			if (v[j][i[j]] < value)
			{
				value = v[j][i[j]];
				ind.clear();
				ind.push_back(j);
			}
			else if (v[j][i[j]] == value)
				ind.push_back(j);
		}

		if (ind.size() == v.size())
			result.push_back(value);

		for (int j = 0; j < (int)ind.size(); j++)
		{
			i[ind[j]]++;
			if (i[ind[j]] >= (int)v[ind[j]].size())
				done = true;
		}
	}

	return result;
}

template <typename type>
void vector_symmetric_compliment(vector<type> &v1, vector<type> &v2)
{
	typename vector<type>::iterator i, j;
	for (i = v1.begin(), j = v2.begin(); i != v1.end() && j != v2.end();)
	{
		if (*j > *i)
			i++;
		else if (*i > *j)
			j++;
		else
		{
			i = v1.erase(i);
			j = v2.erase(j);
		}
	}
}

template <typename type>
vector<type> vector_difference(const vector<type> &v1, const vector<type> &v2)
{
	vector<type> result;
	typename vector<type>::const_iterator i, j;
	for (i = v1.begin(), j = v2.begin(); i != v1.end() || j != v2.end();)
	{
		if (j == v2.end() || (i != v1.end() && *j > *i))
		{
			result.push_back(*i);
			i++;
		}
		else if (i == v1.end() || (j != v2.end() && *i > *j))
			j++;
		else
		{
			i++;
			j++;
		}
	}
	return result;
}

template <typename type>
ostream &operator<<(ostream &os, vector<type> s)
{
	os << "{";
	for (size_t i = 0; i < s.size(); i++)
	{
		if (i != 0)
			os << ", ";
		os << s[i];
	}
	os << "}";
	return os;
}

template <typename type>
ostream &operator<<(ostream &os, list<type> s)
{
	os << "{";
	for (typename list<type>::iterator i = s.begin(); i != s.end(); i++)
	{
		if (i != s.begin())
			os << ", ";
		os << *i;
	}
	os << "}";
	return os;
}

template <typename type1, typename type2>
ostream &operator<<(ostream &os, map<type1, type2> s)
{
	os << "{";
	for (typename map<type1, type2>::iterator i = s.begin(); i != s.end(); i++)
	{
		if (i != s.begin())
			os << ", ";
		os << i->first << "=>" << i->second;
	}
	os << "}";
	return os;
}

template <typename type1, typename type2>
ostream &operator<<(ostream &os, pair<type1, type2> s)
{
	os << "(" << s.first << ", " << s.second << ")";
	return os;
}

#endif
