# Build for Linux

## Dependencies

Please make sure the dependencies are installed:

```
apt-get install \
    autoconf libtool automake \
    libevent-dev zlib1g-dev libssl-dev \
    cmake
```

## Nest simulator

Create a nest simulator:

- Get ipfs from https://ipfs.io/docs/install/ and install it

```
$ ipfs init
$ ipfs daemon &
```

- Get tor from https://www.torproject.org/download/download  and unpack it

```
$ ./configure && make
```

- Start tor with `src/or/tor &`

(don't worry, it's just in client mode)
FIXME: should not allow access to anything but .onion addresses, and never exit the tor network
FIXME: should be set up with a control socket for the nest manager so it can register hidden services

## Build and run

- Clone the project with `git clone --recursive https://github.com/thehumanweb/qml-runtime`
  - If you didn't cloned the project recursively, initialize the submodules via `git submodule init && git submodule update`
- Build torsocks

```
$ cd torsocks
$ ./autogen.sh
$ ./configure --prefix=$PWD/out
$ make
$ cd ..
```

- Build the project with cmake

```
$ mkdir build && cd build
$ cmake ..
$ make
$ cd ..
```

FIXME: build torsocks with CMake

- Use torsocks to run the qml-runtime

```
TORSOCKS_CONF_FILE=$PWD/torsocks.conf LD_PRELOAD=$PWD/torsocks/out/lib/torsocks/libtorsocks.so $PWD/build/src/bin/qml-runtime file:///ipfs/QmVkPJXs54R5mLG9hWBe7Sn8oj5waqegHXSbgCWz8gXAL5/main.qml
```

