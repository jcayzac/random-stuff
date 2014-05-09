CUDA_PATH="$(which nvcc 2>/dev/null || true)"
if [ ! -z "$CUDA_PATH" ]
then
	CUDA_PATH="${CUDA_PATH%%\/bin\/nvcc}"
elif [ -d "/Developer/NVIDIA" ]
then
	CUDA_PATH="$(find /Developer/NVIDIA -type d -name "CUDA-*" -maxdepth 1)"
fi

if [ ! -z "$CUDA_PATH" ]
then
	echo "Found CUDA at $CUDA_PATH"
	for L in gcc g++
	do
		for V in 8 7 6 5 4 3 2
		do
			for G in ${L}-mp-4.$V ${L}-4.$V ${L}-4$V ${L}4$V llvm-${L}-4.$V
			do
				which $G >/dev/null 2>&1 && break || true
			done
			which $G >/dev/null 2>&1 && break || true
		done
		if ! which $G >/dev/null 2>&1
		then
			echo "No suitable compiler found. Installing GCC 4.8..."
			sudo port install gcc48
			V=8
			G=${L}-mp-4.$V
		fi
		if [ "gcc" == "$L" ]
		then
			export GCC_FOR_CUDA="$(which $G)"
		else
			export GXX_FOR_CUDA="$(which $G)"
		fi
	done

	[ -x "$CUDA_PATH"/bin/gcc ] && [ "$GCC_FOR_CUDA" != "$(readlink "$CUDA_PATH"/bin/gcc)" ] && sudo rm "$CUDA_PATH"/bin/gcc
	[ -x "$CUDA_PATH"/bin/g++ ] && [ "$GXX_FOR_CUDA" != "$(readlink "$CUDA_PATH"/bin/g++)" ] && sudo rm "$CUDA_PATH"/bin/g++
	[ -x "$CUDA_PATH"/bin/gcc ] || sudo ln -sf "$GCC_FOR_CUDA" "$CUDA_PATH"/bin/gcc
	[ -x "$CUDA_PATH"/bin/g++ ] || sudo ln -sf "$GXX_FOR_CUDA" "$CUDA_PATH"/bin/g++

	export CUDA_PATH
fi

