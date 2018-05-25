cmake_minimum_required(VERSION 3.0)

project(Examples)

add_executable(simple simple.c)
add_executable(global global.c)
add_executable(parallel parallel.c)
add_executable(external_function external_function.c)
add_executable(external_object external_object.c)
add_executable(library library.c)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package(BlueVM 2 REQUIRED)

if(BLUEVM_FOUND)
	include_directories(${BLUEVM_INCLUDE_DIR})
	target_link_libraries(simple ${BLUEVM_LIBRARY})
	target_link_libraries(global ${BLUEVM_LIBRARY})
	target_link_libraries(parallel ${BLUEVM_LIBRARY})
	target_link_libraries(external_function ${BLUEVM_LIBRARY})
	target_link_libraries(external_object ${BLUEVM_LIBRARY})
	target_link_libraries(library ${BLUEVM_LIBRARY})

	if (LINK_LIBM)
		target_link_libraries(simple m)
		target_link_libraries(global m)
		target_link_libraries(parallel m)
		target_link_libraries(external_function m)
		target_link_libraries(external_object m)
		target_link_libraries(library m)
	endif()
endif()