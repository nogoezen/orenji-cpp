@echo off
mkdir -p build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=.. -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF -DBUILD_TOOLS=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_UNIT_TESTS=OFF
cmake --build . --config Release
cmake --install . --config Release
cd .. 