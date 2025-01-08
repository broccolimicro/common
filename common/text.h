#pragma once

#include "standard.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"

bool ac(char c);
bool nc(char c);
bool oc(char c);
bool sc(char c);

string to_minstring(float value);
string to_minstring(double value);

template <class type>
string to_string(type value)
{
	ostringstream os;
	os << value;
	return os.str();
}

template <typename type>
string to_string(const vector<type> &s)
{
	string result = "{";
	for (size_t i = 0; i < s.size(); i++)
	{
		if (i != 0)
			result += ", ";
		result += to_string(s[i]);
	}
	result += "}";
	return result;
}

template <typename type>
string to_string(const list<type> &s)
{
	string result = "{";
	for (auto i = s.begin(); i != s.end(); i++)
	{
		if (i != s.begin())
			result += ", ";
		result += to_string(*i);
	}
	result += "}";
	return result;
}

template <typename type>
string to_string(const set<type> &s)
{
	string result = "{";
	for (auto i = s.begin(); i != s.end(); i++)
	{
		if (i != s.begin())
			result += ", ";
		result += to_string(*i);
	}
	result += "}";
	return result;
}

template <typename type1, typename type2>
string to_string(const map<type1, type2> &s)
{
	string result = "{";
	for (auto i = s.begin(); i != s.end(); i++)
	{
		if (i != s.begin())
			result += ", ";
		result += to_string(i->first) + "=>" + to_string(i->second);
	}
	result += "}";
	return result;
}

template <typename type1, typename type2>
string to_string(const pair<type1, type2> &s)
{
	return "(" + to_string(s.first) + ", " + to_string(s.second) + ")";
}

string lower(string str);
int edit_distance(const string &s1, const string &s2);
int get_column_end(string line, int column_start, string *space = NULL);
string line_wrap(string str, int length);

size_t findFirstUnescapedSpace(const std::string& str);
std::string extractPath(const std::string& input);
std::string escapePath(const std::string &input);

