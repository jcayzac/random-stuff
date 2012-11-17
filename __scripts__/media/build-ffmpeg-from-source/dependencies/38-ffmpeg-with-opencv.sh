git clone git://source.ffmpeg.org/ffmpeg.git
cd ffmpeg
sed -i '' -E 's#require libopenjpeg#require_pkg_config libopenjpeg#g' configure
./configure --prefix="$PREFIX" \
--enable-gpl --enable-version3 --enable-nonfree \
--enable-shared --disable-doc \
--enable-avresample \
--enable-indevs \
--enable-fontconfig \
--enable-frei0r \
--enable-libaacplus \
--enable-libass \
--enable-libbluray \
--enable-libfaac \
--enable-libfdk-aac \
--enable-libfreetype \
--enable-libmp3lame \
--enable-libopenjpeg \
--enable-libopus \
--enable-libschroedinger \
--enable-libspeex \
--enable-libtheora \
--enable-libvorbis \
--enable-libvpx \
--enable-libx264 \
--enable-libxavs \
--enable-libxvid \
--enable-libopencv \
--enable-swscale \
--enable-avfilter \
--enable-postproc \
--enable-static \
--enable-shared \
--arch=x86_64 --cpu=core2
make -j4 all alltools
make -j4 install
install -m 755 tools/qt-faststart "${PREFIX}/bin"
cd ..
rm -rf ffmpeg

