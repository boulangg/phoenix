/*
 * Copyright (c) 2016-2021 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#define SIZEOF_VECT_ITEM	      sizeof(bitset::vector_t)
#define ROUNDUP_BITS_EXT(nr, d)   ((nr + d - 1) / d)
#define ROUNDUP_BITS(nr)          ROUNDUP_BITS_EXT(nr, SIZEOF_VECT_ITEM)
#define VECTOR_ID_EXT(nr, d)      (nr / d)
#define VECTOR_ID(nr)             VECTOR_ID_EXT(nr, SIZEOF_VECT_ITEM)
#define VECTOR_OFF_EXT(nr, d)     (nr % d)
#define VECTOR_OFF(nr)            VECTOR_OFF_EXT(nr, SIZEOF_VECT_ITEM)
#define VECTOR_ITEM_MASK(nr)      (1 << VECTOR_OFF(nr))

namespace std {

template <size_t N>
bitset<N>::reference::reference(bitset<N>& b, size_t pos) : ref(b), pos(pos) {

}

/*template <size_t N>
operator bitset<N>::reference::bool() const {
	return ref[pos];
}*/

template <size_t N>
typename bitset<N>::reference& bitset<N>::reference::operator=(bool x) {
	ref.set(pos, x);
	return *this;
}

template <size_t N>
typename bitset<N>::reference& bitset<N>::reference::operator=(const reference& x) {
	ref = x.ref;
	pos = x.pos;
	return *this;
}

template <size_t N>
typename bitset<N>::reference& bitset<N>::reference::flip() {
	ref.flip(pos);
	return *this;
}

template <size_t N>
bool bitset<N>::reference::operator~() const {
	return ~ref[pos];
}

template <size_t N>
bitset<N>::bitset() {
	capacity = ROUNDUP_BITS(N);
	vector = new uint64_t[capacity];
	reset();
}

template <size_t N>
bitset<N>::bitset(unsigned long val) : bitset() {
	vector[0] = val;
}

template <size_t N>
bool bitset<N>::operator[](size_t pos) const {
	vector_t item = vector[VECTOR_ID(pos)];
	return ((item & VECTOR_ITEM_MASK(pos)) != 0);
}

template <size_t N>
typename bitset<N>::reference bitset<N>::operator[](size_t pos) {
	return reference(*this, pos);
}

template <size_t N>
bitset<N>& bitset<N>::set() {
	for (size_t i = 0; i < capacity; i++) {
		vector[i] = (vector_t)-1;
	}
	return *this;
}

template <size_t N>
bitset<N>& bitset<N>::set(size_t pos, bool val) {
	vector_t& item = vector[VECTOR_ID(pos)];
	if (val) {
		item |= VECTOR_ITEM_MASK(pos);
	} else {
		item &= ~VECTOR_ITEM_MASK(pos);
	}
	return *this;
}

template <size_t N>
bitset<N>& bitset<N>::reset() {
	for (size_t i = 0; i < capacity; i++) {
		vector[i] = 0;
	}
	return *this;
}

template <size_t N>
bitset<N>& bitset<N>::reset(size_t pos) {
	vector_t& item = vector[VECTOR_ID(pos)];
	item &= ~VECTOR_ITEM_MASK(pos);
	return *this;
}

template <size_t N>
bitset<N>& bitset<N>::flip() {
	for (size_t i = 0; i < capacity; i++) {
		vector[i] = ~vector[i];
	}
	return *this;
}

template <size_t N>
bitset<N>& bitset<N>::flip(size_t pos) {
	set(pos, !this->operator[](pos));
	return *this;
}

template <size_t N>
size_t bitset<N>::count() const {
	size_t count = 0;
	for (size_t i = 0; i < N; i++) {
		if (this->operator[](i)) {
			count++;
		}
	}
	return count;
}

template <size_t N>
size_t bitset<N>::size() const {
	return N;
}

template <size_t N>
bool bitset<N>::test (size_t pos) const {
	if (pos >= N) {
		// TODO throw out_of_range exception
		return false;
	}
}

template <size_t N>
bool bitset<N>::any() const {
	return count() > 0;
}

template <size_t N>
bool bitset<N>::none() const {
	return count() == 0;
}

template <size_t N>
bool bitset<N>::all() const noexcept {
	return count() == N;
}


}
