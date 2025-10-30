set(BS_MODEL_INC_MODEL
	"bsfModel/Model/BsSkeleton.h"
	"bsfModel/Model/BsSkeletonMask.h"
)

set(BS_MODEL_SRC_MODEL
	"bsfModel/Model/BsSkeleton.cpp"
	"bsfModel/Model/BsSkeletonMask.cpp"
)

set(BS_MODEL_INC_RTTI
	"bsfModel/Private/RTTI/BsSkeletonRTTI.h"
)

set(BS_MODEL_INC_NOFILTER
	"bsfModel/BsModelPrerequisites.h"
)

source_group("Model" FILES ${BS_MODEL_INC_MODEL} ${BS_MODEL_SRC_MODEL})
source_group("RTTI" FILES ${BS_MODEL_INC_RTTI})
source_group("" FILES ${BS_MODEL_INC_NOFILTER})

set(BS_MODEL_SRC
	${BS_MODEL_INC_MODEL}
	${BS_MODEL_SRC_MODEL}
	${BS_MODEL_INC_RTTI}
	${BS_MODEL_INC_NOFILTER}
)
