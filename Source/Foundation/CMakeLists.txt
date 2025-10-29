# Source files and their filters
include(bsfUtility/CMakeSources.cmake)
include(bsfCoreObject/CMakeSources.cmake)
include(bsfResources/CMakeSources.cmake)
include(bsfCore/CMakeSources.cmake)

# Ensure pre-built binary dependencies are up to date
if(USE_BUNDLED_LIBRARIES)
	if(EXPERIMENTAL_ENABLE_NETWORKING)
		set(BS_RAKNET_PREBUILT_DEPENDENCY_VERSION 1) # Tick this when you change the binaries
		check_and_update_binary_deps(bsfDep_RakNet RakNet ${BSF_SOURCE_DIR}/../Dependencies/RakNet ${BS_RAKNET_PREBUILT_DEPENDENCY_VERSION})
	endif()
endif()

# Packages
find_package(snappy REQUIRED)
find_package(nvtt REQUIRED)

if(EXPERIMENTAL_ENABLE_NETWORKING)
	find_package(RakNet REQUIRED)
endif()

if(LINUX)
	find_package(X11 REQUIRED)
	find_package(LibUUID REQUIRED)
	find_package(LibICU REQUIRED)

	if(NOT X11_Xcursor_FOUND)
		message(FATAL_ERROR "Could not find XCursor library.")
	endif()

	if(NOT X11_Xrandr_FOUND)
		message(FATAL_ERROR "Could not find XRandR library.")
	endif()

	if(NOT X11_Xi_FOUND)
		message(FATAL_ERROR "Could not find Xi (XInput) library.")
	endif()
elseif(APPLE)
	find_package(LibUUID REQUIRED)
endif()

# Third party (non-package) libraries
add_library(ThirdParty INTERFACE)
target_include_directories(ThirdParty INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfUtility/ThirdParty>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfUtility/ThirdParty>)

# bsfUtility Target
add_library(bsfUtility SHARED ${BS_UTILITY_SRC})
install_dll_on_build(bsfUtility ${PROJECT_SOURCE_DIR})

add_common_flags(bsfUtility)

# bsfUtility Includes
target_include_directories(bsfUtility PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfUtility>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfUtility>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfUtility>)

# bsfUtility Defines
target_compile_definitions(bsfUtility PRIVATE
	-DBS_UTILITY_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfCoreObject Target
add_library(bsfCoreObject SHARED ${BS_COREOBJECT_SRC})
install_dll_on_build(bsfCoreObject ${PROJECT_SOURCE_DIR})

add_common_flags(bsfCoreObject)

# bsfCoreObject Includes
target_include_directories(bsfCoreObject PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfCoreObject>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfCoreObject>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfCoreObject>)

# bsfCoreObject Defines
target_compile_definitions(bsfCoreObject PRIVATE
	-DBS_COREOBJECT_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfCoreObject depends on bsfUtility
target_link_libraries(bsfCoreObject PUBLIC bsfUtility)

# bsfCore Target
add_library(bsfCore SHARED ${BS_CORE_SRC} ${BS_ENGINE_SRC})
install_dll_on_build(bsfCore ${PROJECT_SOURCE_DIR})

add_common_flags(bsfCore)

# bsfCore Includes
target_include_directories(bsfCore PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfCore>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfCoreObject>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfResources>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfCore>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfCore>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfCoreObject>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfResources>)

# bsfCore Defines
target_compile_definitions(bsfCore PRIVATE
	-DBS_CORE_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfResources Target
add_library(bsfResources SHARED ${BS_RESOURCES_SRC})
install_dll_on_build(bsfResources ${PROJECT_SOURCE_DIR})

add_common_flags(bsfResources)

# bsfResources Includes
target_include_directories(bsfResources PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfResources>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfCoreObject>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfResources>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfResources>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfCoreObject>)

# bsfResources Defines
target_compile_definitions(bsfResources PRIVATE
	-DBS_RESOURCES_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfResources depends on bsfCoreObject
target_link_libraries(bsfResources PUBLIC bsfCoreObject)

# bsfCore depends on bsfResources (which depends on bsfCoreObject which depends on bsfUtility)
target_link_libraries(bsfCore PUBLIC bsfResources)

# Compiler options for all libraries
if(MSVC)
	if(NOT BSF_ENABLE_RTTI)
		target_compile_options(bsfUtility PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfCoreObject PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfResources PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfCore PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
	if(NOT BSF_ENABLE_EXCEPTIONS)
		target_compile_options(bsfUtility PUBLIC
			-fno-exceptions)
		target_compile_options(bsfCoreObject PUBLIC
			-fno-exceptions)
		target_compile_options(bsfResources PUBLIC
			-fno-exceptions)
		target_compile_options(bsfCore PUBLIC
			-fno-exceptions)
	endif()
	if(NOT BSF_ENABLE_RTTI)
		target_compile_options(bsfUtility PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfCoreObject PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfResources PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfCore PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
	endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	if(NOT BSF_ENABLE_EXCEPTIONS)
		target_compile_options(bsfUtility PUBLIC
			-fno-exceptions)
		target_compile_options(bsfCoreObject PUBLIC
			-fno-exceptions)
		target_compile_options(bsfResources PUBLIC
			-fno-exceptions)
		target_compile_options(bsfCore PUBLIC
			-fno-exceptions)
	endif()
	if(NOT BSF_ENABLE_RTTI)
		target_compile_options(bsfUtility PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfCoreObject PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfResources PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfCore PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
	endif()
else()
# TODO_OTHER_COMPILERS_GO_HERE
endif()

# bsfUtility Libraries
## External lib: Snappy (used for serialization)
target_link_libraries(bsfUtility PRIVATE ${snappy_LIBRARIES})

## External libs: Header only libraries (ThirdParty is in bsfUtility)
target_link_libraries(bsfUtility PUBLIC ThirdParty)

## OS libs needed by bsfUtility
if(LINUX)
	target_link_libraries(bsfUtility PRIVATE dl pthread)
	target_link_libraries(bsfUtility PRIVATE ${LibUUID_LIBRARIES})
	target_link_libraries(bsfUtility PRIVATE ${LibICU_LIBRARIES})
elseif(APPLE)
	target_link_libraries(bsfUtility PRIVATE ${LibUUID_LIBRARIES})
	target_link_framework(bsfUtility CoreServices)
elseif(WIN32)
	target_link_libraries(bsfUtility PRIVATE DbgHelp Rpcrt4)
endif()

# bsfCore Libraries
## External lib: NVTT (used for texture compression)
target_link_libraries(bsfCore PRIVATE ${nvtt_LIBRARIES})

## External lib: RakNet (used for networking)
if(EXPERIMENTAL_ENABLE_NETWORKING)
	target_link_libraries(bsfCore PRIVATE ${RakNet_LIBRARIES})
endif()

## Platform libs
if(LINUX)
	## External lib: X11 (used for window management)
	target_link_libraries(bsfCore PUBLIC ${X11_LIBRARIES} ${X11_Xcursor_LIB} ${X11_Xrandr_LIB} ${X11_Xi_LIB})
	target_link_libraries(bsfCore PRIVATE pthread)
endif()

## OS libs
if(WIN32)
	target_link_libraries(bsfCore PRIVATE Winmm dinput8 xinput9_1_0 dxguid.lib)
	target_link_libraries(bsfCore PRIVATE IPHLPAPI)
elseif(APPLE) # MacOS
	target_link_framework(bsfCore IOKit)
	target_link_framework(bsfCore AppKit)
	target_link_framework(bsfCore Carbon)
endif()

# IDE specific
set_property(TARGET bsfUtility PROPERTY FOLDER Foundation)
set_property(TARGET bsfCore PROPERTY FOLDER Foundation)

# Install
if(NOT BS_IS_BANSHEE3D)
	install(
		DIRECTORY bsfUtility
		DESTINATION include
		FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp"
		PATTERN "Private" EXCLUDE
	)

	install(
		DIRECTORY bsfCore
		DESTINATION include
		FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp"
		PATTERN "Private" EXCLUDE
	)

	install(
		DIRECTORY ${PROJECT_BINARY_DIR}/Generated/bsfCore
		DESTINATION include
		FILES_MATCHING
		PATTERN "*.h"
	)

	install(
		DIRECTORY ${PROJECT_BINARY_DIR}/Generated/bsfUtility
		DESTINATION include
		FILES_MATCHING
		PATTERN "*.h"
	)
endif()

install_bsf_target(bsfUtility)
install_bsf_target(bsfCoreObject)
install_bsf_target(bsfResources)
install_bsf_target(bsfCore)

if(NOT BS_IS_BANSHEE3D)
	# This doesn't actually install anything as it is an imported library
	# but it is required because it is a dependency of the bsfUtility target
	install(TARGETS ThirdParty EXPORT bsf)

	# Install both libraries as part of the bsf package
	install(
		EXPORT bsf
		NAMESPACE bsf::
		FILE bsf-config.cmake
		DESTINATION lib/bsf
	)
endif()

# Precompiled headers (Cotire)
if(MSVC)
	find_windows_system_headers(windows_headers "Windows.h")
	get_filename_component(windows_system_header_dir "${windows_headers}" DIRECTORY)
	set(ignore_paths "${CMAKE_CURRENT_SOURCE_DIR};" "${windows_system_header_dir}/..")
else()
	find_clang_invalid_libc_pch_headers(clang_bad_headers)
	set(ignore_paths "${CMAKE_CURRENT_SOURCE_DIR};${X11_INCLUDE_DIR}/X11;${clang_bad_headers}")
endif()

set_directory_properties(PROPERTIES
	COTIRE_PREFIX_HEADER_IGNORE_PATH "${ignore_paths}")

conditional_cotire(bsfUtility)
conditional_cotire(bsfCore)
