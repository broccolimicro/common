#pragma once

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

template <typename T>
map<T, int> count_elements(const vector<T> &v1, map<T, int> &result = map<T, int>())
{
	for (typename vector<T>::const_iterator i = v1.begin(); i != v1.end(); i++)
	{
		typename map<T, int>::iterator it = result.find(*i);
		if (it != result.end())
			it->second++;
		else
			result.insert(pair<T, int>(*i, 1));
	}

	return result;
}

template <typename T>
int vector_intersection_size(const vector<T> &v1, const vector<T> &v2)
{
	int result = 0;
	typename vector<T>::const_iterator i, j;
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

template <typename T>
bool vector_intersects(const vector<T> &v1, const vector<T> &v2)
{
	typename vector<T>::const_iterator i, j;
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

template <typename T>
int vector_intersection_size(const vector<T> &v1, const vector<T> &v2, const vector<T> &v3)
{
	int result = 0;
	typename vector<T>::const_iterator i, j, k;
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

template <typename T>
vector<T> vector_intersection(const vector<T> &v1, const vector<T> &v2)
{
	vector<T> result;
	typename vector<T>::const_iterator i, j;
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

template <typename T>
vector<T> vector_intersection(const vector<vector<T> > &v)
{
	vector<T> result;
	vector<int> i(v.size(), 0);

	bool done = false;
	while (!done)
	{
		vector<int> ind(1, 0);
		T value = v[0][i[0]];
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

template <typename T>
void vector_symmetric_compliment(vector<T> &v1, vector<T> &v2)
{
	typename vector<T>::iterator i, j;
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

template <typename T>
vector<T> vector_difference(const vector<T> &v1, const vector<T> &v2)
{
	vector<T> result;
	typename vector<T>::const_iterator i, j;
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

template <typename T>
ostream &operator<<(ostream &os, const vector<T> &s)
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

template <typename T>
ostream &operator<<(ostream &os, const list<T> &s)
{
	os << "{";
	for (auto i = s.begin(); i != s.end(); i++)
	{
		if (i != s.begin())
			os << ", ";
		os << *i;
	}
	os << "}";
	return os;
}

template <typename T1, typename T2>
ostream &operator<<(ostream &os, const map<T1, T2> &s)
{
	os << "{";
	for (auto i = s.begin(); i != s.end(); i++)
	{
		if (i != s.begin())
			os << ", ";
		os << i->first << "=>" << i->second;
	}
	os << "}";
	return os;
}

template <typename T1, typename T2>
ostream &operator<<(ostream &os, const pair<T1, T2> &s)
{
	os << "(" << s.first << ", " << s.second << ")";
	return os;
}

