#!/usr/bin/env bash

format_file() {
    file_to_format="${1}"
    if [ -f $file_to_format ]; then
        clang-format -i $file_to_format
        git add $file_to_format
    fi
}

for file in `git diff-index --cached --name-only HEAD`; do
    if [[ $file =~ \.(c|cpp|h|cc)$ ]] || [[ $file =~ .*"libstdcpp/include".* ]]; then
        format_file "${file}"
    fi
done
