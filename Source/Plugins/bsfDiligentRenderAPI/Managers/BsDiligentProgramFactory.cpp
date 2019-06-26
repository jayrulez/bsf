#include "BsDiligentProgramFactory.h"
#include "BsDiligentGpuProgram.h"

namespace bs
{
	namespace ct
	{
		SPtr<GpuProgram> DiligentProgramFactory::create(const GPU_PROGRAM_DESC& desc, GpuDeviceFlags deviceMask)
		{
			SPtr<GpuProgram> gpuProg = bs_shared_ptr<DiligentGpuProgram>(new (bs_alloc<DiligentGpuProgram>())
				DiligentGpuProgram(desc, deviceMask));
			gpuProg->_setThisPtr(gpuProg);

			return gpuProg;
		}

		SPtr<GpuProgram> DiligentProgramFactory::create(GpuProgramType type, GpuDeviceFlags deviceMask)
		{
			GPU_PROGRAM_DESC desc;
			desc.type = type;

			SPtr<GpuProgram> gpuProg = bs_shared_ptr<DiligentGpuProgram>(new (bs_alloc<DiligentGpuProgram>())
				DiligentGpuProgram(desc, deviceMask));
			gpuProg->_setThisPtr(gpuProg);

			return gpuProg;
		}

		SPtr<GpuProgramBytecode> DiligentProgramFactory::compileBytecode(const GPU_PROGRAM_DESC& desc)
		{
			return nullptr;
		}
	}
}