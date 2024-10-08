#!/usr/bin/env sh
echo 'Running script'

BASEDIR=$(dirname "$0")

cmake --build "${BASEDIR}"/cmake-build-debug
echo "alias lst='${BASEDIR}/cmake-build-debug/cli '" >> ~/.bashrc # change .-file if you are using different shell
source ~/.bashrc

echo 'To use lst command, just type in lst in the command line and use it as with ls'