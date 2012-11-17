wget http://www.libsdl.org/release/SDL-1.2.15.tar.gz
tar xf SDL-1.2.15.tar.gz
cd SDL-1.2.15
./autogen.sh
CC=clang ./configure --prefix="$PREFIX"
make -j4 install
cd ..
rm -rf SDL-1.2.15*

