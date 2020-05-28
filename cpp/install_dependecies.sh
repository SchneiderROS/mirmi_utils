#!/bin/sh -e

sudo apt-get install -y libeigen3-dev fping

# http lib
git clone https://github.com/yhirose/cpp-httplib.git
cd cpp-httplib
git checkout v0.6.5
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc --all)
sudo make install

cd ../..
sudo rm -r cpp-httplib

# nlohman json

git clone https://github.com/nlohmann/json.git
cd json
git checkout v3.7.3
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc --all)
sudo make install

cd ../..
sudo rm -r json

# json rpc
git clone https://github.com/jsonrpcx/json-rpc-cxx.git
cd json-rpc-cxx
git checkout v0.1.0
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc --all)
sudo make install

cd ../..
sudo rm -r json-rpc-cxx

# websocket
git clone https://gitlab.com/eidheim/Simple-WebSocket-Server.git
cd Simple-WebSocket-Server
git checkout v2.0.0
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc --all)
sudo make install

cd ../..
sudo rm -r Simple-WebSocket-Server
