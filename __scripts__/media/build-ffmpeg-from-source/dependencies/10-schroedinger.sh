git clone git://diracvideo.org/git/schroedinger.git
cd schroedinger
sed -i '' -E \
-e 's#^AS_NANO\([^\)]*\).*##g' \
-e 's#NVCCFLAGS="-O2 -use_fast_math"#NVCCFLAGS="-v -m64 -O2 -use_fast_math"#g' \
configure.ac
autoreconf -fvi
# CUDA support is broken (hasn't been updated since CUDA 2.0)
# ./configure --prefix="$PREFIX" --with-thread=pthread --disable-gtk-doc ${CUDA_PATH/\//--with-cuda=/}
./configure --prefix="$PREFIX" --with-thread=pthread --disable-gtk-doc
make -j4
make -j4 install
cd ..
rm -rf schroedinger

