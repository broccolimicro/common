#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace ucs {

struct Field {
	Field();
	Field(string name, vector<int> slice);
	Field(string field);
	~Field();

	// index in hierarchical data structure
	int index;

	// name of the field
	string name;
	vector<int> slice;

	string to_string() const;
};

bool operator<(const Field &i0, const Field &i1);
bool operator>(const Field &i0, const Field &i1);
bool operator<=(const Field &i0, const Field &i1);
bool operator>=(const Field &i0, const Field &i1);
bool operator==(const Field &i0, const Field &i1);
bool operator!=(const Field &i0, const Field &i1);

struct Variable {
	Variable();
	Variable(vector<Field> fields);
	Variable(string name);
	~Variable();

	vector<Field> fields;

	string to_string() const;
};

bool operator<(const Variable &v0, const Variable &v1);
bool operator>(const Variable &v0, const Variable &v1);
bool operator<=(const Variable &v0, const Variable &v1);
bool operator>=(const Variable &v0, const Variable &v1);
bool operator==(const Variable &v0, const Variable &v1);
bool operator!=(const Variable &v0, const Variable &v1);

}

