/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace kernel::utils {

constexpr std::size_t divRoundUp(std::size_t numerator, std::size_t denominator)
{
    return (numerator + denominator - 1) / denominator;
}

constexpr std::size_t alignUp(std::size_t value, std::size_t align)
{
    return divRoundUp(value, align) * align;
}

constexpr std::size_t alignDown(std::size_t value, std::size_t align)
{
    return (value / align) * align;
}

namespace {
template <class Value>
class SimpleContainerIterator
{
public:
    using value_type = Value;
    using pointer = Value*;
    using reference = Value&;

    SimpleContainerIterator(const SimpleContainerIterator& it) :
        _begin(it._begin), _index(it._index), _entrySize(it._entrySize)
    {}
    SimpleContainerIterator(value_type* begin, std::size_t index, std::size_t entrySize) :
        _begin(reinterpret_cast<std::uint64_t>(begin)), _index(index), _entrySize(entrySize)
    {}

    SimpleContainerIterator& operator=(const SimpleContainerIterator& it)
    {
        _begin = it._begin;
        _index = it._index;
        return *this;
    }

    SimpleContainerIterator& operator++()
    {
        _index++;
        return *this;
    }

    SimpleContainerIterator operator++(int)
    {
        SimpleContainerIterator it(*this);
        ++(*this);
        return it;
    }

    pointer operator->() const
    {
        return reinterpret_cast<pointer>(_begin + _index * _entrySize);
    }

    reference operator*() const
    {
        return *reinterpret_cast<pointer>(_begin + _index * _entrySize);
    }

    template <class T>
    friend bool operator==(const SimpleContainerIterator<T>&, const SimpleContainerIterator<T>&);
    template <class T>
    friend bool operator!=(const SimpleContainerIterator<T>&, const SimpleContainerIterator<T>&);

private:
    std::uint64_t _begin;
    std::size_t _index;
    std::size_t _entrySize;
};

template <class Value>
bool operator==(const SimpleContainerIterator<Value>& x, const SimpleContainerIterator<Value>& y)
{
    // assert(x._begin == y._begin);
    return x._index == y._index;
}

template <class Value>
bool operator!=(const SimpleContainerIterator<Value>& x, const SimpleContainerIterator<Value>& y)
{
    // assert(x._begin == y._begin);
    return x._index != y._index;
}

}
template <class Value>
class SimpleContainer
{
public:
    using value_type = Value;
    using reference_type = Value&;
    using iterator = SimpleContainerIterator<value_type>;
    SimpleContainer() : _ptr(nullptr), _count(0), _entrySize(0) {}

    SimpleContainer(value_type* ptr, std::size_t count, std::size_t entrySize = sizeof(value_type)) :
        _ptr(ptr), _count(count), _entrySize(entrySize)
    {}
    ~SimpleContainer() {}

    iterator begin()
    {
        return iterator(_ptr, 0, _entrySize);
    }

    iterator end()
    {
        return iterator(_ptr, _count, _entrySize);
    }

    reference_type operator[](std::size_t index)
    {
        return _ptr[index];
    }

    std::size_t size()
    {
        return _count;
    }

private:
    Value* _ptr;
    std::size_t _count;
    std::size_t _entrySize;
};

template <class Value, class EntrySize, class EndCondition>
class DynamicContainerIterator
{
public:
    using value_type = Value;
    using pointer = Value*;
    using reference = Value&;

    DynamicContainerIterator(const DynamicContainerIterator& it) : _ptr(it._ptr) {}
    DynamicContainerIterator(value_type* ptr) : _ptr(ptr) {}

    DynamicContainerIterator& operator=(const DynamicContainerIterator& it)
    {
        _ptr = it._ptr;
        return *this;
    }

    DynamicContainerIterator& operator++()
    {
        std::size_t entrySize = EntrySize(*_ptr);
        _ptr = static_cast<pointer>(static_cast<std::uint8_t>(_ptr) + entrySize);

        if (EndCondition(_ptr)) {
            _ptr = nullptr;
        }
    }

    DynamicContainerIterator operator++(int)
    {
        SimpleContainerIterator it(*this);
        ++(*this);
        return it;
    }

    pointer operator->() const
    {
        return _ptr;
    }

    reference operator*() const
    {
        return *_ptr;
    }

    template <class T, class U, class V>
    friend bool operator==(const DynamicContainerIterator<T, U, V>&, const DynamicContainerIterator<T, U, V>&);
    template <class T, class U, class V>
    friend bool operator!=(const DynamicContainerIterator<T, U, V>&, const DynamicContainerIterator<T, U, V>&);

private:
    pointer _ptr;
};

template <class Value, class EntrySize, class EndCondition>
bool operator==(const DynamicContainerIterator<Value, EntrySize, EndCondition>& x,
                const DynamicContainerIterator<Value, EntrySize, EndCondition>& y)
{
    return x._ptr == y._ptr;
}

template <class Value, class EntrySize, class EndCondition>
bool operator!=(const DynamicContainerIterator<Value, EntrySize, EndCondition>& x,
                const DynamicContainerIterator<Value, EntrySize, EndCondition>& y)
{
    return x._ptr != y._ptr;
}

template <class Value, class EntrySize, class EndCondition>
class DynamicContainer
{
public:
    using iterator = DynamicContainerIterator<Value, EntrySize, EndCondition>;

    DynamicContainer(Value* ptr) : _ptr(ptr) {}
    ~DynamicContainer() {}

    iterator begin()
    {
        return iterator(_ptr);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

private:
    Value* _ptr;
};

}
