#include "net.h"

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

Net::Net() {
	this->region = 0;
}

Net::Net(vector<Field> fields, int region) {
	this->fields = fields;
	this->region = region;
}

Net::Net(const char *name, int region) : Net(string(name), region) {
}

Net::Net(string name, int region) {
	if (not name.empty()) {
		size_t prev = 0u;
		size_t dot = name.find('.', prev);
		while (dot != string::npos and dot < name.size()) {
			fields.push_back(name.substr(prev, dot-prev));
			prev = dot+1;
			dot = name.find('.', prev);
		}
		fields.push_back(name.substr(prev));
	}
	this->region = region;
}

Net::~Net() {
}

string Net::to_string() const {
	string result = "";
	for (int i = 0; i < (int)fields.size(); i++) {
		if (i != 0) {
			result += ".";
		}

		result += fields[i].to_string();
	}

	if (region != 0) {
		result += "'" + std::to_string(region);
	}

	return result;
}

const char *Net::c_str() const {
	return to_string().c_str();
}

bool Net::empty() const {
	return fields.empty();
}

Net Net::prefix(string prefix) const {
	Net result = *this;
	if (not result.fields.empty()) {
		result.fields.back().name = prefix + result.fields.back().name;
	}
	return result;
}

Net Net::postfix(string postfix) const {
	Net result = *this;
	if (not result.fields.empty()) {
		result.fields.back().name += postfix;
	}
	return result;
}

ostream &operator<<(ostream &os, const Net &net) {
	os << net.to_string();
	return os;
}

bool operator<(const Net &v0, const Net &v1) {
	return (v0.fields < v1.fields) ||
		   (v0.fields == v1.fields && v0.region < v1.region);
}

bool operator>(const Net &v0, const Net &v1) {
	return (v0.fields > v1.fields) ||
		   (v0.fields == v1.fields && v0.region > v1.region);
}

bool operator<=(const Net &v0, const Net &v1) {
	return (v0.fields < v1.fields) ||
		   (v0.fields == v1.fields && v0.region <= v1.region);
}

bool operator>=(const Net &v0, const Net &v1) {
	return (v0.fields > v1.fields) ||
		   (v0.fields == v1.fields && v0.region >= v1.region);
}

bool operator==(const Net &v0, const Net &v1) {
	return (v0.fields == v1.fields && v0.region == v1.region);
}

bool operator!=(const Net &v0, const Net &v1) {
	return (v0.fields != v1.fields || v0.region != v1.region);
}

}
