git clone https://chromium.googlesource.com/webm/libvpx
cd libvpx
./configure --prefix="$PREFIX" --cpu=${CPU:-core2} --enable-vp8 --enable-postproc --disable-install-docs --disable-examples
make -j4
make -j4 install
cd ..
rm -rf libvpx

