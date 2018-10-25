#!/bin/sh

# - install depends tools
# yum -y install git
# yum -y install gcc gcc-c++ autoconf libtool automake make
#

# - clone code
# git clone https://github.com/brinkqiang/dmdump.git
# pushd dmdump
# git submodule update --init --recursive
#

cd thirdparty/dmflags
libtoolize && aclocal && autoheader && autoconf && automake --add-missing
sh configure
cd ~

rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=relwithdebinfo ..
cmake --build .
cd ~
# popd

# echo continue && read -n 1
