git clone https://code.google.com/p/libass
cd libass
autoreconf -fvi
./configure --prefix="$PREFIX"
make -j4
make -j4 install
cd ..
rm -rf libass

