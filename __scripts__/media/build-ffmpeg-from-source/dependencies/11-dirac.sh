git clone git://diracvideo.org/git/dirac-research.git
cd dirac-research
rm -rf doc/*
touch doc/Makefile.am
autoreconf -fvi -I m4
./configure --prefix="$PREFIX"
make -j4
make -j4 install
cd ..
rm -rf dirac-research

