git clone git://code.dyne.org/frei0r.git
cd frei0r
./autogen.sh
./configure --prefix="$PREFIX" --enable-static
make -j4
make -j4 install
cd ..
rm -rf frei0r
cd "${PREFIX}/lib/frei0r-1"
for X in *.so
do
	ln -s $X ${X%.*}.dylib
done
cd $OLDPWD

