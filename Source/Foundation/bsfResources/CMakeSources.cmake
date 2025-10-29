set(BS_RESOURCES_INC_NOFILTER
	"bsfResources/BsResourcesPrerequisites.h"
)

set(BS_RESOURCES_INC_UTILITY
	"bsfResources/Utility/BsResourceUtility.h"
)

set(BS_RESOURCES_SRC_UTILITY
	"bsfResources/Utility/BsResourceUtility.cpp"
)

set(BS_RESOURCES_INC_RESOURCES
	"bsfResources/Resources/BsResources.h"
	"bsfResources/Resources/BsResourceManifest.h"
	"bsfResources/Resources/BsResourceHandle.h"
	"bsfResources/Resources/BsResource.h"
	"bsfResources/Resources/BsGpuResourceData.h"
	"bsfResources/Resources/BsResourceMetaData.h"
	"bsfResources/Resources/BsResourceListenerManager.h"
	"bsfResources/Resources/BsSavedResourceData.h"
	"bsfResources/Resources/BsIResourceListener.h"
)

set(BS_RESOURCES_SRC_RESOURCES
	"bsfResources/Resources/BsGpuResourceData.cpp"
	"bsfResources/Resources/BsResource.cpp"
	"bsfResources/Resources/BsResourceHandle.cpp"
	"bsfResources/Resources/BsResourceManifest.cpp"
	"bsfResources/Resources/BsResources.cpp"
	"bsfResources/Resources/BsResourceMetaData.cpp"
	"bsfResources/Resources/BsResourceListenerManager.cpp"
	"bsfResources/Resources/BsSavedResourceData.cpp"
	"bsfResources/Resources/BsIResourceListener.cpp"
)

set(BS_RESOURCES_INC_RTTI
	"bsfResources/Private/RTTI/BsGpuResourceDataRTTI.h"
	"bsfResources/Private/RTTI/BsResourceHandleRTTI.h"
	"bsfResources/Private/RTTI/BsResourceManifestRTTI.h"
	"bsfResources/Private/RTTI/BsResourceMetaDataRTTI.h"
	"bsfResources/Private/RTTI/BsResourceRTTI.h"
	"bsfResources/Private/RTTI/BsSavedResourceDataRTTI.h"
)

source_group("" FILES ${BS_RESOURCES_INC_NOFILTER})
source_group("Utility" FILES ${BS_RESOURCES_INC_UTILITY} ${BS_RESOURCES_SRC_UTILITY})
source_group("Resources" FILES ${BS_RESOURCES_INC_RESOURCES} ${BS_RESOURCES_SRC_RESOURCES})
source_group("Private\\RTTI" FILES ${BS_RESOURCES_INC_RTTI})

set(BS_RESOURCES_SRC
	${BS_RESOURCES_INC_NOFILTER}
	${BS_RESOURCES_INC_UTILITY}
	${BS_RESOURCES_SRC_UTILITY}
	${BS_RESOURCES_INC_RESOURCES}
	${BS_RESOURCES_SRC_RESOURCES}
	${BS_RESOURCES_INC_RTTI}
)
