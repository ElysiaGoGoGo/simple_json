#/bin/bash
cmake --preset=debug && cmake --build build && cd build && ctest -C Debug

