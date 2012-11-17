wget http://jaist.dl.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz
tar xf lame-3.99.5.tar.gz
cd lame-3.99.5
./configure --prefix="$PREFIX" --enable-nasm
make -j4 install
cd ..
rm -rf lame-3.99.5*

