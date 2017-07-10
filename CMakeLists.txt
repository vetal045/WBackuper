cmake_minimum_required(VERSION 3.7)

project(Backuper)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/thirdparty/cmake")

include(cotire)
include(qtloader)

# Limit configurations to Debug and Release only (for multi-configuration CMake generators).
if (CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configurations" FORCE)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(QT_CMAKE_DIR "$ENV{QTDIR}/lib/cmake")

#
# Project subdirectories.
#

add_subdirectory(modules/BackupManager)
add_subdirectory(modules/ConsoleApp)
add_subdirectory(modules/GuiApp)