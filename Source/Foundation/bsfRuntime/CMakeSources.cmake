set(BS_RUNTIME_INC_RUNTIME
	"bsfRuntime/BsApplication.h"
	"bsfRuntime/BsCoreApplication.h"
	"bsfRuntime/BsEntry.h"
)

set(BS_RUNTIME_SRC_RUNTIME
	"bsfRuntime/BsApplication.cpp"
	"bsfRuntime/BsCoreApplication.cpp"
)

set(BS_RUNTIME_INC_ENGINE
	"bsfRuntime/Engine/BsPlainText.h"
	"bsfRuntime/Engine/BsPlainTextImporter.h"
	"bsfRuntime/Engine/BsGameResourceManager.h"
	"bsfRuntime/Engine/BsEngineShaderIncludeHandler.h"
	"bsfRuntime/Engine/BsBuiltinResourcesHelper.h"
	"bsfRuntime/Engine/BsRuntimeBuiltinResources.h"
)

set(BS_RUNTIME_SRC_ENGINE
	"bsfRuntime/Engine/BsPlainText.cpp"
	"bsfRuntime/Engine/BsPlainTextImporter.cpp"
	"bsfRuntime/Engine/BsGameResourceManager.cpp"
	"bsfRuntime/Engine/BsEngineShaderIncludeHandler.cpp"
	"bsfRuntime/Engine/BsBuiltinResourcesHelper.cpp"
	"bsfRuntime/Engine/BsRuntimeBuiltinResources.cpp"
)

set(BS_RUNTIME_INC_UTILITY
	"bsfRuntime/Utility/BsGameSettings.h"
)

set(BS_RUNTIME_SRC_UTILITY
	"bsfRuntime/Utility/BsGameSettings.cpp"
)

set(BS_RUNTIME_INC_RTTI
	"bsfRuntime/Private/RTTI/BsPlainTextRTTI.h"
	"bsfRuntime/Private/RTTI/BsGameSettingsRTTI.h"
	"bsfRuntime/Private/RTTI/BsResourceMappingRTTI.h"
)

set(BS_RUNTIME_INC_NOFILTER
	"bsfRuntime/BsRuntimePrerequisites.h"
)

source_group("Runtime" FILES ${BS_RUNTIME_INC_RUNTIME} ${BS_RUNTIME_SRC_RUNTIME})
source_group("Engine" FILES ${BS_RUNTIME_INC_ENGINE} ${BS_RUNTIME_SRC_ENGINE})
source_group("Utility" FILES ${BS_RUNTIME_INC_UTILITY} ${BS_RUNTIME_SRC_UTILITY})
source_group("RTTI" FILES ${BS_RUNTIME_INC_RTTI})
source_group("" FILES ${BS_RUNTIME_INC_NOFILTER})

set(BS_RUNTIME_SRC
	${BS_RUNTIME_INC_RUNTIME}
	${BS_RUNTIME_SRC_RUNTIME}
	${BS_RUNTIME_INC_ENGINE}
	${BS_RUNTIME_SRC_ENGINE}
	${BS_RUNTIME_INC_UTILITY}
	${BS_RUNTIME_SRC_UTILITY}
	${BS_RUNTIME_INC_RTTI}
	${BS_RUNTIME_INC_NOFILTER}
)
