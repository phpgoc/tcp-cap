#!/bin/bash
pushd $(dirname $0) >/dev/null
cmddir=$(pwd)
popd >/dev/null

if [ ! -z "$build_type" ] && [ "${build_type}" == "release" ]; then
  build_type=Release
else
  build_type=Debug
fi
echo "build type: ${build_type}"

set -e
mkdir -p $cmddir/../${build_type}/
cd $cmddir/../${build_type}/
pwd
cmake -DCMAKE_BUILD_TYPE=${build_type} -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake -G "CodeBlocks - Unix Makefiles" $cmddir/../
make
