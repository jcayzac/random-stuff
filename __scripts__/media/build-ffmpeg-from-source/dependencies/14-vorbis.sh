wget http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.4.tar.gz
tar xf libvorbis-1.3.4.tar.gz
cd libvorbis-1.3.4
./configure --prefix="$PREFIX"
make -j4
make -j4 install
cd ..
rm -rf libvorbis-1.3.4*

