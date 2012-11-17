wget http://217.20.164.161/~tipok/aacplus/libaacplus-2.0.2.tar.gz
tar xf libaacplus-2.0.2.tar.gz
cd libaacplus-2.0.2
autoreconf -fvi
./configure --prefix="$PREFIX"
make -j1
make -j4 install
cd ..
rm -rf libaacplus-2.0.2*

