/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _VECTOR_TPP_
#define _VECTOR_TPP_


template <class T>
vector<T>::vector(): _capacity(DEFAULT_CAPACITY), _size(0),_data(new T[_capacity]) {
}

template <class T>
vector<T>::vector(size_type n, const_reference val): _capacity(DEFAULT_CAPACITY), _size(0),_data(new T[_capacity]) {
	for (int i = 0; i < n; ++i) {
		this->push_back(val);
	}
}

template <class T>
vector<T>::vector(const vector<T>& v): _capacity(v._capacity), _size(v._size),_data(new T[_capacity]) {
    for(size_t i=0;i<_size;i++)
        _data[i]=v._data[i];
}

template <class T>
vector<T>::vector(vector<T>&& v): _capacity(v._capacity), _size(v._size),_data(v._data) {
    v._data= nullptr;
    v._size = 0;
    v._capacity = 0;
}

template <class T>
vector<T>::~vector() {
	delete[] _data;
}

template <class T>
typename vector<T>::reference vector<T>::operator[](int pos) const {
	return _data[pos];
}

template <class T>
vector<T>& vector<T>::operator=(const vector<T>& v) {
    T* tmp = new T[v._capacity];
    for(size_t i=0;i<v._size;i++)
        tmp[i]=v._data[i];
    delete[] _data;
    _data = tmp;
    _size = v._size;
    _capacity = v._capacity;
    return *this;
}

template <class T>
vector<T>& vector<T>::operator=(vector<T>&& v) {
    delete[] _data;
    _data = v._data;
    _size = v._size;
    _capacity = v._capacity;

    v._data = nullptr;
    v._size = 0;
    v._capacity = 0;

    return *this;
}

template <class T>
typename vector<T>::reference vector<T>::front() {
	return _data[0];
}

template <class T>
typename vector<T>::reference vector<T>::back() {
	return _data[_size];
}

template <class T>
void vector<T>::push_back(const_reference val) {
	if(_size ==_capacity)
		resize();
	_data[_size++] = val;
}

template <class T>
void vector<T>::push_back(value_type&& val) {
	value_type v = val;
	push_back(v);
}

template <class T>
void vector<T>::pop_back() {
	if(_size>0)
		_size--;
}

template <class T>
typename vector<T>::size_type vector<T>::size() const {
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
