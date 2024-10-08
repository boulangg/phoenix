/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#ifdef __cplucplus
namespace std {
typedef decltype(nullptr) nullptr_t;
}
using ::std::nullptr_t;
#else
//typedef typeof_unqual(nullptr) nullptr_t;
#endif
