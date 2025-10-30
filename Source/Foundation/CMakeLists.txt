# Source files and their filters
include(bsfUtility/CMakeSources.cmake)
include(bsfPlatform/CMakeSources.cmake)
include(bsfInput/CMakeSources.cmake)
include(bsfLocalization/CMakeSources.cmake)
include(bsfProfiling/CMakeSources.cmake)
include(bsfRenderAPI/CMakeSources.cmake)
include(bsfPhysics/CMakeSources.cmake)
include(bsfRenderer/CMakeSources.cmake)
include(bsfAudio/CMakeSources.cmake)
include(bsfGUI/CMakeSources.cmake)
include(bsfNetwork/CMakeSources.cmake)
include(bsfAnimation/CMakeSources.cmake)
include(bsfModel/CMakeSources.cmake)
include(bsfScene/CMakeSources.cmake)
include(bsfRuntime/CMakeSources.cmake)
include(bsfImage/CMakeSources.cmake)
include(bsf2D/CMakeSources.cmake)
include(bsfText/CMakeSources.cmake)
include(bsfParticles/CMakeSources.cmake)
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

# bsfPlatform Target
add_library(bsfPlatform SHARED ${BS_PLATFORM_SRC})
install_dll_on_build(bsfPlatform ${PROJECT_SOURCE_DIR})

add_common_flags(bsfPlatform)

# bsfPlatform Includes
target_include_directories(bsfPlatform PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfPlatform>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfPlatform>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfPlatform>)

# bsfPlatform Defines
target_compile_definitions(bsfPlatform PRIVATE
	-DBS_PLATFORM_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfPlatform depends on bsfUtility and bsfImage
# Note: Cannot add bsfInput due to circular dependency
target_link_libraries(bsfPlatform PUBLIC bsfUtility bsfImage)

# bsfInput Target
add_library(bsfInput SHARED ${BS_INPUT_SRC})
install_dll_on_build(bsfInput ${PROJECT_SOURCE_DIR})

add_common_flags(bsfInput)

# bsfInput Includes
target_include_directories(bsfInput PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfInput>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfInput>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfInput>)

# bsfInput Defines
target_compile_definitions(bsfInput PRIVATE
	-DBS_INPUT_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfInput depends on bsfUtility and bsfPlatform
target_link_libraries(bsfInput PUBLIC bsfUtility bsfPlatform)

# bsfLocalization Target
add_library(bsfLocalization SHARED ${BS_LOCALIZATION_SRC})
install_dll_on_build(bsfLocalization ${PROJECT_SOURCE_DIR})

add_common_flags(bsfLocalization)

# bsfLocalization Includes
target_include_directories(bsfLocalization PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfLocalization>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfLocalization>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfLocalization>)

# bsfLocalization Defines
target_compile_definitions(bsfLocalization PRIVATE
	-DBS_LOCALIZATION_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfLocalization depends on bsfCore
target_link_libraries(bsfLocalization PUBLIC bsfCore)

# bsfProfiling Target
add_library(bsfProfiling SHARED ${BS_PROFILING_SRC})
install_dll_on_build(bsfProfiling ${PROJECT_SOURCE_DIR})

add_common_flags(bsfProfiling)

# bsfProfiling Includes
target_include_directories(bsfProfiling PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfProfiling>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfProfiling>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfProfiling>)

# bsfProfiling Defines
target_compile_definitions(bsfProfiling PRIVATE
	-DBS_PROFILING_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfProfiling depends on bsfUtility, bsfPlatform, and bsfRenderAPI (for GPU profiling)
target_link_libraries(bsfProfiling PUBLIC bsfUtility bsfPlatform bsfRenderAPI)

# bsfRenderAPI Target
add_library(bsfRenderAPI SHARED ${BS_RENDERAPI_SRC})
install_dll_on_build(bsfRenderAPI ${PROJECT_SOURCE_DIR})

add_common_flags(bsfRenderAPI)

# bsfRenderAPI Includes
target_include_directories(bsfRenderAPI PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfRenderAPI>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfCore>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfRenderAPI>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfRenderAPI>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfCore>)

# bsfRenderAPI Defines
target_compile_definitions(bsfRenderAPI PRIVATE
	-DBS_RENDERAPI_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfRenderAPI depends on bsfCore
target_link_libraries(bsfRenderAPI PUBLIC bsfCore)

# bsfPhysics Target
add_library(bsfPhysics SHARED ${BS_PHYSICS_SRC})
install_dll_on_build(bsfPhysics ${PROJECT_SOURCE_DIR})

add_common_flags(bsfPhysics)

# bsfPhysics Includes
target_include_directories(bsfPhysics PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfPhysics>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfPhysics>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfPhysics>)

# bsfPhysics Defines
target_compile_definitions(bsfPhysics PRIVATE
	-DBS_PHYSICS_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfPhysics depends on bsfScene (for Component base class)
target_link_libraries(bsfPhysics PUBLIC bsfScene)

# bsfRenderer Target
add_library(bsfRenderer SHARED ${BS_RENDERER_SRC})
install_dll_on_build(bsfRenderer ${PROJECT_SOURCE_DIR})

add_common_flags(bsfRenderer)

# bsfRenderer Includes
target_include_directories(bsfRenderer PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfRenderer>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfRenderer>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfRenderer>)

# bsfRenderer Defines
target_compile_definitions(bsfRenderer PRIVATE
	-DBS_RENDERER_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfRenderer depends on bsfRenderAPI and bsfScene
target_link_libraries(bsfRenderer PUBLIC bsfRenderAPI bsfScene)

# bsfAudio Target
add_library(bsfAudio SHARED ${BS_AUDIO_SRC})
install_dll_on_build(bsfAudio ${PROJECT_SOURCE_DIR})

add_common_flags(bsfAudio)

# bsfAudio Includes
target_include_directories(bsfAudio PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfAudio>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfAudio>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfAudio>)

# bsfAudio Defines
target_compile_definitions(bsfAudio PRIVATE
	-DBS_AUDIO_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfAudio depends on bsfCore and bsfScene
target_link_libraries(bsfAudio PUBLIC bsfCore bsfScene)

# bsfGUI Target
add_library(bsfGUI SHARED ${BS_GUI_SRC})
install_dll_on_build(bsfGUI ${PROJECT_SOURCE_DIR})

add_common_flags(bsfGUI)

# bsfGUI Includes
target_include_directories(bsfGUI PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfGUI>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfGUI>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfGUI>)

# bsfGUI Defines
target_compile_definitions(bsfGUI PRIVATE
	-DBS_GUI_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfGUI depends on bsfRenderAPI and bsfCore
target_link_libraries(bsfGUI PUBLIC bsfRenderAPI bsfCore)

# bsfNetwork Target
add_library(bsfNetwork SHARED ${BS_NETWORK_SRC})
install_dll_on_build(bsfNetwork ${PROJECT_SOURCE_DIR})

add_common_flags(bsfNetwork)

# bsfNetwork Includes
target_include_directories(bsfNetwork PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfNetwork>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfNetwork>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfNetwork>)

# bsfNetwork Defines
target_compile_definitions(bsfNetwork PRIVATE
	-DBS_NETWORK_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfNetwork depends on bsfUtility (for IReflectable and Module)
target_link_libraries(bsfNetwork PUBLIC bsfUtility)

# bsfAnimation Target
add_library(bsfAnimation SHARED ${BS_ANIMATION_SRC})
install_dll_on_build(bsfAnimation ${PROJECT_SOURCE_DIR})

add_common_flags(bsfAnimation)

# bsfAnimation Includes
target_include_directories(bsfAnimation PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfAnimation>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfAnimation>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfAnimation>)

# bsfAnimation Defines
target_compile_definitions(bsfAnimation PRIVATE
	-DBS_ANIMATION_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfAnimation depends on bsfCore and bsfScene
target_link_libraries(bsfAnimation PUBLIC bsfCore bsfScene)

# bsfModel Target
add_library(bsfModel SHARED ${BS_MODEL_SRC})
install_dll_on_build(bsfModel ${PROJECT_SOURCE_DIR})

add_common_flags(bsfModel)

# bsfModel Includes
target_include_directories(bsfModel PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfModel>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfModel>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfModel>)

# bsfModel Defines
target_compile_definitions(bsfModel PRIVATE
	-DBS_MODEL_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfModel depends on bsfUtility
target_link_libraries(bsfModel PUBLIC bsfUtility)

# bsfRuntime Target
add_library(bsfRuntime SHARED ${BS_RUNTIME_SRC})
install_dll_on_build(bsfRuntime ${PROJECT_SOURCE_DIR})

add_common_flags(bsfRuntime)

# bsfRuntime Includes
target_include_directories(bsfRuntime PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfRuntime>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfRuntime>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfRuntime>)

# bsfRuntime Defines
target_compile_definitions(bsfRuntime PRIVATE
	-DBS_RUNTIME_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfRuntime depends on bsfCore (Application classes need Core functionality)
# Note: This dependency is defined after bsfCore target is created

# bsfImage Target
add_library(bsfImage SHARED ${BS_IMAGE_SRC})
install_dll_on_build(bsfImage ${PROJECT_SOURCE_DIR})

add_common_flags(bsfImage)

# bsfImage Includes
target_include_directories(bsfImage PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfImage>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfImage>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfImage>)

# bsfImage Defines
target_compile_definitions(bsfImage PRIVATE
	-DBS_IMAGE_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfImage depends on bsfUtility and bsfCore (for GpuResourceData)
target_link_libraries(bsfImage PUBLIC bsfUtility bsfCore)

# bsf2D Target
add_library(bsf2D SHARED ${BS_2D_SRC})
install_dll_on_build(bsf2D ${PROJECT_SOURCE_DIR})

add_common_flags(bsf2D)

# bsf2D Includes
target_include_directories(bsf2D PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsf2D>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsf2D>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsf2D>)

# bsf2D Defines
target_compile_definitions(bsf2D PRIVATE
	-DBS_2D_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsf2D depends on bsfUtility and bsfImage
target_link_libraries(bsf2D PUBLIC bsfUtility bsfImage)

# bsfText Target
add_library(bsfText SHARED ${BS_TEXT_SRC})
install_dll_on_build(bsfText ${PROJECT_SOURCE_DIR})

add_common_flags(bsfText)

# bsfText Includes
target_include_directories(bsfText PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfText>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfText>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfText>)

# bsfText Defines
target_compile_definitions(bsfText PRIVATE
	-DBS_TEXT_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfText depends on bsfCore
target_link_libraries(bsfText PUBLIC bsfCore)

# bsfParticles Target
add_library(bsfParticles SHARED ${BS_PARTICLES_SRC})
install_dll_on_build(bsfParticles ${PROJECT_SOURCE_DIR})

add_common_flags(bsfParticles)

# bsfParticles Includes
target_include_directories(bsfParticles PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfParticles>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfParticles>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfParticles>)

# bsfParticles Defines
target_compile_definitions(bsfParticles PRIVATE
	-DBS_PARTICLES_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfParticles depends on bsfScene
target_link_libraries(bsfParticles PUBLIC bsfScene)

# bsfCore Target
add_library(bsfCore SHARED ${BS_CORE_SRC} ${BS_ENGINE_SRC})
install_dll_on_build(bsfCore ${PROJECT_SOURCE_DIR})

add_common_flags(bsfCore)

# bsfCore Includes
target_include_directories(bsfCore PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfCore>
	
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfScene>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfCore>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfCore>
	
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfScene>)

# bsfCore Defines
target_compile_definitions(bsfCore PRIVATE
	-DBS_CORE_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfScene Target
add_library(bsfScene SHARED ${BS_SCENE_SRC})
install_dll_on_build(bsfScene ${PROJECT_SOURCE_DIR})

add_common_flags(bsfScene)

# bsfScene Includes
target_include_directories(bsfScene PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bsfScene>
	
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfScene>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfScene>
	)

# bsfScene Defines
target_compile_definitions(bsfScene PRIVATE
	-DBS_SCENE_EXPORTS
	$<$<CONFIG:Debug>:BS_CONFIG=BS_CONFIG_DEBUG>
	$<$<CONFIG:RelWithDebInfo>:BS_CONFIG=BS_CONFIG_RELWITHDEBINFO>
	$<$<CONFIG:MinSizeRel>:BS_CONFIG=BS_CONFIG_MINSIZEREL>
	$<$<CONFIG:Release>:BS_CONFIG=BS_CONFIG_RELEASE>)

# bsfScene depends on bsfCore
target_link_libraries(bsfScene PUBLIC bsfCore)

# bsfCore depends on all subsystem libraries
target_link_libraries(bsfCore PUBLIC
	bsfScene
	bsfRenderAPI
	bsfPhysics
	bsfRenderer
	bsfAudio
	bsfGUI
	bsfNetwork
	bsfAnimation
	bsfPlatform
	bsfInput
	bsfImporter
	bsfLocalization
	bsfProfiling)

# bsfRuntime depends on bsfCore (defined here after bsfCore is created)
target_link_libraries(bsfRuntime PUBLIC bsfCore)

# Compiler options for all libraries
if(MSVC)
	if(NOT BSF_ENABLE_RTTI)
		target_compile_options(bsfUtility PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfPlatform PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfRenderAPI PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfPhysics PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfRenderer PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfAudio PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfGUI PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfNetwork PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfAnimation PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfRuntime PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfScene PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfImage PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsf2D PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfText PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfParticles PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
		target_compile_options(bsfCore PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:/GR->)
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
	if(NOT BSF_ENABLE_EXCEPTIONS)
		target_compile_options(bsfUtility PUBLIC
			-fno-exceptions)
		target_compile_options(bsfPlatform PUBLIC
			-fno-exceptions)
		target_compile_options(bsfRenderAPI PUBLIC
			-fno-exceptions)
		target_compile_options(bsfPhysics PUBLIC
			-fno-exceptions)
		target_compile_options(bsfRenderer PUBLIC
			-fno-exceptions)
		target_compile_options(bsfAudio PUBLIC
			-fno-exceptions)
		target_compile_options(bsfGUI PUBLIC
			-fno-exceptions)
		target_compile_options(bsfNetwork PUBLIC
			-fno-exceptions)
		target_compile_options(bsfAnimation PUBLIC
			-fno-exceptions)
		target_compile_options(bsfRuntime PUBLIC
			-fno-exceptions)
			-fno-exceptions)
		target_compile_options(bsfScene PUBLIC
			-fno-exceptions)
		target_compile_options(bsfImage PUBLIC
			-fno-exceptions)
		target_compile_options(bsf2D PUBLIC
			-fno-exceptions)
		target_compile_options(bsfText PUBLIC
			-fno-exceptions)
		target_compile_options(bsfParticles PUBLIC
			-fno-exceptions)
		target_compile_options(bsfCore PUBLIC
			-fno-exceptions)
	endif()
	if(NOT BSF_ENABLE_RTTI)
		target_compile_options(bsfUtility PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfPlatform PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfRenderAPI PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfPhysics PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfRenderer PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfAudio PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfGUI PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfNetwork PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfAnimation PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfRuntime PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfScene PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfImage PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsf2D PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfText PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfParticles PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfCore PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
	endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	if(NOT BSF_ENABLE_EXCEPTIONS)
		target_compile_options(bsfUtility PUBLIC
			-fno-exceptions)
		target_compile_options(bsfPlatform PUBLIC
			-fno-exceptions)
		target_compile_options(bsfRenderAPI PUBLIC
			-fno-exceptions)
		target_compile_options(bsfPhysics PUBLIC
			-fno-exceptions)
		target_compile_options(bsfRenderer PUBLIC
			-fno-exceptions)
		target_compile_options(bsfAudio PUBLIC
			-fno-exceptions)
		target_compile_options(bsfGUI PUBLIC
			-fno-exceptions)
		target_compile_options(bsfNetwork PUBLIC
			-fno-exceptions)
		target_compile_options(bsfAnimation PUBLIC
			-fno-exceptions)
		target_compile_options(bsfRuntime PUBLIC
			-fno-exceptions)
			-fno-exceptions)
		target_compile_options(bsfScene PUBLIC
			-fno-exceptions)
		target_compile_options(bsfImage PUBLIC
			-fno-exceptions)
		target_compile_options(bsf2D PUBLIC
			-fno-exceptions)
		target_compile_options(bsfText PUBLIC
			-fno-exceptions)
		target_compile_options(bsfParticles PUBLIC
			-fno-exceptions)
		target_compile_options(bsfCore PUBLIC
			-fno-exceptions)
	endif()
	if(NOT BSF_ENABLE_RTTI)
		target_compile_options(bsfUtility PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfPlatform PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfRenderAPI PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfPhysics PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfRenderer PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfAudio PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfGUI PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfNetwork PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfAnimation PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfRuntime PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfScene PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfImage PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsf2D PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfText PUBLIC
			$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
		target_compile_options(bsfParticles PUBLIC
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

# bsfPlatform Libraries
## Platform libs
if(LINUX)
	## External lib: X11 (used for window management)
	target_link_libraries(bsfPlatform PUBLIC ${X11_LIBRARIES} ${X11_Xcursor_LIB} ${X11_Xrandr_LIB} ${X11_Xi_LIB})
elseif(APPLE)
	target_link_framework(bsfPlatform IOKit)
	target_link_framework(bsfPlatform AppKit)
	target_link_framework(bsfPlatform Carbon)
elseif(WIN32)
	target_link_libraries(bsfPlatform PRIVATE Winmm)
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
set_property(TARGET bsfPlatform PROPERTY FOLDER Foundation)
set_property(TARGET bsfRenderAPI PROPERTY FOLDER Foundation)
set_property(TARGET bsfPhysics PROPERTY FOLDER Foundation)
set_property(TARGET bsfRenderer PROPERTY FOLDER Foundation)
set_property(TARGET bsfAudio PROPERTY FOLDER Foundation)
set_property(TARGET bsfGUI PROPERTY FOLDER Foundation)
set_property(TARGET bsfNetwork PROPERTY FOLDER Foundation)
set_property(TARGET bsfAnimation PROPERTY FOLDER Foundation)
set_property(TARGET bsfRuntime PROPERTY FOLDER Foundation)
set_property(TARGET bsfScene PROPERTY FOLDER Foundation)
set_property(TARGET bsfImage PROPERTY FOLDER Foundation)
set_property(TARGET bsf2D PROPERTY FOLDER Foundation)
set_property(TARGET bsfText PROPERTY FOLDER Foundation)
set_property(TARGET bsfParticles PROPERTY FOLDER Foundation)
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
install_bsf_target(bsfPlatform)
install_bsf_target(bsfInput)
install_bsf_target(bsfLocalization)
install_bsf_target(bsfProfiling)
install_bsf_target(bsfRenderAPI)
install_bsf_target(bsfPhysics)
install_bsf_target(bsfRenderer)
install_bsf_target(bsfAudio)
install_bsf_target(bsfGUI)
install_bsf_target(bsfNetwork)
install_bsf_target(bsfAnimation)
install_bsf_target(bsfRuntime)
install_bsf_target(bsfScene)
install_bsf_target(bsfImage)
install_bsf_target(bsf2D)
install_bsf_target(bsfText)
install_bsf_target(bsfParticles)
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
conditional_cotire(bsfPlatform)
conditional_cotire(bsfInput)
conditional_cotire(bsfLocalization)
conditional_cotire(bsfProfiling)
conditional_cotire(bsfRenderAPI)
conditional_cotire(bsfPhysics)
conditional_cotire(bsfRenderer)
conditional_cotire(bsfAudio)
conditional_cotire(bsfGUI)
conditional_cotire(bsfNetwork)
conditional_cotire(bsfAnimation)
conditional_cotire(bsfRuntime)
conditional_cotire(bsfScene)
conditional_cotire(bsfImage)
conditional_cotire(bsf2D)
conditional_cotire(bsfText)
conditional_cotire(bsfParticles)
conditional_cotire(bsfCore)
