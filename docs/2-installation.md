# Redpak-binding installation

## Using distribution packager manager

Add the Redpesk repository into your packager manager following this link: <download.redpesk.bzh>
Install the package **redpak-binding**

## Building from sources

### Install dependencies

Building tools

* gcc
* g++
* make
* cmake
* afb-cmake-modules

Then the following dependencies:

* json-c
* afb-binding
* afb-libhelpers

Fedora/OpenSuse:

```bash
dnf install gcc-c++ make cmake afb-cmake-modules json-c-devel afb-binding-devel afb-libhelpers-devel
```

Ubuntu:

```bash
apt install gcc g++ make cmake afb-cmake-modules-bin libsystemd-dev libjson-c-dev afb-binding-dev afb-libhelpers-dev
```

### build

```bash
git clone http://git.ovh.iot/redpesk/redpesk-addons/wit-gps-binding
cd wit-gps-binding
export WORK_DIR="${PWD}"
mkdir build
cd build
cmake ..
make
```
