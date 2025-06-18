#pragma once

#include <vector>
#include <string>
#include <map>
#include <utility>

using namespace std;

struct MockNetlist {
	vector<pair<string, int> > nets;
	
	int netIndex(string name) const;
	int netIndex(string name, bool define);
	string netAt(int uid) const;
	int netCount() const;
	void clear();
};

