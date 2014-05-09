git clone git://libpng.git.sourceforge.net/gitroot/libpng/libpng
cd libpng
git fetch --tags
# Builds 1.2
git checkout v1.2.51
sed -i '' -E 's/libtoolize/glibtoolize/g' autogen.sh
./autogen.sh
./configure --prefix="$PREFIX" --enable-static --disable-shared
make -j4 install
# Builds 1.6
git reset --hard
git clean -fdx
git checkout master
LIBTOOLIZE=glibtoolize ./autogen.sh
./configure --prefix="$PREFIX" --enable-static --disable-shared
make -j4 install
cd ..
rm -rf libpng

