git clone git://github.com/yasm/yasm.git
cd yasm
./autogen.sh --prefix="$PREFIX"
make -j4
make -j4 install
cd ..
rm -rf yasm

