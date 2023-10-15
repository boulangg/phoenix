#!/bin/sh

folders="kernel lib tests tools user"

format_file() {
  file="${1}"
  if [ -f $file ]; then
    clang-format -i ${1}
    git add ${1}
  fi
}

for file in $(find ${folders} -name '*.c' -or -name '*.cpp' -or -name '*.h' -or -wholename "*/libstdcpp/include/*"); do
    if [[ "$file" =~ "malloc" ]] || [[ "$file" =~ "build" ]]; then
        echo "Ignore ${file}"
    else
        echo "Format file: ${file}" 
        format_file "${file}"
    fi
done
