git clone git://code.dyne.org/frei0r.git
cd frei0r
./autogen.sh
./configure --prefix="$PREFIX" --enable-static
make -j4
make -j4 install
cd ..
rm -rf frei0r

