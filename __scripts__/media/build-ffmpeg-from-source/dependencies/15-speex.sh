wget http://downloads.xiph.org/releases/speex/speex-1.2rc1.tar.gz
tar xf speex-1.2rc1.tar.gz
cd speex-1.2rc1
./configure --prefix="$PREFIX"
make -j4
make -j1 install
cd ..
rm -rf speex-*

