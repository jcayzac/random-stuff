git clone git://anongit.freedesktop.org/fribidi/fribidi
cd fribidi
rm -rf doc/*
touch doc/Makefile.am
autoreconf -fvi
./configure --prefix="$PREFIX"
make -j1
make -j4 install
cd ..
rm -rf fribidi

