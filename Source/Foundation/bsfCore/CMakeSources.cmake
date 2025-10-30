set(BS_CORE_SRC_RTTI
	"bsfCore/Private/RTTI/BsGUISkinRTTI.cpp"
)

set(BS_CORE_INC_NOFILTER
	"bsfCore/BsCorePrerequisites.h"
	"bsfCore/BsPrerequisites.h"
)

set(BS_CORE_SRC_UTILITY
	"bsfCore/Utility/BsPaths.cpp"
	"bsfCore/Utility/BsResourceUtility.cpp"
)

set(BS_CORE_INC_UTILITY
	"bsfCore/Utility/BsPaths.h"
	"bsfCore/Utility/BsResourceUtility.h"
)

set(BS_CORE_INC_RESOURCES
	"bsfCore/Resources/BsResources.h"
	"bsfCore/Resources/BsResourceManifest.h"
	"bsfCore/Resources/BsResourceHandle.h"
	"bsfCore/Resources/BsResource.h"
	"bsfCore/Resources/BsGpuResourceData.h"
	"bsfCore/Resources/BsResourceMetaData.h"
	"bsfCore/Resources/BsResourceListenerManager.h"
	"bsfCore/Resources/BsSavedResourceData.h"
	"bsfCore/Resources/BsIResourceListener.h"
)

set(BS_CORE_SRC_RESOURCES
	"bsfCore/Resources/BsGpuResourceData.cpp"
	"bsfCore/Resources/BsResource.cpp"
	"bsfCore/Resources/BsResourceHandle.cpp"
	"bsfCore/Resources/BsResourceManifest.cpp"
	"bsfCore/Resources/BsResources.cpp"
	"bsfCore/Resources/BsResourceMetaData.cpp"
	"bsfCore/Resources/BsResourceListenerManager.cpp"
	"bsfCore/Resources/BsSavedResourceData.cpp"
	"bsfCore/Resources/BsIResourceListener.cpp"
)

set(BS_CORE_INC_RTTI
	"bsfCore/Private/RTTI/BsGpuResourceDataRTTI.h"
	"bsfCore/Private/RTTI/BsResourceHandleRTTI.h"
	"bsfCore/Private/RTTI/BsResourceManifestRTTI.h"
	"bsfCore/Private/RTTI/BsResourceMetaDataRTTI.h"
	"bsfCore/Private/RTTI/BsResourceRTTI.h"
	"bsfCore/Private/RTTI/BsSavedResourceDataRTTI.h"
	"bsfCore/Private/RTTI/BsImportOptionsRTTI.h"
)

set(BS_CORE_INC_IMPORTER
	"bsfCore/Importer/BsImporter.h"
	"bsfCore/Importer/BsImportOptions.h"
	"bsfCore/Importer/BsSpecificImporter.h"
)

set(BS_CORE_SRC_IMPORTER
	"bsfCore/Importer/BsImporter.cpp"
	"bsfCore/Importer/BsImportOptions.cpp"
	"bsfCore/Importer/BsSpecificImporter.cpp"
)

set(BS_CORE_SRC_NOFILTER
)

set(BS_CORE_INC_CORETHREAD
	"bsfCore/CoreThread/BsCoreThread.h"
	"bsfCore/CoreThread/BsCoreObjectManager.h"
	"bsfCore/CoreThread/BsCoreObject.h"
	"bsfCore/CoreThread/BsCommandQueue.h"
	"bsfCore/CoreThread/BsCoreObjectCore.h"
	"bsfCore/CoreThread/BsCoreObjectSync.h"
)

set(BS_CORE_SRC_CORETHREAD
	"bsfCore/CoreThread/BsCommandQueue.cpp"
	"bsfCore/CoreThread/BsCoreObject.cpp"
	"bsfCore/CoreThread/BsCoreObjectManager.cpp"
	"bsfCore/CoreThread/BsCoreThread.cpp"
	"bsfCore/CoreThread/BsCoreObjectCore.cpp"
)

set(BS_CORE_INC_PLATFORM
)

set(BS_CORE_SRC_PLATFORM
)

set(BS_CORE_INC_PLATFORM_WIN32
)

set(BS_CORE_SRC_PLATFORM_WIN32
)

set(BS_CORE_INC_PLATFORM_LINUX
	"bsfCore/Private/Linux/BsLinuxWindow.h"
)

set(BS_CORE_SRC_PLATFORM_LINUX
	"bsfCore/Private/Linux/BsLinuxWindow.cpp"
)

foreach(source ${BS_CORE_SRC_PLATFORM_LINUX} ${BS_CORE_SRC_PLATFORM_WIN32})
	set_source_files_properties ("${source}" PROPERTIES
			COTIRE_EXCLUDED "True")
endforeach()

set(BS_CORE_INC_PLATFORM_MACOS
	"bsfCore/Private/MacOS/BsMacOSWindow.h"
)

set(BS_CORE_SRC_PLATFORM_MACOS
	"bsfCore/Private/MacOS/BsMacOSWindow.mm"
)

if(MSVC)
	list(APPEND BS_CORE_INC_PLATFORM "bsfCore/VSVisualizations.natvis")
endif()

if(WIN32)
	list(APPEND BS_CORE_INC_PLATFORM ${BS_CORE_INC_PLATFORM_WIN32})
	list(APPEND BS_CORE_SRC_PLATFORM ${BS_CORE_SRC_PLATFORM_WIN32})
elseif(LINUX)
	list(APPEND BS_CORE_INC_PLATFORM ${BS_CORE_INC_PLATFORM_LINUX})
	list(APPEND BS_CORE_SRC_PLATFORM ${BS_CORE_SRC_PLATFORM_LINUX})
elseif(APPLE)
	list(APPEND BS_CORE_INC_PLATFORM ${BS_CORE_INC_PLATFORM_MACOS})
	list(APPEND BS_CORE_SRC_PLATFORM ${BS_CORE_SRC_PLATFORM_MACOS})
endif()

source_group("Platform" FILES ${BS_CORE_INC_PLATFORM} ${BS_CORE_SRC_PLATFORM})
source_group("Utility" FILES ${BS_CORE_INC_UTILITY} ${BS_CORE_SRC_UTILITY})
source_group("CoreThread" FILES ${BS_CORE_INC_CORETHREAD} ${BS_CORE_SRC_CORETHREAD})
source_group("Resources" FILES ${BS_CORE_INC_RESOURCES} ${BS_CORE_SRC_RESOURCES})
source_group("Importer" FILES ${BS_CORE_INC_IMPORTER} ${BS_CORE_SRC_IMPORTER})
source_group("Private\\RTTI" FILES ${BS_CORE_INC_RTTI})
source_group("" FILES ${BS_CORE_INC_NOFILTER} ${BS_CORE_SRC_NOFILTER})

if(APPLE)
	source_group("MacOS" FILES ${BS_CORE_INC_PLATFORM_MACOS} ${BS_CORE_SRC_PLATFORM_MACOS})
endif()

set(BS_CORE_SRC
	${BS_CORE_INC_PLATFORM}
	${BS_CORE_INC_NOFILTER}
	${BS_CORE_SRC_UTILITY}
	${BS_CORE_SRC_PLATFORM}
	${BS_CORE_INC_UTILITY}
	${BS_CORE_SRC_NOFILTER}
	${BS_CORE_INC_CORETHREAD}
	${BS_CORE_SRC_CORETHREAD}
	${BS_CORE_INC_RESOURCES}
	${BS_CORE_SRC_RESOURCES}
	${BS_CORE_INC_IMPORTER}
	${BS_CORE_SRC_IMPORTER}
	${BS_CORE_INC_RTTI}
	${BS_CORE_SRC_RTTI}
)

