#include "variable.h"

using std::vector;
using std::string;

namespace ucs {

Field::Field() {
	this->index = -1;
}

Field::Field(string name, vector<int> slice) {
	this->index = -1;
	this->name = name;
	this->slice = slice;
}

Field::Field(string field) {
	this->index = -1;
	size_t open = name.find('[');
	name = field.substr(0u, open);
	while (open != string::npos and open < field.size()) {
		open += 1;
		size_t close = name.find(']', open);
		slice.push_back(atoi(name.substr(open, close-open).c_str()));
		open = close+1;
	}
}

Field::~Field() {
}

string Field::to_string() const {
	string result = name;
	for (int i = 0; i < (int)slice.size(); i++) {
		result += "[" + std::to_string(slice[i]) + "]";
	}
	return result;
}

bool operator<(const Field &i0, const Field &i1) {
	return (i0.name < i1.name) ||
		   (i0.name == i1.name && i0.slice < i1.slice);
}

bool operator>(const Field &i0, const Field &i1) {
	return (i0.name > i1.name) ||
		   (i0.name == i1.name && i0.slice > i1.slice);
}

bool operator<=(const Field &i0, const Field &i1) {
	return (i0.name < i1.name) ||
		   (i0.name == i1.name && i0.slice <= i1.slice);
}

bool operator>=(const Field &i0, const Field &i1) {
	return (i0.name > i1.name) ||
		   (i0.name == i1.name && i0.slice >= i1.slice);
}

bool operator==(const Field &i0, const Field &i1) {
	return i0.name == i1.name && i0.slice == i1.slice;
}

bool operator!=(const Field &i0, const Field &i1) {
	return i0.name != i1.name || i0.slice != i1.slice;
}

Variable::Variable() {
}

Variable::Variable(vector<Field> fields) {
	this->fields = fields;
}

Variable::Variable(string name) {
	size_t prev = 0u;
	size_t dot = name.find('.', prev);
	while (dot != string::npos and dot < name.size()) {
		fields.push_back(name.substr(prev, dot-prev));
		prev = dot+1;
		dot = name.find('.', prev);
	}
	fields.push_back(name.substr(prev));
}

Variable::~Variable() {
}

string Variable::to_string() const {
	string result = "";
	for (int i = 0; i < (int)fields.size(); i++) {
		if (i != 0) {
			result += ".";
		}

		result += fields[i].to_string();
	}

	return result;
}

bool operator<(const Variable &v0, const Variable &v1) {
	return v0.fields < v1.fields;
}

bool operator>(const Variable &v0, const Variable &v1) {
	return v0.fields > v1.fields;
}

bool operator<=(const Variable &v0, const Variable &v1) {
	return v0.fields <= v1.fields;
}

bool operator>=(const Variable &v0, const Variable &v1) {
	return v0.fields >= v1.fields;
}

bool operator==(const Variable &v0, const Variable &v1) {
	return v0.fields == v1.fields;
}

bool operator!=(const Variable &v0, const Variable &v1) {
	return v0.fields != v1.fields;
}

}
