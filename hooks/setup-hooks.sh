#!/bin/bash

folder=$(dirname $(realpath "$0"))

for hookFolder in "$folder"/*; do
    if [ -d "$hookFolder" ]; then
        hookBaseName=$(basename "$hookFolder")
        hookFilePathName="${folder}/../.git/hooks/${hookBaseName}"
        touch $hookFilePathName
        echo "#!/bin/bash" > $hookFilePathName
        echo "" >> $hookFilePathName
        echo "folder=\"./\$(dirname \"\$0\")/../../hooks\"" >> $hookFilePathName
        for scriptFile in "$hookFolder"/*; do
            scriptFileName=$(basename "$scriptFile")
            echo "\${folder}/${hookBaseName}/${scriptFileName}" >> $hookFilePathName
        done
    fi
done
