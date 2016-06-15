/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _LIST_TPP_
#define _LIST_TPP_

template<class T>
_list_node<T>::_list_node(T data, _list_node::_self* prev, _list_node::_self* next) :
	data(data), prev(prev), next(next) {
	if (prev == nullptr) {
		prev = this;
	}
	if (next == nullptr) {
		next = this;
	}
}

template <class T>
_list_iterator<T>::_list_iterator() : node(nullptr) {}

template <class T>
_list_iterator<T>::_list_iterator(const _list_iterator& b) : node(b.node) {}

template <class T>
_list_iterator<T>::_list_iterator(_node* node) : node(node) {}

template <class T>
_list_iterator<T>::~_list_iterator() {}

template <class T>
_list_iterator<T> & _list_iterator<T>::operator=(const _list_iterator<T>& b) {
	node = b.node;
	return *this;
}

template <class T>
bool _list_iterator<T>::operator==(const _list_iterator<T>& b) {
	return node == b.node;
}

template <class T>
bool _list_iterator<T>::operator!=(const _list_iterator<T>& b) {
	return node != b.node;
}

template <class T>
typename _list_iterator<T>::value_type& _list_iterator<T>::operator*() {
	return node->data;
}

template <class T>
typename _list_iterator<T>::value_type& _list_iterator<T>::operator->() {
	return *this;
}

template <class T>
_list_iterator<T>& _list_iterator<T>::operator++() {
	node = node->next;
	return *this;
}

template <class T>
_list_iterator<T>& _list_iterator<T>::operator--() {
	node = node->prev;
	return *this;
}

template <class T>
_list_iterator<T> _list_iterator<T>::operator++(int) {
	_list_iterator tmp(*this);
	operator++();
	return tmp;
}

template <class T>
_list_iterator<T> _list_iterator<T>::operator--(int) {
	_list_iterator tmp(*this);
	operator--();
	return tmp;
}

template <class T>
void _list_iterator<T>::swap(_list_iterator<T>& b) {
	swap(node, b.node);
}

template <class T>
list<T>::list() : _begin(), _end(), _size(0) {
	_node* node = new _node(T(), 0, 0);
	_begin.node = node;
	_end.node = node;
}

template <class T>
list<T>::~list() {
	clear();
}

template <class T>
list<T>& list<T>::operator=(const list<T>& x) {
	clear();
	for (auto it = x.begin(); it < x.end(); ++it) {
		push_back(*it);
	}
}

template <class T>
list<T>& list<T>::operator=(list<T>&& x) {
	clear();
	_begin = x._begin;
	_end = x._end;
	_size = x._size;
}

template <class T>
typename list<T>::iterator list<T>::begin() {
	return _begin;
}

template <class T>
typename list<T>::iterator list<T>::end() {
	return _end;
}

template <class T>
bool list<T>::empty() const {
	return _size == 0;
}

template <class T>
typename list<T>::size_type list<T>::size() const {
	return _size;
}

template <class T>
typename list<T>::reference list<T>::front() {
	return *_begin;
}

template <class T>
typename list<T>::reference list<T>::back() {
	iterator it = _end;
	return *(--it);
}

template <class T>
void list<T>::push_front(list<T>::const_reference val) {
	insert(_begin, val);
}

/*template <class T>
void list<T>::push_front(list<T>::value_type&& val) {
	insert(_begin, val);
}*/

template <class T>
void list<T>::pop_front() {
	erase(_begin);
}


template <class T>
void list<T>::push_back(list<T>::const_reference val) {
	insert(_end, val);
}

/*template <class T>
void list<T>::push_back(list<T>::value_type&& val) {
	insert(_end, val);
}*/

template <class T>
void list<T>::pop_back() {
	remove(_end);
}

template <class T>
typename list<T>::iterator list<T>::insert(iterator pos, list<T>::const_reference val) {
	if (pos == _begin) {
		_node* curr = new _node(val, nullptr, _begin.node);
		_begin.node->prev = curr;
		_begin.node = curr;
		++_size;
		return _begin;
	} else {
		_node* prev = pos.node->prev;
		_node* next = pos.node;
		_node* curr = new _node(val, prev, next);
		prev->next = curr;
		next->prev = curr;
		++_size;
		iterator res(curr);
		return res;
	}
}

template <class T>
typename list<T>::iterator list<T>::erase(iterator pos) {
	if (_size == 0) {
		return _end;
	}
	if (pos == _end) {
		return _end;
	}
	if (pos == _begin) {
		_node* curr = pos.node;
		_node* next = pos.node->next;
		next->prev = next;
		_begin = next;
		--_size;
		delete curr;
		return _begin;
	} else {
		_node* prev = pos.node->prev;
		_node* curr = pos.node;
		_node* next = pos.node->next;
		prev->next = next;
		next->prev = prev;
		--_size;
		delete curr;
		iterator res(next);
		return res;
	}
}

template <class T>
void list<T>::swap(list& x) {
	std::swap(_begin, x._begin);
	std::swap(_end, x._end);
	std::swap(_size, x._size);
}

template <class T>
void list<T>::clear() {
	while(size()) {
		pop_front();
	}
}

#endif // _LIST_TPP
