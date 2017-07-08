function(qt_set_dir_for module_name)
  set("${module_name}_DIR" "${QT_CMAKE_DIR}/lib/cmake/${module_name}" PARENT_SCOPE)
endfunction()

function(qt_copy_dll_for module_name)
	file(COPY "$ENV{QTDIR}/bin/${module_name}${_QT_DLL_POSTFIX}d.dll" 
		 DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
	
	file(COPY "$ENV{QTDIR}/bin/${module_name}${_QT_DLL_POSTFIX}.dll" 
		 DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release")
endfunction()