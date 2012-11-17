git clone git://git.sv.nongnu.org/freetype/freetype2.git
cd freetype2
./autogen.sh
./configure --prefix="$PREFIX"
make -j4
make -j4 install
cd ..
rm -rf freetype2

