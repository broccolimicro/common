#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "interface.h"

using std::vector;
using std::string;
using std::ostream;

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

struct Net {
	Net();
	Net(vector<Field> fields, int region=0);
	Net(const char *name, int region=0);
	Net(string name, int region=0);
	~Net();

	vector<Field> fields;
	int region;

	string to_string() const;
	const char *c_str() const;
	bool empty() const;

	Net prefix(string prefix) const;
	Net postfix(string postfix) const;
};

ostream &operator<<(ostream &os, const Net &net);

bool operator<(const Net &v0, const Net &v1);
bool operator>(const Net &v0, const Net &v1);
bool operator<=(const Net &v0, const Net &v1);
bool operator>=(const Net &v0, const Net &v1);
bool operator==(const Net &v0, const Net &v1);
bool operator!=(const Net &v0, const Net &v1);

_CONST_INTERFACE_ARG(ConstNetlist,
	(int, netIndex, (Net name) const),
	(Net, netAt, (int uid) const),
	(int, netCount, () const)
)

_INTERFACE_ARG(Netlist,
	(int, netIndex, (Net name, bool define)),
	(Net, netAt, (int uid) const),
	(int, netCount, () const)
)

}

