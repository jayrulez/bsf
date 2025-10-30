set(BS_TEXT_INC_TEXT
	"bsfText/Text/BsTextData.h"
	"bsfText/Text/BsFontImportOptions.h"
	"bsfText/Text/BsFontDesc.h"
	"bsfText/Text/BsFont.h"
	"bsfText/Text/BsTextBuiltinResources.h"
)

set(BS_TEXT_SRC_TEXT
	"bsfText/Text/BsFont.cpp"
	"bsfText/Text/BsFontImportOptions.cpp"
	"bsfText/Text/BsTextData.cpp"
	"bsfText/Text/BsTextBuiltinResources.cpp"
)

set(BS_TEXT_INC_RTTI
	"bsfText/Private/RTTI/BsCharDescRTTI.h"
	"bsfText/Private/RTTI/BsFontRTTI.h"
	"bsfText/Private/RTTI/BsFontImportOptionsRTTI.h"
)

set(BS_TEXT_INC_NOFILTER
	"bsfText/BsTextPrerequisites.h"
)

source_group("Text" FILES ${BS_TEXT_INC_TEXT} ${BS_TEXT_SRC_TEXT})
source_group("RTTI" FILES ${BS_TEXT_INC_RTTI})
source_group("" FILES ${BS_TEXT_INC_NOFILTER})

set(BS_TEXT_SRC
	${BS_TEXT_INC_TEXT}
	${BS_TEXT_SRC_TEXT}
	${BS_TEXT_INC_RTTI}
	${BS_TEXT_INC_NOFILTER}
)
