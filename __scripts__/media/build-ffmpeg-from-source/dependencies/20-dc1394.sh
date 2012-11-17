wget http://jaist.dl.sourceforge.net/project/libdc1394/libdc1394-2/2.2.0/libdc1394-2.2.0.tar.gz
tar xf libdc1394-2.2.0.tar.gz
cd libdc1394-2.2.0
./configure --prefix="$PREFIX" --disable-examples --disable-doxygen-doc
make -j4
make -j4 install
cd ..
rm -rf libdc1394-*

