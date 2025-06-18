#include "mock_netlist.h"

int MockNetlist::netIndex(string name) const {
	int region = 0;
	size_t tic = name.rfind('\'');
	if (tic != string::npos) {
		region = std::stoi(name.substr(tic+1));
		name = name.substr(0, tic);
	}

	for (int i = 0; i < (int)nets.size(); i++) {
		if (nets[i].first == name and nets[i].second == region) {
			return i;
		}
	}
	return -1;
}

int MockNetlist::netIndex(string name, bool define) {
	int region = 0;
	size_t tic = name.rfind('\'');
	if (tic != string::npos) {
		region = std::stoi(name.substr(tic+1));
		name = name.substr(0, tic);
	}

	bool found = false;
	for (int i = 0; i < (int)nets.size(); i++) {
		if (nets[i].first == name) {
			found = true;
			if (nets[i].second == region) {
				return i;
			}
		}
	}

	if (found or define) {
		nets.push_back({name, region});
		return (int)nets.size()-1;
	}
	return -1;
}

string MockNetlist::netAt(int uid) const {
	if (uid >= 0 && uid < (int)nets.size()) {
		return nets[uid].first + (nets[uid].second != 0 ?
			"'"+::to_string(nets[uid].second) : "");
	}
	return "";
}

int MockNetlist::netCount() const {
	return (int)nets.size();
}

void MockNetlist::clear() {
	nets.clear();
}


