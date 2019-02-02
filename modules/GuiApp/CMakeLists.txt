project(GuiApp)


#

# Source files.

#



set(HEADERS
	
	include/ProfileTab.h
	
	include/MainWindow.h
	
	include/QWizardNewProfile.h

	include/ProfilesFolder.h
	include/BackupHelper.h
	include/EditProfile.h
	include/InformationTab.h
	)



set(MISC
	
	src/pch.h
	)
	


set(SOURCES
	
	src/Main.cpp
	
	src/ProfileTab.cpp
	
	src/MainWindow.cpp
	
	src/QWizardNewProfile.cpp
	
	src/ProfilesFolder.cpp

	src/BackupHelper.cpp

	src/EditProfile.cpp
	src/InformationTab.cpp
	)



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

qt_set_dir_for(Qt5Widgets)

qt_set_dir_for(Qt5Gui)



find_package(Qt5Core)

find_package(Qt5Widgets)

find_package(Qt5Gui)



add_executable(${PROJECT_NAME}
	
		${HEADERS}
	
		${MISC}
	
		${SOURCES})
	


target_link_libraries(${PROJECT_NAME}
	
	BackupManager
	
	Qt5::Core)


target_link_libraries(${PROJECT_NAME}
	
	Qt5::Widgets)

target_link_libraries(${PROJECT_NAME}
	
	Qt5::Gui)



# Precompiled headers.

set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "src/pch.h")

cotire(${PROJECT_NAME})



qt_copy_dll_for(Qt5Core)

qt_copy_dll_for(Qt5Widgets)

qt_copy_dll_for(Qt5Gui)