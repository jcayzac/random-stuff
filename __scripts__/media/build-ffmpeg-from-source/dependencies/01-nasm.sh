wget http://www.nasm.us/pub/nasm/releasebuilds/2.11.02/nasm-2.11.02.tar.xz
tar xf nasm-2.11.02.tar.xz
cd nasm-2.11.02
./configure --prefix="$PREFIX"
make -j4 install
cd ..
rm -rf nasm-2.11.02*

