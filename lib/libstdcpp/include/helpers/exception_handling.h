/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

namespace std {
namespace details {

void* __exc_get_cuurent_ptr();
void __exc_increment_refCount(void* ptr);
void __exc_decrement_refcount(void* ptr);
void __exc_rethrow_exception(void* ptr);

}
}
