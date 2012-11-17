wget http://jaist.dl.sourceforge.net/project/faac/faac-src/faac-1.28/faac-1.28.tar.bz2
tar xf faac-1.28.tar.bz2
cd faac-1.28
./configure --prefix="$PREFIX" --with-mp4v2
make -j4 install
cd ..
rm -rf faac-1.28*

