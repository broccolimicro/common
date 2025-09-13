#pragma once

#include <vector>
#include <map>
#include <ostream>

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
	Mapping() {
		this->identity = true;
	}

	Mapping(T undef) {
		this->identity = false;
		this->undef = undef;
	}

	Mapping(bool identity, T undef=T()) {
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

	bool has(T from) const {
		return fwd.find(from) != fwd.end() or identity;
	}

	Mapping flip() const {
		Mapping result(identity, undef);
		for (auto i = fwd.begin(); i != fwd.end(); i++) {
			result.set(i->second, i->first);
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
		for (auto i = fwd.begin(); i != fwd.end();) {
			i->second = m.map(i->second);
			if (not identity and i->second == undef) {
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
