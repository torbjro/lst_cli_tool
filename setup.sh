#!/usr/bin/env sh
echo 'Running script.'

BASEDIR=$(realpath "$(dirname "$0")")

# compile code
cmake -B "${BASEDIR}"/build_files
cmake --build "${BASEDIR}"/build_files
echo 'Compiled code to /build_files.'

# create alias
echo "alias lst='${BASEDIR}/build_files/cli '" >> ~/.bashrc # change . file if you are using different shell
echo 'Please run "source ~/.bashrc" to apply changes.'

echo 'To use lst command, just type in lst in the command line and use it as with ls.'