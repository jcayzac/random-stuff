wget http://webm.googlecode.com/files/libvpx-v1.1.0.tar.bz2
tar xf libvpx-v1.1.0.tar.bz2
cd libvpx-v1.1.0
./configure --prefix="$PREFIX" --cpu=core2 --enable-vp8 --enable-postproc --disable-install-docs --disable-examples
make -j4
make -j4 install
cd ..
rm -rf libvpx-*

