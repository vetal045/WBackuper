project(BackupManager)

#
# Source files.
#

set(HEADERS
	include/BackupManager.h
	include/BackupProfile.h
	include/ConfigManager.h
	include/StreamDevice.h
	)

set(MISC
	src/pch.h)
	
set(SOURCES
	src/BackupManager.cpp
	src/BackupProfile.cpp
	src/ConfigManager.cpp
	src/StreamDevice.cpp
	)

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})
source_group("" FILES ${MISC})

include_directories(include)

#
# Target.
#

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

qt_set_dir_for(Qt5Core)

find_package(Qt5Core)

add_library(${PROJECT_NAME}
	${HEADERS}
	${MISC}
	${SOURCES})
	
target_link_libraries(${PROJECT_NAME}
	Qt5::Core)

# Precompiled headers.
set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "src/pch.h")
cotire(${PROJECT_NAME})

qt_copy_dll_for(Qt5Core)

