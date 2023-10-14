#!/bin/bash

folder=$(dirname $(realpath "$0"))

for hookFolder in "$folder"/*; do
    if [ -d "$hookFolder" ]; then
        hookBaseName=$(basename "$hookFolder")
        hookFilePathName="${folder}/../.git/hooks/${hookBaseName}"
        touch $hookFilePathName
        echo "#!/bin/sh" > $hookFilePathName
        echo "" >> $hookFilePathName
        for scriptFile in "$hookFolder"/*; do
            scriptFileName=$(basename "$scriptFile")
            echo "${folder}/${hookBaseName}/${scriptFileName}" >> $hookFilePathName
        done
    fi
done
