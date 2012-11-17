wget http://www.nasm.us/pub/nasm/releasebuilds/2.10.05/nasm-2.10.05.tar.xz
tar xf nasm-2.10.05.tar.xz
cd nasm-2.10.05
./configure --prefix="$PREFIX"
make -j4 install
cd ..
rm -rf nasm-2.10.05*

