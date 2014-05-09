wget http://www.fftw.org/fftw-3.3.4.tar.gz
tar xf fftw-3.3.4.tar.gz
cd fftw-3.3.4
./configure --prefix="$PREFIX" --enable-shared
make -j4
make -j4 install
cd ..
rm -rf fftw-*

