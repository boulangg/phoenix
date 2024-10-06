# Libstdcpp

Based on N4950 (C++ latest public working draft on 2023-05-10)

## Concept library
| Header   | Standard | Support | Note |
|----------|----------|---------|------|
| concepts | C++20    | None    |      |


## Coroutine library
| Header    | Standard | Support | Note |
|-----------|----------|---------|------|
| coroutine | C++20    | None    |      |

## Utilities library

| Header          | Standard | Support | Note |
|-----------------|----------|---------|------|
| any             | C++17    | None    |      |
| bitset          |          | None    |      |
| chrono          | C++11    | None    |      |
| compare         | C++20    | None    |      |
| csetjmp         |          | None    |      |
| csignal         |          | None    |      |
| **cstdarg**         |          | **Full**    | **-**     |
| cstddef         |          | Partial    | Missing std::byte::to_integer |
| ctime           |          | None    |      |
| debugging       | C++26    | None    |      |
| expected        | C++23    | None    |      |
| functional      |          | None    |      |
| intializer_list | C++11    | None    |      |
| optional        | c++17    | None    |      |
| source_location | c++20    | None    |      |
| tuple           | c++11    | None    |      |
| type_traits     | c++11    | None    |      |
| typeindex       | c++11    | None    |      |
| typeinfo        |          | None    |      |
| utility         |          | None    |      |
| variant         | c++17    | None    |      |
| version         | c++20    | None    |      |

## Dynamic memory management

| Header           | Standard | Support | Note |
|------------------|----------|---------|------|
| memory           |          | None    |      |
| memory_resource  | c++17    | None    |      |
| new              |          | None    |      |
| scoped_allocator | C++11    | None    |      |

## Numeric limits

| Header    | Standard | Support | Note |
|-----------|----------|---------|------|
| cfloat    |          | None    |      |
| cinttypes | c++11    | None    |      |
| **climits**   |          | **Full**    | **-**     |
| cstdint   | C++11    | None    |      |
| limits    |          | None    |      |
| stdfloat  | c++23    | None    |      |

## Error handling

| Header        | Standard | Support  | Note           |
|---------------|----------|----------|----------------|
| cassert       |          | None     |                |
| cerrno        |          | None     |                |
| exception     |          | None     |                |
| stacktrace    | c++23    | None     |                |
| **stdexcept** |          | **Full** | **Not tested** |
| system_error  | c++11    | None     |                |

## Strings library

| Header      | Standard | Support | Note |
|-------------|----------|---------|------|
| cctype      |          | None    |      |
| charconv    | c++17    | None    |      |
| cstring     |          | None    |      |
| cuchar      | c++11    | None    |      |
| cwhar       |          | None    |      |
| cwctype     |          | None    |      |
| format      | C++20    | None    |      |
| string      |          | Partial |      |
| string_view | C++17    | None    |      |

## Containers library

| Header         | Standard | Support | Note |
|----------------|----------|---------|------|
| array          | c++11    | None    |      |
| deque          |          | None    |      |
| flat_map       | C++23    | None    |      |
| flat_set       | C++23    | None    |      |
| forward_list   | C++11    | None    |      |
| inplace_vector | C++26    | None    |      |
| list           |          | None    |      |
| map            |          | None    |      |
| mdspan         | C++23    | None    |      |
| queue          |          | None    |      |
| set            |          | None    |      |
| span           | C++20    | None    |      |
| stack          |          | None    |      |
| unordered_map  | C++11    | Partial |      |
| unordered_set  | C++11    | Partial |      |
| vector         |          | None    |      |

## Iterators library

| Header         | Standard | Support | Note |
|----------------|----------|---------|------|
| iterator       |          | None    |      |

## Ranges library

| Header         | Standard | Support | Note |
|----------------|----------|---------|------|
| generator      | C++23    | None    |      |
| ranges         | C++20    | None    |      |

## Algorithms library

| Header         | Standard | Support | Note |
|----------------|----------|---------|------|
| algorithm      |          | None    |      |
| ranges         | C++17    | None    |      |

## Numerics library

| Header        | Standard | Support | Note |
|---------------|----------|---------|------|
| bit           | C++20    | None    |      |
| cfenv         | C++11    | None    |      |
| cmath         |          | None    |      |
| complex       |          | None    |      |
| linalg        | C++26    | None    |      |
| numbers       | C++20    | None    |      |
| numeric       |          | None    |      |
| random        | C++11    | None    |      |
| ratio         | C++11    | None    |      |
| valarray      |          | None    |      |

## Localization library

| Header        | Standard | Support | Note                                   |
|---------------|----------|---------|----------------------------------------|
| cloale        | C++20    | None    |                                        |
| codecvt       | C++11    | None    | deprecated in C++17 - removed in C++26 |
| locale        |          | None    |                                        |
| text_encoding | C++26    | None    |                                        |

## Input/output library

| Header        | Standard | Support | Note                                  |
|---------------|----------|---------|---------------------------------------|
| cstdio        |          | None    |                                       |
| fstream       |          | None    |                                       |
| iomanip       |          | None    |                                       |
| ios           |          | None    |                                       |
| iosfwd        |          | None    |                                       |
| iostream      |          | None    |                                       |
| istream       |          | None    |                                       |
| ostream       |          | None    |                                       |
| print         | C++23    | None    |                                       |
| spanstream    | C++23    | None    |                                       |
| sstream       |          | None    |                                       |
| sstreambuf    |          | None    |                                       |
| strstream     |          | None    | deperacate in C++98 - remove in C++26 |
| syncstream    | C++20    | None    |                                       |

## Filesystem library

| Header     | Standard | Support | Note |
|------------|----------|---------|------|
| filesystem | C++17    | None    |      |

## Regular Expressions library

| Header | Standard | Support | Note |
|--------|----------|---------|------|
| regex  | C++11    | None    |      |

## Atomic Operations library

| Header | Standard | Support | Note |
|--------|----------|---------|------|
| atomic | C++11    | None    |      |

## Thread support library

| Header             | Standard | Support | Note |
|--------------------|----------|---------|------|
| barrier            | C++20    | None    |      |
| condition_variable | C++11    | None    |      |
| future             | C++11    | None    |      |
| hazard_pointer     | C++26    | None    |      |
| latch              | C++20    | None    |      |
| mutex              | C++11    | None    |      |
| rcu                | C++26    | None    |      |
| semaphore          | C++20    | None    |      |
| shared_mutex       | C++14    | None    |      |
| stop_token         | C++20    | None    |      |
| thread             | C++11    | None    |      |
