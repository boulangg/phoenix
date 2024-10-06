/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <def/type_traits/arrays.h>

#include <def/type_traits/_is_same.h>

namespace libstdcpp {
namespace type_traits {
// remove_extent
static_assert(std::is_same_v<std::remove_extent_t<int[]>, int> == true);
static_assert(std::is_same_v<std::remove_extent_t<int[][10]>, int[10]> == true);

// remove_all_extents
static_assert(std::is_same_v<std::remove_all_extents_t<int[][10]>, int> == true);
}
}
