/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _VECTOR_TPP_
#define _VECTOR_TPP_

template <class T>
vector<T>::vector():_capacity(DEFAULT_CAPACITY), _size(0) {
	_data = new T[_capacity];
}

template <class T>
vector<T>::~vector() {
	delete[] _data;
}

template <class T>
reference vector<T>::operator[](int pos) const {
	return _data[pos];
}

template <class T>
reference vector<T>::front() {
	return _data[0];
}

template <class T>
reference vector<T>::back() {
	return _data[_size];
}

template <class T>
void vector<T>::push_back(const_reference val) {
	if(_size ==_capacity)
		resize();
	_data[_size++] = val;
}

template <class T>
void vector<T>::pop_back() {
	if(_size>0)
		_size--;
}

template <class T>
size_type vector<T>::size() const {
	return _size;
}

template <class T>
bool vector<T>::empty() const {
	return _size==0;
}

template <class T>
void vector<T>::resize() {
	T* tmp = new T[_capacity*2];
	for(size_t i=0;i<_size;i++)
		tmp[i] = _data[i];
	delete[] _data;
	_data = tmp;
	_capacity*=2;
}

#endif // _VECTOR_TPP_
