wget http://downloads.xvid.org/downloads/xvid_latest.tar.gz
tar xf xvid_latest.tar.gz
rm  xvid_latest.tar.gz
cd xvid_*
cd trunk/xvidcore/build/generic
./bootstrap.sh
./configure --prefix="$PREFIX"
make -j4
make -j4 install
cd ../../../../..
rm -rf xvid_*

