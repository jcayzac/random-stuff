wget http://downloads.xiph.org/releases/flac/flac-1.2.1.tar.gz
tar xf flac-1.2.1.tar.gz
cd flac-1.2.1
# asm sources seem broken
./configure --prefix="$PREFIX" --disable-doxygen-docs --disable-asm-optimizations
make -j4
make -j4 install
cd ..
rm -rf flac-*

