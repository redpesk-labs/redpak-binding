# Redpak-binding installation

## Using distribution packager manager

Add the Redpesk repository into your packager manager following this link: <download.redpesk.bzh>
Install the package **redpak-binding**

```bash
dnf install redpak-binding
```

## Building from sources

### Install dependencies

Building tools

* gcc
* g++
* make
* cmake
* afb-cmake-modules
* red-pak

Then the following dependencies:

* json-c
* afb-binding

Fedora/OpenSuse:

```bash
dnf install gcc-c++ make cmake afb-cmake-modules json-c-devel afb-binding-devel red-pak
```

> **Note**
> Because redpak can only be using on RedHat distribution with rpm management, this binding can be used and build ony on sames distributions.
> So, no availability on debian/ubuntu distributions.

### build

```bash
git clone https://github.com/redpesk-labs/redpak-binding
cd redpak-binding
export WORK_DIR="${PWD}"
mkdir build
cd build
cmake ..
make
```
