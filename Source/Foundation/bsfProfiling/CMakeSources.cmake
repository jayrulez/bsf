set(BS_PROFILING_INC_PROFILING
	"bsfProfiling/Profiling/BsCpuid.h"
	"bsfProfiling/Profiling/BsProfilerCPU.h"
	"bsfProfiling/Profiling/BsProfilerGPU.h"
	"bsfProfiling/Profiling/BsProfilingManager.h"
	"bsfProfiling/Profiling/BsRenderStats.h"
)

set(BS_PROFILING_SRC_PROFILING
	"bsfProfiling/Profiling/BsProfilerCPU.cpp"
	"bsfProfiling/Profiling/BsProfilerGPU.cpp"
	"bsfProfiling/Profiling/BsProfilingManager.cpp"
)

set(BS_PROFILING_INC_NOFILTER
	"bsfProfiling/BsProfilingPrerequisites.h"
)

source_group("Profiling" FILES ${BS_PROFILING_INC_PROFILING} ${BS_PROFILING_SRC_PROFILING})
source_group("" FILES ${BS_PROFILING_INC_NOFILTER})

set(BS_PROFILING_SRC
	${BS_PROFILING_INC_PROFILING}
	${BS_PROFILING_SRC_PROFILING}
	${BS_PROFILING_INC_NOFILTER}
)
