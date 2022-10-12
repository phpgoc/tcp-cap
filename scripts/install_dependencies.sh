#!/usr/bin/env bash

set -e
f1=/tmp/8302baab259e9d9912ee7e2d6daf05db5829
f2=/tmp/8302baab259e9d9912ee7e2d6daf05db5828
if [ ! -e $f1 ]; then
  if [ -e /etc/redhat-release ]; then
    echo "RedHat"
    sudo yum install -y cmake gcc-c++ make libtool autoconf automake pkgconfig libsodium-devel bison-devel
  elif [ -e /etc/debian_version ]; then
    echo "Debian"
    sudo apt-get install -y cmake g++ make ninja-build libtool autoconf automake pkg-config libsodium-dev bison flex libbison-dev
  elif [ $(uname) == "Darwin" ]; then
    echo "Mac"
    brew install cmake ninja libtool autoconf automake pkg-config libsodium bison
  else
    echo "Unknown"
  fi
  touch $f1
fi
if [ ! -e $f2 ]; then

  echo "Install dependencies vcpkg..."
  echo "maybe you need do these yourself:"
  echo "git clone https://github.com/microsoft/vcpkg"
  echo "cd vcpkg  && ./bootstrap-vcpkg.sh"
  echo "mv vcpkg /usr/local/bin/"
  echo "set VCPKG_ROOT to <vcpkg path>"
  echo "echo \"export VCPKG_ROOT=<vcpkg path >\" >> ~/.bashrc\""

  vcpkg install pcapplusplus

  vcpkg install redis-plus-plus

  vcpkg install tomlplusplus

  vcpkg install cnats

  vcpkg install libuv

  touch $f2
fi