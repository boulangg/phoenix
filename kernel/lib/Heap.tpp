/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __HEAP_TPP__
#define __HEAP_TPP__

#include <utility>

template<class Elem, class PrioType, PrioType Elem::* attr, class Compare>
Heap<Elem, PrioType, attr, Compare>::Heap(): _tree(), _cmp() {
	_tree.push_back(nullptr);
}

template<class Elem, class PrioType, PrioType Elem::* attr, class Compare>
void Heap<Elem, PrioType, attr, Compare>::insert(Elem* obj, PrioType prio) {
	(*obj).*attr = prio;
	uint64_t index = _tree.size();
	_tree.push_back(obj);
	moveUp(index);
}

template<class Elem, class PrioType, PrioType Elem::* attr, class Compare>
void Heap<Elem, PrioType, attr, Compare>::remove(Elem* obj) {
	uint64_t index;
	for (index = 0; index < _tree.size(); ++index) {
		if (_tree[index] == obj) {
			break;
		}
	}
	if (index >= _tree.size()) {
		return;
	}
	std::swap<Elem*>(_tree[index], _tree.back());
	_tree.pop_back();
	moveDown(index);
}


template<class Elem, class PrioType, PrioType Elem::* attr, class Compare>
void Heap<Elem, PrioType, attr, Compare>::update(Elem* obj, PrioType prio) {
	remove(obj);
	insert(obj, prio);
}

template<class Elem, class PrioType, PrioType Elem::* attr, class Compare>
Elem* Heap<Elem, PrioType, attr, Compare>::top() {
	return _tree[1];
}

template<class Elem, class PrioType, PrioType Elem::* attr, class Compare>
void Heap<Elem, PrioType, attr, Compare>::pop() {
	if (_tree.size() <= 1) {
		return;
	}
	uint64_t index = 1;
	Elem* obj = top();
	std::swap<Elem*>(_tree[index], _tree.back());
	_tree.pop_back();
	moveDown(index);
}

template<class Elem, class PrioType, PrioType Elem::* attr, class Compare>
void Heap<Elem, PrioType, attr, Compare>::moveDown(uint64_t index) {
	int larger, right, left;
	while (!_empty(index)) {
		left = _left(index);
		right = _right(index);
		larger=left;
		if (!_empty(right)) {
			if (_cmp((*_tree[larger]).*attr, (*_tree[right]).*attr)) {
				larger = right;
			}
		}

		if (!_empty(larger)) {
			if (_cmp((*_tree[index]).*attr, (*_tree[larger]).*attr)) {
				std::swap<Elem*>(_tree[index], _tree[larger]);
				index = larger;
			} else {
				break;
			} 
		}
		index = larger;
	}
}

template<class Elem, class PrioType, PrioType Elem::* attr, class Compare>
void Heap<Elem, PrioType, attr, Compare>::moveUp(uint64_t index) {
	int parent;
	while (!_root(index)) {
		parent = _parent(index);
		if (_cmp((*_tree[parent]).*attr, (*_tree[index]).*attr)) {
			std::swap<Elem*>(_tree[index], _tree[parent]);
			index = parent;
		} else {
			break;
		}
	}
}


#endif // __HEAP_TPP__
