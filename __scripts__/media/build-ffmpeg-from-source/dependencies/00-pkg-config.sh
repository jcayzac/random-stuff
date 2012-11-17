git clone git://anongit.freedesktop.org/pkg-config
cd pkg-config
autoreconf -fvi
./configure --prefix="$PREFIX"
make -j4 install
cd ..
rm -rf pkg-config

