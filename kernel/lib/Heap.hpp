/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef __HEAP_HPP__
#define __HEAP_HPP__

#include <vector>

template<class Elem, class PrioType, PrioType Elem::* attr, class Compare>
class Heap {
public:
	Heap();

	/* Insert an element in the heap with the priority prio. The
	 * corresponding attribute is update with the given prio.
	 * Complexity: O(log(n))
	 */
	void insert(Elem* obj, PrioType prio);

	/* Remove the given element from the heap.
	 * Complexity: O(log(n))
	 */
	void remove(Elem* obj);

	/* Update the priority of an element and reorder the heap
	 * based on the new value.
	 * Complexity: O(n)
	 */
	void update(Elem* obj, PrioType prio);

	/* Return the element with the highest priority without removing it
	 * Complexity: O(n)
	 */
	Elem* top();

	/* Remove the element with the highest priority from the heap.
	 * Complexity: O(log(n))
	 */
	void pop();


private:
	uint64_t _left(uint64_t index) {return 2*index;}
	uint64_t _right(uint64_t index) {return 2*index+1;}
	uint64_t _parent(uint64_t index) {return index/2;}
	bool _root(uint64_t index) {return index == 1;}
	bool _empty(uint64_t index) {return index >= _tree.size();}

	void moveUp(uint64_t index);
	void moveDown(uint64_t index);

	std::vector<Elem*> _tree;
	Compare _cmp;
};

#include "Heap.tpp"

#endif // __HEAP_HPP__
