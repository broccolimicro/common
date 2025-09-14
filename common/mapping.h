#pragma once

#include <vector>
#include <map>
#include <ostream>
#include "message.h"

// Mapping is used to transform from one index space to another.  If you are
// applying iterative transformations to a single thing, then create separate
// mappings for each one and then multiply them together.
//
// Mapping<size_t> m0;
// m0.set(1, 5);
// m0.set(2, 3);
//
// Mapping<size_t> m1;
// m1.set(3, 7);
// m1.set(2, 4);
//
// m0 *= m1;
// cout << m0 << endl;

// mapping identity -> {1 -> 5, 2 -> 7, 3 -> 7}
template <typename T>
struct Mapping {
	Mapping(T undef=T(), bool identity = true) {
		this->identity = identity;
		this->undef = undef;
	}

	~Mapping() {
	}

	std::map<T, T> fwd;
	bool identity;
	T undef;

	void set(T from, T to) {
		fwd[from] = to;
	}

	void set(std::initializer_list<T> from, T to) {
		// Remap "from" to "to" in the forward map
		for (auto i = from.begin(); i != from.end(); i++) {
			fwd[*i] = to;
		}
	}

	void set(std::vector<T> from, T to) {
		// Remap "from" to "to" in the forward map
		for (auto i = from.begin(); i != from.end(); i++) {
			fwd[*i] = to;
		}
	}

	void set(std::initializer_list<std::pair<T, T> > m) {
		// Remap "from" to "to" in the forward map
		for (auto i = m.begin(); i != m.end(); i++) {
			fwd[i->first] = i->second;
		}
	}

	void set(std::map<T, T> m) {
		// Remap "from" to "to" in the forward map
		for (auto i = m.begin(); i != m.end(); i++) {
			fwd[i->first] = i->second;
		}
	}

	void update(T from, T to) {
		for (auto i = fwd.begin(); i != fwd.end();) {
			if (i->second == from) {
				i->second = to;
			}
			if (not identity and i->second == undef) {
				i = fwd.erase(i);
			} else {
				++i;
			}
		}

		fwd.insert({from, to});
	}

	T unmap(T to) const {
		for (auto i = fwd.begin(); i != fwd.end(); i++) {
			if (i->second == to) {
				return i->first;
			}
		}
		if (identity) {
			return to;
		}
		return undef;
	}
	
	T map(T from) const {
		auto pos = fwd.find(from);
		if (pos != fwd.end()) {
			return pos->second;
		} else if (identity) {
			return from;
		}
		return undef;
	}

	std::vector<T> map(std::vector<T> from) const {
		std::vector<T> result;
		for (auto i = from.begin(); i != from.end(); i++) {
			auto pos = fwd.find(*i);
			if (pos != fwd.end()) {
				if (pos->second != undef) {
					result.push_back(pos->second);
				}
			} else if (identity) {
				result.push_back(*i);
			}
		}
		return result;
	}

	std::vector<T> mapUniq(std::vector<T> from) const {
		std::vector<T> result;
		for (auto i = from.begin(); i != from.end(); i++) {
			auto pos = fwd.find(*i);
			if (pos != fwd.end()) {
				if (pos->second != undef) {
					result.push_back(pos->second);
				}
			} else if (identity) {
				result.push_back(from);
			}
		}
		sort(result.begin(), result.end());
		result.erase(unique(result.begin(), result.end()), result.end());
		return result;
	}

	bool mapsTo(T t) const {
		auto pos = fwd.find(t);
		if ((identity and pos == fwd.end()) or pos->second == t) {
			return true;
		}
		for (auto i = fwd.begin(); i != fwd.end(); i++) {
			if (i->second == t) {
				return true;
			}
		}
		return false;
	}

	bool mapsFrom(T t) const {
		if (identity) {
			return true;
		}
		return fwd.find(t) != fwd.end();
	}

	bool isBijective() const {
		std::set<T> from;
		std::set<T> to;
		for (auto i = fwd.begin(); i != fwd.end(); i++) {
			if (identity) {
				from.insert(i->first);
			}
			if (not to.insert(i->second).second) {
				return true;
			}
		}
		return identity and not equal(to.begin(), to.end(), from.begin(), from.end());
	}

	Mapping flip() const {
		Mapping result(undef, identity);
		for (auto i = fwd.begin(); i != fwd.end(); i++) {
			auto pos = result.fwd.insert({i->second, i->first});
			if (not pos.second) {
				internal("", "unable to flip mapping with overlapping assignments", __FILE__, __LINE__);
			}
		}
		return result;
	}

	T operator[](T from) const {
		return map(from);
	}

	Mapping<T> &operator+=(const Mapping<T> &m) {
		for (auto i = m.fwd.begin(); i != m.fwd.end(); i++) {
			fwd.set(*i);
		}
		return *this;
	}

	Mapping<T> &operator*=(const Mapping<T> &m) {
		identity = identity and m.identity;
		for (auto i = fwd.begin(); i != fwd.end();) {
			i->second = m.map(i->second);
			if (i->second == (identity ? i->first : undef)) {
				i = fwd.erase(i);
			} else {
				++i;
			}
		}

		if (identity) {
			for (auto i = m.fwd.begin(); i != m.fwd.end(); i++) {
				fwd.insert(*i);
			}
		}
		return *this;
	}
};

template <typename T>
Mapping<T> operator+(Mapping<T> t0, const Mapping<T> &t1) {
	return t0 += t1;
}

template <typename T>
Mapping<T> operator*(Mapping<T> t0, const Mapping<T> &t1) {
	return t0 *= t1;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Mapping<T> &m) {
	os << "map " << (m.identity ? "identity" : "reset") << " -> {";
	for (auto i = m.fwd.begin(); i != m.fwd.end(); i++) {
		if (i != 0) {
			os << ", ";
		}
		os << i->first << " -> " << i->second;
	}
	os << "}" << std::endl;
	return os;
}

/*template <typename T>
struct MappingBidir {
	MappingBidir(T undef=T(), bool identity = true) : fwd(undef, identity), bwd(undef, identity) {
	}

	~MappingBidir() {
	}

	Mapping<T> fwd;
	Mapping<T> bwd;

	void set(T from, T to) {
		fwd.set(from, to);
		bwd.set(to, from);
	}

	void set(std::initializer_list<std::pair<T, T> > m) {
		for (auto i = m.begin(); i != m.end(); i++) {
			fwd.set(i->first, i->second);
			bwd.set(i->second, i->first);
		}
	}

	void set(std::map<T, T> m) {
		for (auto i = m.begin(); i != m.end(); i++) {
			fwd.set(i->first, i->second);
			bwd.set(i->second, i->first);
		}
	}

	void update(T from, T to) {
		fwd.update(from, to);
		bwd.update(to, from);
	}

	T unmap(T to) const {
		return bwd.map(to);
	}
	
	T map(T from) const {
		return fwd.map(from);
	}

	std::vector<T> map(std::vector<T> from) const {
		return fwd.map(from);
	}

	std::vector<T> mapUniq(std::vector<T> from) const {
		return fwd.mapUniq(from);
	}

	bool mapsTo(T t) const {
		return bwd.mapsFrom(t);
	}

	bool mapsFrom(T t) const {
		return fwd.mapsFrom(t);
	}

	MappingBidir flip() const {
		MappingBidir result(undef, identity);
		result.fwd = bwd;
		result.bwd = fwd;
		return result;
	}

	T operator[](T from) const {
		return fwd.map(from);
	}

	MappingBidir<T> &operator+=(const Mapping<T> &m) {
		fwd += m;
		bwd = m.flip() + bwd;
		return *this;
	}

	MappingBidir<T> &operator*=(const Mapping<T> &m) {
		fwd *= m;
		bwd = m.flip()*bwd;
		return *this;
	}
};*/
