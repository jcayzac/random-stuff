svn co https://xavs.svn.sourceforge.net/svnroot/xavs xavs
cd xavs/trunk
sed -i '' -E "s#prefix='/usr/local'#prefix='${PREFIX}'#g" configure
./configure --host="x86_64-apple-darwin$(uname -r)" --disable-asm
make -j4
make -j4 install
cd ../..
rm -rf xavs

