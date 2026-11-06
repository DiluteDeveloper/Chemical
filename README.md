# Info

This is a fork/continuation of the Minecraft clone
OpenGL C++ project from two years ago (in branch mc),
Modified to run in the browser with Emscripten.

# Configure and build (web)

## Release

root/ : emcmake cmake -S . -B build/ -DCMAKE_BUILD_TYPE=RELEASE && cd build && cmake --build . --target ChemicalMC_wasm

## Debug

root/ : emcmake cmake -S . -B build/ -DCMAKE_BUILD_TYPE=DEBUG && cd build && cmake --build . --target ChemicalMC_wasm

# Configure and build (native)

## Release

root/ : cmake -S . -B build_native/ -DCMAKE_BUILD_TYPE=RELEASE && cd build_native && cmake --build . --target ChemicalMC_native

## Debug

root/ : cmake -S . -B build_native/ -DCMAKE_BUILD_TYPE=DEBUG && cd build_native && cmake --build . --target ChemicalMC_native

## Ubuntu packages required to build Dawn

sudo apt-get install libxrandr-dev libxinerama-dev libxcursor-dev mesa-common-dev libx11-xcb-dev pkg-config nodejs npm

# Host web server locally (python3 or python cmd)

root/ : python3 -m http.server
