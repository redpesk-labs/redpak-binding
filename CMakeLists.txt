###########################################################################
# Copyright (C) 2022-26 IoT.bzh Company
#
# Author: IoT.bzh Dev team <dev-team@iot.bzh>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is furnished
# to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
###########################################################################

cmake_minimum_required(VERSION 3.18)
project(
	redpak-binding
	VERSION 1.0.0
	DESCRIPTION "Service which manage redpak"
	HOMEPAGE_URL  "http://github.com/redpesk-labs/redpak-binding"
	LANGUAGES C
)

# setting
set(AFM_APP_DIR ${CMAKE_INSTALL_PREFIX}/redpesk CACHE PATH "Directory of Redpesk Applications")
set(APPDIR ${AFM_APP_DIR}/redpak-binding CACHE PATH "Binding root directory")

# dependencies
include(FindPkgConfig)
include(GNUInstallDirs)
pkg_check_modules(
	deps
	REQUIRED IMPORTED_TARGET
	json-c
	librp-utils-json-c-static
	afb-binding
	red-pak>=2.3.0
)

# genskel
find_package(afb-genskel 0.5 REQUIRED)
message(STATUS "  Found afb-genskel, ${AFB-GENSKEL}")

# produce the manifest
configure_file(manifest.yaml.in manifest.yaml)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.yaml DESTINATION ${APPDIR}/.rpconfig)

# produce the library
add_subdirectory(src)

