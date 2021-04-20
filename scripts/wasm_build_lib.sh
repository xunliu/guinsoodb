#!/usr/bin/env bash

set -euo pipefail

source "$(dirname "${BASH_SOURCE[0]}")/wasm_env.sh"

GUINSOODB_CC="${PROJECT_ROOT}/src/amalgamation/guinsoodb.cpp"
GUINSOODB_H="${PROJECT_ROOT}/src/amalgamation/guinsoodb.hpp"

test -f "${GUINSOODB_CC}" \
    && { echo "[ OK  ] Amalgamation Source: ${GUINSOODB_CC}"; } \
    || { echo "[ ERR ] Amalgamation Source: ${GUINSOODB_CC}"; exit 1; }

test -f "${GUINSOODB_H}" \
    && { echo "[ OK  ] Amalgamation Header: ${GUINSOODB_H}"; } \
    || { echo "[ ERR ] Amalgamation Header: ${GUINSOODB_H}"; exit 1; }

BUILD_TYPE=${1:-Release}
FLAGS=-O3
case $BUILD_TYPE in
  "Debug") FLAGS=-O0 -g ;;
  "RelWithDebInfo") FLAGS=-O2 -g ;;
   *) ;;
esac
echo "Build Type: ${BUILD_TYPE}"

set -x
BUILD_DIR="${PROJECT_ROOT}/.wasm/build"
if [ -d ${BUILD_DIR} ]; then
    rm -r "${BUILD_DIR}"
fi
mkdir -p ${BUILD_DIR}

source "${EMSDK_ENV}"

${EMCPP} \
    ${FLAGS} \
    -std=gnu++17 \
    -fexceptions \
    -sDISABLE_EXCEPTION_CATCHING=0 \
    -sUSE_PTHREADS=0 \
    -DNDEBUG \
    -DGUINSOODB_NO_THREADS=1 \
    -I ${PROJECT_ROOT}/src/include \
    -I ${PROJECT_ROOT}/third_party/concurrentqueue/ \
    -o ${GUINSOODB_WASM} \
    -c ${GUINSOODB_CC}