* Make sure the following is installed:
    apt-get install \
      autoconf libtool automake \
      libevent-dev zlib1g-dev \
      cmake \
      ninja-build \
      python-yaml \
      python-mako \
      perl \
      golang 

Create a nest simulator:

* Get ipfs from https://ipfs.io/docs/install/ and install it
$ ipfs init
$ ipfs daemon &

* Get tor from https://www.torproject.org/download/download  and unpack it
* ./configure && make

* Start tor with src/or/tor &
(don't worry, it's just in client mode)
FIXME: should not allow access to anything but .onion addresses, and never exit the tor network
FIXME: should be set up with a control socket for the nest manager so it can register hidden services

Build the software and it's dependencies:

* git submodule init
* git submodule update
* Make sure the following is installed:
    apt-get install \
      autoconf libtool automake \
      libevent-dev zlib1g-dev \
      cmake \
      ninja-build \
      python-yaml \
      python-mako \
      perl \
      golang 
* cd protobuf-qml
$ ./tools/build_dependencies.py --shared
// example /home/carsten/Qt/5.5/gcc_64/lib/cmake
$ ./tools/bootstrap.py --qt5dir=/WHERE/YOUR/QT5/IS/lib/cmake
$ source ./tools/setup_env.sh
$ ninja -C out/Release
* cd ..
* cd torsocks
$ ./autogen.sh
$ ./configure --prefix=$PWD/out
$ make
$ make install
* cd ..
* /WHERE/YOUR/QT5/IS/bin/qmake 
* make 

and then try to run it:

* TORSOCKS_CONF_FILE=$PWD/torsocks.conf LD_PRELOAD=torsocks/out/lib/torsocks/libtorsocks.so ./qml-runtime file:///ipfs/QmVkPJXs54R5mLG9hWBe7Sn8oj5waqegHXSbgCWz8gXAL5/main.qml


