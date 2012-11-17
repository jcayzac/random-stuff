git clone git://opencore-amr.git.sourceforge.net/gitroot/opencore-amr/fdk-aac
cd fdk-aac
autoreconf -fvi
./configure --prefix="$PREFIX"
make -j4
make -j4 install
cd ..
rm -rf fdk-aac

