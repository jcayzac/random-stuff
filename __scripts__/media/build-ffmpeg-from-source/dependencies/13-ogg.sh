wget http://downloads.xiph.org/releases/ogg/libogg-1.3.0.tar.gz
tar xf libogg-1.3.0.tar.gz
cd libogg-1.3.0
./configure --prefix="$PREFIX"
make -j4
make -j4 install
cd ..
rm -rf libogg-1.3.0*

