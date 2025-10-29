set(BS_COREOBJECT_INC_NOFILTER
	"bsfCoreObject/BsCoreObjectPrerequisites.h"
)

set(BS_COREOBJECT_INC_CORETHREAD
	"bsfCoreObject/CoreThread/BsCoreThread.h"
	"bsfCoreObject/CoreThread/BsCoreObjectManager.h"
	"bsfCoreObject/CoreThread/BsCoreObject.h"
	"bsfCoreObject/CoreThread/BsCommandQueue.h"
	"bsfCoreObject/CoreThread/BsCoreObjectCore.h"
	"bsfCoreObject/CoreThread/BsCoreObjectSync.h"
)

set(BS_COREOBJECT_SRC_CORETHREAD
	"bsfCoreObject/CoreThread/BsCommandQueue.cpp"
	"bsfCoreObject/CoreThread/BsCoreObject.cpp"
	"bsfCoreObject/CoreThread/BsCoreObjectManager.cpp"
	"bsfCoreObject/CoreThread/BsCoreThread.cpp"
	"bsfCoreObject/CoreThread/BsCoreObjectCore.cpp"
)

source_group("" FILES ${BS_COREOBJECT_INC_NOFILTER})
source_group("CoreThread" FILES ${BS_COREOBJECT_INC_CORETHREAD} ${BS_COREOBJECT_SRC_CORETHREAD})

set(BS_COREOBJECT_SRC
	${BS_COREOBJECT_INC_NOFILTER}
	${BS_COREOBJECT_INC_CORETHREAD}
	${BS_COREOBJECT_SRC_CORETHREAD}
)
