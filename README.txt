# Configure

root/ : emcmake cmake -B build

# Build

root/build/ : make

# Host locally (python3 or python cmd)

root/ : python serve.py
