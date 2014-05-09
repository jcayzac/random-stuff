wget http://downloads.xiph.org/releases/flac/flac-1.3.0.tar.gz
tar xf flac-1.3.0.tar.gz
cd flac-1.3.0
# FIXME: asm sources seem broken
./configure --prefix="$PREFIX" --disable-doxygen-docs --disable-asm-optimizations
make -j4
make -j4 install
cd ..
rm -rf flac-*

