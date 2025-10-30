set(BS_LOCALIZATION_INC_LOCALIZATION
	"bsfLocalization/Localization/BsHString.h"
	"bsfLocalization/Localization/BsHEString.h"
	"bsfLocalization/Localization/BsStringTable.h"
	"bsfLocalization/Localization/BsStringTableManager.h"
)

set(BS_LOCALIZATION_SRC_LOCALIZATION
	"bsfLocalization/Localization/BsHString.cpp"
	"bsfLocalization/Localization/BsHEString.cpp"
	"bsfLocalization/Localization/BsStringTable.cpp"
	"bsfLocalization/Localization/BsStringTableManager.cpp"
)

set(BS_LOCALIZATION_INC_RTTI
	"bsfLocalization/Private/RTTI/BsStringTableRTTI.h"
)

set(BS_LOCALIZATION_INC_NOFILTER
	"bsfLocalization/BsLocalizationPrerequisites.h"
)

source_group("Localization" FILES ${BS_LOCALIZATION_INC_LOCALIZATION} ${BS_LOCALIZATION_SRC_LOCALIZATION})
source_group("RTTI" FILES ${BS_LOCALIZATION_INC_RTTI})
source_group("" FILES ${BS_LOCALIZATION_INC_NOFILTER})

set(BS_LOCALIZATION_SRC
	${BS_LOCALIZATION_INC_LOCALIZATION}
	${BS_LOCALIZATION_SRC_LOCALIZATION}
	${BS_LOCALIZATION_INC_RTTI}
	${BS_LOCALIZATION_INC_NOFILTER}
)
