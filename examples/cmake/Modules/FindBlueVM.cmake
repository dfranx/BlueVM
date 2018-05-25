find_library(BLUEVM_LIBRARY
			 NAMES BlueVM
			 PATH_SUFFIXES lib64 lib32 lib ./ bin
			 PATHS ${BLUEVM_PATH})


set(BLUEVM_INCLUDE_DIR ${BLUEVM_PATH}/inc)

if (BLUEVM_LIBRARY) 
	set(BLUEVM_FOUND TRUE)
	message("Found BlueVM: ${BLUEVM_LIBRARY}")
else()
	set(BLUEVM_FOUND FALSE)
	message(FATAL_ERROR "Could not find BlueVM.")
endif()