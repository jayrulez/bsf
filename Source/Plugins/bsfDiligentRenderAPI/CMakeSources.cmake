set(BS_DILIGENTRENDERAPI_INC_NOFILTER
	"BsDiligentTexture.h"
	"BsDiligentRenderTargets.h"
	"BsDiligentRenderStates.h"
	"BsDiligentPrerequisites.h"
	"BsDiligentBuffers.h"
	"BsDiligentRenderAPI.h"
	"BsDiligentRenderAPIFactory.h"
	"BsDiligentCommandBuffer.h"
	"BsDiligentQueries.h"
)

set(BS_DILIGENTRENDERAPI_SRC_NOFILTER
	"BsDiligentTexture.cpp"
	"BsDiligentRenderTargets.cpp"
	"BsDiligentRenderStates.cpp"
	"BsDiligentPlugin.cpp"
	"BsDiligentBuffers.cpp"
	"BsDiligentRenderAPI.cpp"
	"BsDiligentRenderAPIFactory.cpp"
	"BsDiligentCommandBuffer.cpp"
	"BsDiligentQueries.cpp"
	"BsDiligentWin32Resource.rc"
)

source_group("" FILES ${BS_DILIGENTRENDERAPI_SRC_NOFILTER} ${BS_DILIGENTRENDERAPI_INC_NOFILTER})

set(BS_DILIGENTRENDERAPI_SRC
	${BS_DILIGENTRENDERAPI_INC_NOFILTER}
	${BS_DILIGENTRENDERAPI_SRC_NOFILTER}
)