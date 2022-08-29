

template <class T>
struct atomic_base
{

};

struct atomic_bool : public atomic_base<bool>
{

};

template <class T>
struct atomic_integer : public atomic_base<T>
{
// fetch_add, fetch_sub, fetch_and, fetch_or, fetch_xor
};

template <class T>
struct atomic_pointer : public atomic_base<T*>
{
// fetch_add + fetch_sub
};

template <class T>
struct atomic_float : public atomic_base<T>
{
// fetch_add, fetch_sub, fetch_and, fetch_or
};

// header atomic
template <class T>
struct atomic;

template <class T>
struct atomic : public atomic_base<T>
{

};

template <class T>
struct atomic<T*> : public atomic_pointer<T>
{

};

template <>
struct atomic<bool> : public atomic_bool
{

};

template <>
struct atomic<char> : public atomic_integer<char>
{

};

template <>
struct atomic<float> : public atomic_float<float>
{

};
