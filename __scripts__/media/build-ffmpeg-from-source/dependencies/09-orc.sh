git clone git://code.entropywave.com/git/orc.git
cd orc
autoreconf -fvi
./configure --prefix="$PREFIX" --disable-static
make -j4 install
cd ..
rm -rf orc

