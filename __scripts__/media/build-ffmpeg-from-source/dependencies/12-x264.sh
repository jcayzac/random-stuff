git clone git://git.videolan.org/x264.git
cd x264
./configure --prefix="$PREFIX" --enable-shared --enable-static
make -j4
make -j4 install
cd ..
rm -rf x264

