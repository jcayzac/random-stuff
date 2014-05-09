wget http://downloads.xiph.org/releases/opus/opus-1.1.tar.gz
tar xf opus-1.1.tar.gz
cd opus-1.1
./configure --prefix="$PREFIX" --enable-custom-modes --enable-float-approx --disable-doc
make -j4
make -j4 install
cd ..
rm -rf opus-*

