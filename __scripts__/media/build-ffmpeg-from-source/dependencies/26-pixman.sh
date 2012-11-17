git clone git://anongit.freedesktop.org/git/pixman.git
cd pixman
./autogen.sh --prefix="$PREFIX"
make -j4
make -j4 install
cd ..
rm -rf pixman

