/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _HEAP_TPP_
#define _HEAP_TPP_

#include <iterator>
#include <utility>
#include <functional>

namespace std 
{

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __moveup_heap(RandomAccessIterator first, Distance holeIndex, 
		Distance topIndex, T value, 
		Compare comp) {
	Distance parentIndex = (holeIndex - 1) / 2;
	while (holeIndex > topIndex && comp(*(first + parentIndex), value)) {
		*(first + holeIndex) = std::move(*(first + parentIndex));
		holeIndex = parentIndex;
		parentIndex = (holeIndex - 1) / 2;
	}
	*(first+holeIndex) = std::move(value);
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __update_heap(RandomAccessIterator first, Distance holeIndex, 
		Distance size, T value, 
		Compare comp) {
	const Distance topIndex = holeIndex;
	Distance leftIndex;
	Distance rightIndex;
	Distance largerIndex;
	while (holeIndex < size) {
		leftIndex = holeIndex*2 + 1;
		rightIndex = holeIndex*2 + 2;
		largerIndex =leftIndex;
		if (rightIndex < size) {
			if (comp(*(first + leftIndex), *(first + rightIndex))) {
				largerIndex = rightIndex;
			}
		}

		if (largerIndex < size) {
			if (comp(value, *(first + largerIndex))) {
				*(first + holeIndex) = std::move(*(first + largerIndex));
				holeIndex = largerIndex;
			} else {
				break;
			} 
		} else {
			break;
		} 
		holeIndex = largerIndex;
	}

	__moveup_heap(first, holeIndex, topIndex, std::move(value), comp);
}

template <class RandomAccessIterator, class Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp) {
	typedef typename iterator_traits<RandomAccessIterator>::value_type value_t;
	typedef typename iterator_traits<RandomAccessIterator>::difference_type dist_t;
	value_t value = std::move(*(last-1));
	__moveup_heap(first, dist_t((last-first) - 1), dist_t(0), 
			std::move(value), comp);
}

template <class RandomAccessIterator, class Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp) {
	typedef typename iterator_traits<RandomAccessIterator>::value_type value_t;
	typedef typename iterator_traits<RandomAccessIterator>::difference_type dist_t;
	if (last - first > 1) {
		--last;
		value_t value = std::move(*last);
		*last = std::move(*first);
		__update_heap(first, dist_t(0), dist_t(last-first), 
				std::move(value), comp);
	}
}

template <class RandomAccessIterator, class Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp) {
	RandomAccessIterator current = first+1;
	for (RandomAccessIterator current = first+1; current < last; ++current) {
		push_heap(first, current, comp);
	}
}

template <class RandomAccessIterator, class Compare>
void remove_heap(RandomAccessIterator first, RandomAccessIterator item, 
		RandomAccessIterator last, Compare comp) {
	typedef typename iterator_traits<RandomAccessIterator>::value_type value_t;
	typedef typename iterator_traits<RandomAccessIterator>::difference_type dist_t;
	if (last - first > 1) {
		--last;
		value_t value = std::move(*last);
		*last = std::move(*item);
		__update_heap(first, dist_t(item-first) , dist_t(last-first), 
				std::move(value), comp);
	}
}

template <class RandomAccessIterator, class Compare>
bool is_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp) {
	return is_heap_until(first, last, comp) == last;
}

template <class RandomAccessIterator, class Compare>
RandomAccessIterator is_heap_until(RandomAccessIterator first,
		RandomAccessIterator last, Compare comp) {
	typedef typename iterator_traits<RandomAccessIterator>::difference_type dist_t;
	dist_t size = last - first;
	for (dist_t child = 1; child < size; ++child) {
		dist_t parent = child / 2;
		if (comp(*(first + parent), *(first + child))) {
			return first + child;
		}
	}
	return last;
}

}

#endif // _HEAP_TPP_
