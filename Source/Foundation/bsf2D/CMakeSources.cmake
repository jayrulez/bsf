set(BS_2D_INC_2D
	"bsf2D/2D/BsImageSprite.h"
	"bsf2D/2D/BsSprite.h"
	"bsf2D/2D/BsTextSprite.h"
	"bsf2D/2D/BsSpriteMaterial.h"
	"bsf2D/2D/BsSpriteMaterials.h"
	"bsf2D/2D/BsSpriteManager.h"
	"bsf2D/2D/BsSpriteTexture.h"
)

set(BS_2D_SRC_2D
	"bsf2D/2D/BsImageSprite.cpp"
	"bsf2D/2D/BsSprite.cpp"
	"bsf2D/2D/BsTextSprite.cpp"
	"bsf2D/2D/BsSpriteMaterial.cpp"
	"bsf2D/2D/BsSpriteMaterials.cpp"
	"bsf2D/2D/BsSpriteManager.cpp"
	"bsf2D/2D/BsSpriteTexture.cpp"
)

set(BS_2D_INC_RTTI
	"bsf2D/Private/RTTI/BsSpriteTextureRTTI.h"
)

set(BS_2D_INC_NOFILTER
	"bsf2D/Bs2DPrerequisites.h"
)

source_group("2D" FILES ${BS_2D_INC_2D} ${BS_2D_SRC_2D})
source_group("RTTI" FILES ${BS_2D_INC_RTTI})
source_group("" FILES ${BS_2D_INC_NOFILTER})

set(BS_2D_SRC
	${BS_2D_INC_2D}
	${BS_2D_SRC_2D}
	${BS_2D_INC_RTTI}
	${BS_2D_INC_NOFILTER}
)
