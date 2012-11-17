git clone git://anongit.freedesktop.org/fontconfig
cd fontconfig
LIBTOOLIZE=glibtoolize ./autogen.sh --prefix="$PREFIX"
make -j4
make -j4 install
cd ..
rm -rf fontconfig

