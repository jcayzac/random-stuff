wget http://openjpeg.googlecode.com/files/openjpeg-1.5.1.tar.gz
tar xf openjpeg-1.5.1.tar.gz
cd openjpeg-1.5.1
./configure --prefix="$PREFIX" --disable-doc
make -j4
make -j4 install
cd ..
rm -rf openjpeg-*

