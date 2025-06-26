/*
 * text.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: nbingham
 */

#include "text.h"

/** Returns true if 'c' has a value that is legal for the first character in a name.
 */
bool ac(char c)
{
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') );
}


/** Returns true if 'c' has a value that is legal in a type or variable name.
 */
bool nc(char c)
{
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			(c == '_') || (c == '.'));
}

/** Returns true if 'c' is an operator character.
 */
bool oc(char c)
{
	return (c == ':' ||
			c == '=' ||
			c == '|' ||
			c == '&' ||
			c == '~' ||
			c == '>' ||
			c == '<' ||
			c == ';' ||
			c == '*' ||
			c == '[' ||
			c == ']' ||
			c == '(' ||
			c == ')' ||
			c == '{' ||
			c == '}' ||
			c == '+' ||
			c == '-' ||
			c == '!' ||
			c == '?' ||
			c == '@' ||
			c == '#' ||
			c == '/');
}

/** Returns true if 'c' is a whitespace character
 */
bool sc(char c)
{
	return (c == ' '  ||
			c == '\t' ||
			c == '\n' ||
			c == '\r');
}

string to_minstring(float value)
{
	static char result[32];
	int len = snprintf(result, sizeof(result), "%g", value);
	if (len > 0 && static_cast<size_t>(len) < sizeof(result)) {
		return string(result);
	}
	return string();
}

string to_minstring(double value)
{
	static char result[64];
	int len = snprintf(result, sizeof(result), "%g", value);
	if (len > 0 && static_cast<size_t>(len) < sizeof(result)) {
		return string(result);
	}
	return string();
}

string lower(string str) {
	for (auto c = str.begin(); c != str.end(); c++) {
		*c = tolower(*c);
	}
	return str;
}

int edit_distance(const string &s1, const string &s2)
{
	vector<int> col((int)s2.size()+1);
	vector<int> prev_col((int)s2.size()+1);

	for (int i = 0; i < (int)prev_col.size(); i++)
		prev_col[i] = i;

	for (int i = 0; i < (int)s1.size(); i++)
	{
		col[0] = i+1;

		for (int j = 0; j < (int)s2.size(); j++)
			col[j+1] = min(min(prev_col[1 + j] + 1, col[j] + 1), prev_col[j] + (s1[i] == s2[j] ? 0 : 1));

		col.swap(prev_col);
	}

	return prev_col[s2.size()];
}

int get_column_end(string line, int column_start, string *space)
{
	string str;
	int column_end = 1;
	for (int j = 0; j < (int)line.size() && j < column_start; j++)
	{
		if (line[j] != '\t')
		{
			str.push_back(' ');
			column_end++;
		}
		else
		{
			str.push_back('\t');
			column_end+=8;
		}
	}

	if (space != NULL)
		*space = str;
	return column_end;
}

string line_wrap(string str, int length)
{
	string result;
	for (int i = 0; i < (int)str.size(); i += length)
	{
		result += str.substr(i, min((int)str.size(), i+length) - i);
		if (i+length < (int)str.size())
			result += "\n";
	}
	return result;
}

// Function to find the first unescaped space
size_t findFirstUnescapedSpace(const std::string& str) {
	bool inEscape = false;

	for (size_t i = 0; i < str.size(); ++i) {
		if (inEscape) {
			// If we are in an escape sequence, skip the current character
			inEscape = false;
		} else if (str[i] == '\\') {
			// If we encounter a backslash, set the escape flag
			inEscape = true;
		} else if (str[i] == ' ') {
			// If we encounter a space that is not escaped, return its position
			return i;
		}
	}

	// If no unescaped space is found, return string::npos
	return std::string::npos;
}

// Function to extract the file format from the path at the beginning of the arguments
std::string extractPath(const std::string& input) {
	// Find the first unescaped space, which ends the path
	size_t spacePos = findFirstUnescapedSpace(input);

	// If no space is found, consider the entire input to be a single argument (path)
	std::string path;
	if (spacePos != std::string::npos) {
		path = input.substr(0, spacePos);
	} else {
		path = input;
	}

	return path;
}

std::string escapePath(const std::string &input) {
	std::string result;
	for (auto c = input.begin(); c != input.end(); c++) {
		if (*c == ' ' or *c == '\\') {
			result.push_back('\\');
		}
		result.push_back(*c);
	}
	return result;
}

