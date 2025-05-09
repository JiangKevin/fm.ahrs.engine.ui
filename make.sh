#
clear
reset
#
#
export FmDev=$(pwd)
echo "FmDev: ${FmDev}"
#
export fm_links_exe_flags="\
 -O3 \
 --bind \
 -l embind \
 -Wl --gc-sections \
 --fatal-warnings \
 --importMemory \
 --whole-archive \
 --allow-undefined \
 --unresolved-symbols \
 --import-undefined \
 --no-undefined \
 -fsanitize=undefined \
 -s WASM=1 \
 -s INITIAL_MEMORY=134217728 \
 -s TOTAL_MEMORY=2GB \
 -s FETCH=1 \
 -s ASYNCIFY \
 -s FULL_ES2=1 \
 -s FULL_ES3=1 \
 --no-check-features \
 -s MIN_WEBGL_VERSION=2 \
 -s MAX_WEBGL_VERSION=2 \
 -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0 \
 -l GLEW \
 -l idbfs.js \
 -l websocket.js \
 -s ALLOW_MEMORY_GROWTH=1 \
 -s BINARYEN_EXTRA_PASSES=--one-caller-inline-max-function-size=1 \
 -s NO_DISABLE_EXCEPTION_CATCHING \
 -s OFFSCREENCANVAS_SUPPORT=1 \
 -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
 -s USE_ICU=1 \
 -s USE_LIBPNG \
 -s USE_ZLIB=1 \
 -s USE_MPG123=1 \
 -s USE_LIBJPEG=1 \
 -s USE_SQLITE3=1 \
 -s USE_GIFLIB=1 \
 -s USE_BOOST_HEADERS=1 \
 -s ASSERTIONS=0 \
 -s EXPORTED_FUNCTIONS='["_main", "_malloc", "_free"]' \
 -s EXPORTED_RUNTIME_METHODS=["UTF8ToString"] \
 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap \
 -s LLD_REPORT_UNDEFINED \
 -s FORCE_FILESYSTEM \
 --profiling-funcs \
 -s PTHREAD_POOL_SIZE_STRICT=0 \
 -s PTHREAD_POOL_SIZE=0 \
 -pthread \
 -s USE_PTHREADS=1 \
 -s WASM_BIGINT \
 -Wpthreads-mem-growth \
 --shared-memory=1 \
 --preload-file ${FmDev}/Resource@/ \
  "
#
rm -rf build
emcmake cmake -S . -B build
cd build
make -j12
#
