project(ConsoleApp)

#
# Source files.
#

set(HEADERS
	)

set(SOURCES
	src/Main.cpp
	)

set(MISC
	src/pch.h)

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})
source_group("" FILES ${MISC})

include_directories(
	${CMAKE_SOURCE_DIR}/modules/BackupManager/include
	include)

#
# Target.
#

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

qt_set_dir_for(Qt5Core)

find_package(Qt5Core)

add_executable(${PROJECT_NAME}
	${MISC}
	${HEADERS}
	${SOURCES})

target_link_libraries(${PROJECT_NAME}
	BackupManager
	Qt5::Core)

#Precompiled headers.
set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "src/pch.h")
cotire(${PROJECT_NAME})

qt_copy_dll_for(Qt5Core)
