set(BS_SCENE_INC_SCENE
	"bsfScene/Scene/BsComponent.h"
	"bsfScene/Scene/BsGameObject.h"
	"bsfScene/Scene/BsGameObjectHandle.h"
	"bsfScene/Scene/BsGameObjectManager.h"
	"bsfScene/Scene/BsSceneObject.h"
	"bsfScene/Scene/BsSceneManager.h"
	"bsfScene/Scene/BsPrefab.h"
	"bsfScene/Scene/BsPrefabDiff.h"
	"bsfScene/Scene/BsPrefabUtility.h"
	"bsfScene/Scene/BsTransform.h"
	"bsfScene/Scene/BsSceneActor.h"
)

set(BS_SCENE_SRC_SCENE
	"bsfScene/Scene/BsComponent.cpp"
	"bsfScene/Scene/BsGameObject.cpp"
	"bsfScene/Scene/BsGameObjectHandle.cpp"
	"bsfScene/Scene/BsGameObjectManager.cpp"
	"bsfScene/Scene/BsSceneObject.cpp"
	"bsfScene/Scene/BsSceneManager.cpp"
	"bsfScene/Scene/BsPrefab.cpp"
	"bsfScene/Scene/BsPrefabDiff.cpp"
	"bsfScene/Scene/BsPrefabUtility.cpp"
	"bsfScene/Scene/BsTransform.cpp"
	"bsfScene/Scene/BsSceneActor.cpp"
)

set(BS_SCENE_INC_UTILITY
	"bsfScene/Utility/BsUtility.h"
)

set(BS_SCENE_SRC_UTILITY
	"bsfScene/Utility/BsUtility.cpp"
)

set(BS_SCENE_INC_RTTI
	"bsfScene/Private/RTTI/BsComponentRTTI.h"
	"bsfScene/Private/RTTI/BsGameObjectHandleRTTI.h"
	"bsfScene/Private/RTTI/BsGameObjectRTTI.h"
	"bsfScene/Private/RTTI/BsSceneObjectRTTI.h"
	"bsfScene/Private/RTTI/BsPrefabRTTI.h"
	"bsfScene/Private/RTTI/BsPrefabDiffRTTI.h"
	"bsfScene/Private/RTTI/BsTransformRTTI.h"
)

set(BS_SCENE_INC_NOFILTER
	"bsfScene/BsScenePrerequisites.h"
)

source_group("Scene" FILES ${BS_SCENE_INC_SCENE} ${BS_SCENE_SRC_SCENE})
source_group("Utility" FILES ${BS_SCENE_INC_UTILITY} ${BS_SCENE_SRC_UTILITY})
source_group("RTTI" FILES ${BS_SCENE_INC_RTTI})
source_group("" FILES ${BS_SCENE_INC_NOFILTER})

set(BS_SCENE_SRC
	${BS_SCENE_INC_SCENE}
	${BS_SCENE_SRC_SCENE}
	${BS_SCENE_INC_UTILITY}
	${BS_SCENE_SRC_UTILITY}
	${BS_SCENE_INC_RTTI}
	${BS_SCENE_INC_NOFILTER}
)
