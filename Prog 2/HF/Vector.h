#pragma once
#include <string.h>
#include <iostream>

template <typename T>
class Vector {
	size_t capacity;
	size_t _size;
	T* items;

	void resize(size_t newCapacity) {
		if (newCapacity < _size) return;

		T* newItems = new T[newCapacity];
		for (size_t i = 0; i < _size; i++) newItems[i] = items[i];
		
		delete[] items;
		items = newItems;

		capacity = newCapacity;
	}
public:
	Vector() : capacity(2), _size(0), items(new T[2]) {}

	size_t size() {
		return _size;
	}

	//size_t capacity() {
	//	return capacity;
	//}

	void push_back(T item) {
		if (_size + 1 > capacity) resize(capacity * 2);
		_size++;

		items[_size - 1] = item;
	}
	void push_front(T item) {
		if (_size + 1 >= capacity / 2) resize(capacity * 2);
		_size++;

		T* temp = new T[_size];
		for (size_t i = 0; i < (_size - 1); i++) items[1 + i] = temp[i];
		delete[] temp;

		items[0] = item;
	}

	void pop_back() {
		if (_size - 1 < capacity / 2) resize(capacity / 2);
		_size--;

		memset(&items[_size - 1], 0, sizeof(T));
	}

	void splice(size_t start, size_t amount = 1) {
		if (start >= _size) throw std::out_of_range("Start index is out of range");

		size_t newSize = _size - std::min<size_t>(amount, (_size - start));
		T* newItems = new T[newSize];

		for (size_t i = 0; i < start; i++) newItems[i] = items[i];
		if (start + amount < _size)
			for (size_t i = 0; i < (_size - (start + amount)); i++) newItems[start + i] = items[start + amount + i];

		delete[] items;
		items = newItems;
		_size = newSize;

		resize(ceil(_size / 2.0) * 2);
	}

	T& at(size_t index) {
		if (index > _size - 1) throw std::out_of_range("Index is out of range");

		return items[index];
	}

	~Vector() {
		delete[] items;
	}
};
