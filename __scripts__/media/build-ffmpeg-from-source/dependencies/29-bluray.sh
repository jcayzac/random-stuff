git clone git://git.videolan.org/libbluray.git
cd libbluray
./bootstrap
./configure --prefix="$PREFIX" --disable-examples --disable-debug --disable-doxygen-doc
make -j4
make -j4 install
cd ..
rm -rf libbluray

