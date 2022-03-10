###########################################################################
# Copyright 2021 IoT.bzh
#
# Author: Valentin Lefebvre <valentin.lefebvre@iot.bzh>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###########################################################################

# Add target to project dependency list
PROJECT_TARGET_ADD(redpak-binding)

	# Configure some files
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}-apidef.json.in 
		${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}-apidef.json
	)

	# Define project Sources
	AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} SRC_BINDING)
	AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/utils SRC_UTILS)

	# Define project Target
	ADD_LIBRARY(${TARGET_NAME} MODULE 
		${SRC_BINDING}
		${SRC_UTILS}
	)

	# Project include directories
	TARGET_INCLUDE_DIRECTORIES(${TARGET_NAME} PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/utils
	)
	
	# Binder exposes a unique public entry point
	SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
		PREFIX "afb-"
		LABELS "BINDING"
		OUTPUT_NAME ${TARGET_NAME}
	)
