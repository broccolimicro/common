#pragma once

#include <vector>
#include <optional>
#include <iterator>
#include <cstddef>

template<typename T>
struct index_vector {
	using value_type = T;
	using size_type = std::size_t;

	std::vector<T> elems;
	std::vector<bool> valid;
	std::vector<size_t> free;

	index_vector() = default;

	// Add a new element, reusing index if possible
	template<typename... Args>
	size_type emplace(Args&&... args) {
		if (not free.empty()) {
			size_type i = free.back(); free.pop_back();
			elems[i] = T(std::forward<Args>(args)...);
			valid[i] = true;
			return i;
		}
		elems.emplace_back(std::forward<Args>(args)...);
		valid.push_back(true);
		return elems.size() - 1;
	}

	size_type next_index() const {
		if (not free.empty()) {
			return free.back();
		}
		return elems.size();
	}

	template<typename... Args>
	void emplace_at(size_type i, Args&&... args) {
		if (i >= elems.size()) {
			size_type old_size = elems.size();
			elems.resize(i + 1);
			valid.resize(i + 1, false);

			for (size_type j = old_size; j < i; ++j) {
				free.push_back(j);
			}
		}

		if (!valid[i]) {
			auto it = std::find(free.begin(), free.end(), i);
			if (it != free.end()) {
				free.erase(it);
			}

			elems[i] = T(std::forward<Args>(args)...);
			valid[i] = true;
		} else {
			elems[i] = T(std::forward<Args>(args)...);
		}
	}

	size_type insert(const T& value) {
		return emplace(value);
	}

	bool erase(size_type i) {
		if (is_valid(i)) {
			valid[i] = false;
			free.push_back(i);
			return true;
		}
		return false;
	}

	bool is_valid(size_type i) const {
		return i < valid.size() and valid[i];
	}

	T& at(size_type i) {
		if (not is_valid(i)) throw std::out_of_range("index_vector::at() invalid index");
		return elems[i];
	}

	const T& at(size_type i) const {
		if (not is_valid(i)) throw std::out_of_range("index_vector::at() invalid index");
		return elems[i];
	}

	T& operator[](size_type i) {
		if (not is_valid(i)) throw std::out_of_range("index_vector::at() invalid index");
		return elems[i];
	}

	const T& operator[](size_type i) const {
		if (not is_valid(i)) throw std::out_of_range("index_vector::at() invalid index");
		return elems[i];
	}

	size_type capacity() const {
		return elems.size();
	}

	size_type size() const {
		return elems.size() - free.size();
	}

	void clear() {
		elems.clear();
		valid.clear();
		free.clear();
	}

	void compact() {
		std::vector<T> new_elems;
		std::vector<bool> new_valid;

		for (size_type i = 0; i < elems.size(); ++i) {
			if (valid[i]) {
				new_elems.push_back(std::move(elems[i]));
				new_valid.push_back(true);
			}
		}

		elems = std::move(new_elems);
		valid = std::move(new_valid);
		free.clear();
	}

	struct iterator {
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using difference_type = std::ptrdiff_t;
		
		size_type index;
		index_vector<T>* parent;
		
		void skip_invalid() {
			while (index < parent->elems.size() and not parent->valid[index]) ++index;
		}

		iterator(index_vector<T>* p, size_type i) : index(i), parent(p) { skip_invalid(); }
		reference operator*() const { return parent->elems[index]; }
		pointer operator->() const { return &parent->elems[index]; }
		iterator& operator++() { ++index; skip_invalid(); return *this; }
		iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
		bool operator==(const iterator& other) const { return index == other.index; }
		bool operator!=(const iterator& other) const { return not (*this == other); }
	};

	struct const_iterator {
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using reference = const T&;
		using pointer = const T*;
		using difference_type = std::ptrdiff_t;

		size_type index;
		const index_vector<T>* parent;
		
		void skip_invalid() {
			while (index < parent->elems.size() and not parent->valid[index]) ++index;
		}

		const_iterator(const index_vector<T>* p, size_type i) : index(i), parent(p) { skip_invalid(); }
		reference operator*() const { return parent->elems[index]; }
		pointer operator->() const { return &parent->elems[index]; }
		const_iterator& operator++() { ++index; skip_invalid(); return *this; }
		const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
		bool operator==(const const_iterator& other) const { return index == other.index; }
		bool operator!=(const const_iterator& other) const { return not (*this == other); }
	};

	struct reverse_iterator {
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using difference_type = std::ptrdiff_t;

		size_type index;
		index_vector<T>* parent;

		void skip_invalid_reverse() {
			while (index != size_type(-1) and not parent->valid[index])
				--index;
		}

		reverse_iterator(index_vector<T>* p, size_type i) : index(i), parent(p) { skip_invalid_reverse(); }

		reference operator*() const { return parent->elems[index]; }
		pointer operator->() const { return &parent->elems[index]; }

		reverse_iterator& operator++() {
			if (index == 0) index = size_type(-1);
			else --index;
			skip_invalid_reverse();
			return *this;
		}

		reverse_iterator operator++(int) {
			reverse_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator==(const reverse_iterator& other) const { return index == other.index; }
		bool operator!=(const reverse_iterator& other) const { return not (*this == other); }
	};

	struct const_reverse_iterator {
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using reference = const T&;
		using pointer = const T*;
		using difference_type = std::ptrdiff_t;
		
		size_type index;
		const index_vector<T>* parent;

		void skip_invalid_reverse() {
			while (index != size_type(-1) and not parent->valid[index])
				--index;
		}

		const_reverse_iterator(const index_vector<T>* p, size_type i)
				: index(i), parent(p) {
			skip_invalid_reverse();
		}

		reference operator*() const { return parent->elems[index]; }
		pointer operator->() const { return &parent->elems[index]; }

		const_reverse_iterator& operator++() {
			if (index == 0) index = size_type(-1);
			else --index;
			skip_invalid_reverse();
			return *this;
		}

		const_reverse_iterator operator++(int) {
			const_reverse_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator==(const const_reverse_iterator& other) const { return index == other.index; }
		bool operator!=(const const_reverse_iterator& other) const { return not (*this == other); }
	};

	iterator begin() { return iterator(this, 0); }
	iterator end() { return iterator(this, elems.size()); }

	const_iterator begin() const { return const_iterator(this, 0); }
	const_iterator end() const { return const_iterator(this, elems.size()); }

	reverse_iterator rbegin() { return reverse_iterator(this, elems.size() ? elems.size() - 1 : size_type(-1)); }
	reverse_iterator rend()   { return reverse_iterator(this, size_type(-1)); }

	const_reverse_iterator rbegin() const { return const_reverse_iterator(this, elems.size() ? elems.size() - 1 : size_type(-1)); }
	const_reverse_iterator rend() const   { return const_reverse_iterator(this, size_type(-1)); }
};

