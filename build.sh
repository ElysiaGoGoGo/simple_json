
cmake --preset=debug && cmake --build build && cd build && ctest -C Debug
if [ $? -ne 0 ];then
rm -rf build/*
cmake --preset=debug
cmake --build build 
ctest -C Debug
fi
