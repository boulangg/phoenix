#!/bin/bash

copyright="/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 "

add_copyright() {
    file_to_copyright="${1}"
    if [ -f $file_to_copyright ]; then
        if ! grep -qi "Copyright" $file; then
            echo "${copyright}$(cat $file_to_copyright)" > $file_to_copyright
            git add ${1}
        fi
    fi
}

for file in `git diff-index --cached --name-only HEAD`; do
    if [[ $file =~ \.(c|cpp|h|cc)$ ]] || [[ $file =~ .*"libstdcpp/include".* ]]; then
        add_copyright "${file}"
    fi
done
