/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <cstddef>
#include <utility>

#include "mem/Page.h"

#include "limine.h"

namespace kernel::boot {

std::pair<std::uint64_t, std::size_t> initPageArray(const limine_memmap_request& memmap_request);

}
