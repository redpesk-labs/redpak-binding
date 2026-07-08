###########################################################################
# Copyright (C) 2021-2026 IoT.bzh Company
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

# Configure api description
configure_file(redpak-binding-info.json.in redpak-binding-info.json)
add_custom_command(
	OUTPUT redpak-binding-apidef.h
	COMMAND ${AFB-GENSKEL} redpak-binding-info.json > redpak-binding-apidef.h
	DEPENDS redpak-binding-info.json)
add_custom_target(apidef DEPENDS redpak-binding-apidef.h)

# Define project Target
add_library(redpak-binding MODULE redpak-binding.c utils.c)
add_dependencies(redpak-binding apidef)
target_include_directories(redpak-binding PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Link to necessary lib
target_link_libraries(redpak-binding PkgConfig::deps)
set_target_properties(redpak-binding PROPERTIES PREFIX "")

target_compile_definitions(
	redpak-binding
	PRIVATE
	PROJECT_NAME="${PROJECT_NAME}"
	PROJECT_VERSION="${PROJECT_VERSION}"
	PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
	PROJECT_AUTHOR="${PROJECT_AUTHOR}"
)
# Binder exposes a unique public entry point
install(TARGETS redpak-binding DESTINATION ${APPDIR}/lib)

