#!/bin/sh -e

sudo apt-get install -y libeigen3-dev fping
sudo apt-get install libssl-dev libsasl2-dev

ROOT=$(dirname "$(realpath $0)")/third_party

mkdir -p ${ROOT}

# install json
cd ${ROOT}
if [ ! -d "json" ]
then
git clone https://github.com/nlohmann/json.git
fi
cd json
git checkout v3.7.3
mkdir -p build
cd build
cmake -DJSON_BuildTests=OFF -DCMAKE_INSTALL_PREFIX=${ROOT} ..
make -j$(nproc --all)
make install

#install http-lib
cd ${ROOT}
if [ ! -d "cpp-httplib" ]
then
git clone https://github.com/yhirose/cpp-httplib.git
fi
cd cpp-httplib
git checkout v0.6.6
mkdir -p build
cd build
cmake -DHTTPLIB_COMPILE=ON -DCMAKE_INSTALL_PREFIX=${ROOT} ..
make -j$(nproc --all)
make install

#install json-rpccxx
cd ${ROOT}
if [ ! -d "json-rpc-cxx" ]
then
git clone https://github.com/jsonrpcx/json-rpc-cxx.git
fi
cd json-rpc-cxx
git checkout v0.1.0
mkdir -p build
cd build
cmake -DCOMPILE_TESTS=OFF -DCOMPILE_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX=${ROOT} ..
make -j$(nproc --all)
make install

#install websocket
cd ${ROOT}
if [ ! -d "Simple-WebSocket-Server" ]
then
git clone https://gitlab.com/eidheim/Simple-WebSocket-Server.git
fi
cd Simple-WebSocket-Server
git checkout v2.0.0
mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX=${ROOT} ..
make -j$(nproc --all)
make install


