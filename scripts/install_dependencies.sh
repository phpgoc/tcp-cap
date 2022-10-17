#!/usr/bin/env bash

set -e
# 如果更新了下边的内容，可以手动删除下两个lock文件，tmp文件夹自己会定时自动清理，本地开发可以把f1,f2改成非tmp目录
f1=/tmp/8302baab259e9d9912ee7e2d6daf05db5829
f2=/tmp/8302baab259e9d9912ee7e2d6daf05db5828
if [ ! -e $f1 ]; then
  if [ -e /etc/redhat-release ]; then
    echo "RedHat"
    sudo yum install -y git cmake gcc-c++ make libtool autoconf automake pkgconfig libsodium-devel
  elif [ -e /etc/debian_version ]; then
    echo "Debian"
    sudo apt-get install -y git cmake g++ make ninja-build libtool autoconf automake pkg-config libsodium-dev bison flex libbison-dev autoconf-archive
  elif [[ $(uname) == "Darwin" ]]; then
    echo "Mac"
    brew install git cmake ninja libtool autoconf automake pkg-config libsodium bison autoconf-archive
    echo "Unknown Operation System"
  fi
  touch $f1
fi
if [ ! -e $f2 ]; then
  which_shell=$(echo $SHELL|awk -F '/' '{print $NF}')
  rcname="~/.${which_shell}rc"
  echo "Install dependencies vcpkg..."
  echo "maybe you need do these yourself:"
  echo "You can copy next few lines and exec them in your terminal ."
  echo "git clone https://github.com/microsoft/vcpkg --depth=1"
  echo "cd vcpkg  && ./bootstrap-vcpkg.sh"
  echo "mv vcpkg /usr/local/bin/"
  echo "#set VCPKG_ROOT to \`pwd\`"
  echo "echo \"export VCPKG_ROOT=\`pwd\`\" >> $rcname "
  echo "source $rcname"

  #cmake+vcpkg 目前无法自动集成pcapplusplus
  vcpkg install pcapplusplus

  vcpkg install redis-plus-plus

  vcpkg install tomlplusplus

  # 非mac系统安装不成，可能是perl的问题，也可能是网络问题。
  vcpkg install cnats

  vcpkg install libmysql

  touch $f2
fi
