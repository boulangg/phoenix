/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #include <compare>

namespace std {

constexpr std::partial_ordering const std::partial_ordering::less{0};
constexpr std::partial_ordering const std::partial_ordering::equivalent{1};
constexpr std::partial_ordering const std::partial_ordering::greater{2};
constexpr std::partial_ordering const std::partial_ordering::unordered{3};

constexpr std::strong_ordering const std::strong_ordering::less{0};
constexpr std::strong_ordering const std::strong_ordering::equivalent{1};
constexpr std::strong_ordering const std::strong_ordering::equal{2};
constexpr std::strong_ordering const std::strong_ordering::greater{3};

}
