#/bin/bash
cmake --preset=linux_debug && cmake --build build && cd build && ctest -C Debug

