#pragma once

#include <iterator>
#include <list>

namespace std {
namespace detail {

template <class Value>
struct hashtable_node
{
	Value value;
	hashtable_node<Value>* next;
};

template <class Value>
class hashtable_iterator {
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = ptrdiff_t;
	using value_type = Value;
	using pointer = Value*;
	using reference = Value&;

	using node_type = hashtable_node<Value>;

	hashtable_iterator(const hashtable_iterator& it) :
		_node(it._node), _bucket(it._bucket)
	{}
	hashtable_iterator(node_type* node, node_type** bucket) :
		_node(node), _bucket(bucket)
	{}

	~hashtable_iterator()
	{}

	hashtable_iterator& operator=(const hashtable_iterator& it)
	{
		_node = it._node;
		_bucket = it._bucket;
		return *this;
	}

	hashtable_iterator& operator++()
	{
		_node = _node->next;
		if (_node == nullptr) {
			++_bucket;
			while (!*_bucket) {
				++_bucket;
			}
			_node = *_bucket;
		}

		return *this;
	}

	hashtable_iterator operator++(int)
	{
		hashtable_iterator it(*this);
		++(*this);
		return it;
	}

	pointer operator->() const
	{
		return &_node->value;
	}

	reference operator*() const
	{
		return _node->value;
	}

	//friend void swap(_hashtable_iterator& lhs, _hashtable_iterator& rhs);
	template<class T>
	friend bool operator==(const hashtable_iterator<T>&, const hashtable_iterator<T>&);
	template<class T>
	friend bool operator!=(const hashtable_iterator<T>&, const hashtable_iterator<T>&);

//private:
public:
	node_type* _node;
	node_type** _bucket;
};

template <class Value>
bool operator==(const hashtable_iterator<Value>& x, const hashtable_iterator<Value>& y)
{
	return x._node == y._node;
}

template <class Value>
bool operator!=(const hashtable_iterator<Value>& x, const hashtable_iterator<Value>& y)
{
	return x._node != y._node;
}


template <class Key, class Value, class Extractor, class Hash, class KeyEqual>
class hashtable
{
public:
	using iterator = hashtable_iterator<Value>;
	using node_type = typename iterator::node_type;
	using value_type = typename iterator::value_type;
	using key_type = Key;
	using size_type = size_t;

	using extractor_type = Extractor;
	using hasher = Hash;
	using key_equal = KeyEqual;

private:
	size_type _node_count;
	size_type _bucket_count;
	node_type** _buckets;
	using node_type_ptr = node_type*;
	node_type_ptr _end;

	extractor_type _extractor;
	hasher _hash;
	key_equal _key_equal;

public:
	hashtable(size_type bucket_count, extractor_type extractor, hasher hash, key_equal equal) :
		_node_count(0), _extractor(extractor), _hash(hash), _key_equal(equal)
	{
		_bucket_count = bucket_count;
		_buckets = new node_type_ptr[bucket_count + 1];

		_end = new node_type();
		_end->next = _end;
		_buckets[bucket_count] = _end;
	}

	~hashtable()
	{}

	size_type size() const
	{
		return _node_count;
	}

	bool empty() const
	{
		return size() == 0;
	}

	// size_type max_size() const noexcept;

	Value& at(const key_type& key)
	{
		iterator it = find(key);
		if (it == end()) {
			throw std::out_of_range();
		} else {
			return *it;
		}
	}

	iterator find(const Key& key)
	{
		size_t i = reduce(key);
		node_type** bucket = _buckets + i;
		node_type* n = find_node(*bucket, key);
		if (n == nullptr) {
			return end();
		} else {
			return iterator(n, bucket);
		}
	}
	// const_iterator& find(const key_type& key) const noexcept;

	bool contains(const Key& key)
	{
		return find(key) != end();
	}

	iterator begin()
	{
		return iterator(*_buckets, _buckets);
	}
	// const_iterator& begin()

	iterator end()
	{
		return iterator(_end, _buckets + _bucket_count);
	}
	// const_iterator& end()

	void clear() noexcept
	{
		node_type** bucket = _buckets;
		while (*bucket != _end) {
			if (*bucket == nullptr) {
				++bucket;
			} else {
				node_type* oldNode = *bucket;
				*bucket = (*bucket)->next;
				--_node_count;
				delete oldNode;
			}
		}
	}

	std::pair<iterator, bool> insert(const value_type& value)
	{
		const key_type& key = extract(value);

		iterator it = find(key);
		if (it != end()) {
			return std::pair<iterator, bool>(std::move(it), false);
		}

		size_t i = reduce(key);
		node_type** bucket = _buckets + i;
		node_type* newNode = new node_type{ value, *bucket };
		*bucket = newNode;
		++_node_count;
		return std::pair<iterator, bool>(iterator(newNode, bucket), true);
	}

	iterator erase(iterator pos)
	{
		node_type** bucket = pos._bucket;
		while (*bucket != pos._node) {
			bucket = &(*bucket)->next;
		}

		node_type* oldNode = *bucket;
		*bucket = (*bucket)->next;
		--_node_count;
		++pos;
		delete oldNode;
		return pos;
	}

	size_type erase(const Key& key)
	{
		iterator it = find(key);
		if (it == end()) {
			return 0;
		} else {
			erase(it);
			return 1;
		}
	}

private:
	const key_type& extract(const value_type& value) const
	{
		return _extractor(value);
	}

	size_type reduce(const key_type& key)
	{
		return _hash(key) % _bucket_count;
	}

	bool compare(const key_type& key, const value_type& value)
	{
		return _key_equal(key, extract(value));
	}

	node_type* find_node(node_type* node, const key_type& key)
	{
		while (node != nullptr) {
			if (compare(key, node->value)) {
				return node;
			}

			node = node->next;
		}

		return nullptr;
	}
};

}
}
