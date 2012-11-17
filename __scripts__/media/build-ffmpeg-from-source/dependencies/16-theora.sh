wget http://downloads.xiph.org/releases/theora/libtheora-1.1.1.tar.bz2
tar xf libtheora-1.1.1.tar.bz2
cd libtheora-1.1.1
./configure --prefix="$PREFIX" --disable-examples
make -j4
make -j4 install
cd ..
rm -rf libtheora-1.1.1*

