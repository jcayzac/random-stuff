svn co https://gpac.svn.sourceforge.net/svnroot/gpac/trunk/gpac gpac
cd gpac
./configure --prefix="$PREFIX" --cpu=x86_64
make -j4 CC=gcc CXX=g++
make -j4 CC=gcc CXX=g++ install
make -j4 CC=gcc CXX=g++ install-lib
cd ..
rm -rf gpac

