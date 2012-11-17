git clone git://anongit.freedesktop.org/harfbuzz
cd harfbuzz
./autogen.sh --prefix="$PREFIX" --enable-static
make -j4
make -j4 install
cd ..
rm -rf harfbuzz

