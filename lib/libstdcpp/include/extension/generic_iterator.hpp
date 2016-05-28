/*
 * Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#ifndef _GENERIC_ITERATOR_HPP_
#define _GENERIC_ITERATOR_HPP_

#include <iterator>

namespace std {
template <class Iterator>
class generic_iterator {
protected:
	typedef iterator_traits<Iterator> _trait_type;

public:
	typedef typename _trait_type::difference_type   difference_type;
	typedef typename _trait_type::value_type        value_type;
	typedef typename _trait_type::pointer           pointer;
	typedef typename _trait_type::reference         reference;
	typedef typename _trait_type::iterator_category iterator_category;

	constexpr generic_iterator() noexcept : _current() {}

	explicit generic_iterator(const Iterator& It) noexcept : _current(It) {}

	reference operator*() const noexcept {
		return *_current;
	}

	pointer operator->() const noexcept {
		return _current;
	}

	generic_iterator& operator++() noexcept {
		++_current;
		return *this;
	}

	generic_iterator operator++(int) noexcept {
		return generic_iterator(_current++);
	}

	generic_iterator& operator--() noexcept {
		--_current;
		return *this;
	}

	generic_iterator operator--(int) noexcept {
		return generic_iterator(_current--);
	}

	reference operator[](difference_type n) const noexcept {
		return _current[n];
	}

	generic_iterator& operator+=(difference_type n) noexcept {
		_current += n;
		return *this;
	}

	generic_iterator operator+(difference_type n) const noexcept {
		return generic_iterator(_current + n);
	}

	generic_iterator& operator-=(difference_type n) const noexcept {
		_current -= n;
		return *this;
	}

	generic_iterator operator-(difference_type n) const noexcept {
		return generic_iterator(_current - n);
	}

	bool operator==(const generic_iterator& it) {
		return (_current == it._current);
	}

	bool operator!=(const generic_iterator& it) {
		return (_current != it._current);
	}

	bool operator<(const generic_iterator& it) {
		return (_current < it._current);
	}

	bool operator>(const generic_iterator& it) {
		return (_current > it._current);
	}

	bool operator<=(const generic_iterator& it) {
		return (_current <= it._current);
	}

	bool operator>=(const generic_iterator& it) {
		return (_current >= it._current);
	}

	const Iterator& base() const {
		return _current;
	}

protected:
	Iterator _current;
};

template <class Iterator>
inline typename generic_iterator<Iterator>::difference_type operator-(
		const generic_iterator<Iterator> lhs,
		const generic_iterator<Iterator> rhs) {
	return lhs.base() - rhs.base();
}

}

#endif // _GENERIC_ITERATOR_HPP_
