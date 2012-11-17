wget http://jaist.dl.sourceforge.net/project/expat/expat/2.1.0/expat-2.1.0.tar.gz
tar xf expat-2.1.0.tar.gz
cd expat-2.1.0
./configure --prefix="$PREFIX" --disable-shared
make -j4
make -j4 install
cd ..
rm -rf expat-*

