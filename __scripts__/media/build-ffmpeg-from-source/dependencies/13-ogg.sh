wget http://downloads.xiph.org/releases/ogg/libogg-1.3.1.tar.gz
tar xf libogg-1.3.1.tar.gz
cd libogg-1.3.1
./configure --prefix="$PREFIX"
make -j4
make -j4 install
cd ..
rm -rf libogg-1.3.1*

