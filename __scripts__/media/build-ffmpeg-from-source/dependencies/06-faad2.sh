wget http://jaist.dl.sourceforge.net/project/faac/faad2-src/faad2-2.7/faad2-2.7.tar.gz
tar xf faad2-2.7.tar.gz
cd faad2-2.7
./configure --prefix="$PREFIX" --with-mpeg4ip --with-drm
make -j4 install
cd ..
rm -rf faad2-2.7*

