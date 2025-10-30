set(BS_IMAGE_INC_IMAGE
	"bsfImage/Image/BsPixelData.h"
	"bsfImage/Image/BsPixelUtil.h"
	"bsfImage/Image/BsPixelVolume.h"
)

set(BS_IMAGE_SRC_IMAGE
	"bsfImage/Image/BsPixelData.cpp"
	"bsfImage/Image/BsPixelUtil.cpp"
)

set(BS_IMAGE_INC_RTTI
	"bsfImage/Private/RTTI/BsPixelDataRTTI.h"
)

set(BS_IMAGE_INC_NOFILTER
	"bsfImage/BsImagePrerequisites.h"
)

source_group("Image" FILES ${BS_IMAGE_INC_IMAGE} ${BS_IMAGE_SRC_IMAGE})
source_group("RTTI" FILES ${BS_IMAGE_INC_RTTI})
source_group("" FILES ${BS_IMAGE_INC_NOFILTER})

set(BS_IMAGE_SRC
	${BS_IMAGE_INC_IMAGE}
	${BS_IMAGE_SRC_IMAGE}
	${BS_IMAGE_INC_RTTI}
	${BS_IMAGE_INC_NOFILTER}
)
