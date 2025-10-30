set(BS_PARTICLES_INC_PARTICLES
	"bsfParticles/Particles/BsParticleSystem.h"
	"bsfParticles/Particles/BsParticleEmitter.h"
	"bsfParticles/Particles/BsParticleEvolver.h"
	"bsfParticles/Particles/BsParticleManager.h"
	"bsfParticles/Particles/BsParticleDistribution.h"
	"bsfParticles/Particles/BsParticleModule.h"
	"bsfParticles/Particles/BsVectorField.h"
	"bsfParticles/Private/Particles/BsParticleSet.h"
)

set(BS_PARTICLES_SRC_PARTICLES
	"bsfParticles/Particles/BsParticleSystem.cpp"
	"bsfParticles/Particles/BsParticleEmitter.cpp"
	"bsfParticles/Particles/BsParticleEvolver.cpp"
	"bsfParticles/Particles/BsParticleManager.cpp"
	"bsfParticles/Particles/BsParticleDistribution.cpp"
	"bsfParticles/Particles/BsVectorField.cpp"
)

set(BS_PARTICLES_INC_RTTI
	"bsfParticles/Private/RTTI/BsParticleSystemRTTI.h"
	"bsfParticles/Private/RTTI/BsParticleDistributionRTTI.h"
	"bsfParticles/Private/RTTI/BsVectorFieldRTTI.h"
	"bsfParticles/Private/RTTI/BsCParticleSystemRTTI.h"
)

set(BS_PARTICLES_INC_NOFILTER
	"bsfParticles/BsParticlesPrerequisites.h"
)

source_group("Particles" FILES ${BS_PARTICLES_INC_PARTICLES} ${BS_PARTICLES_SRC_PARTICLES})
source_group("RTTI" FILES ${BS_PARTICLES_INC_RTTI})
source_group("" FILES ${BS_PARTICLES_INC_NOFILTER})

set(BS_PARTICLES_SRC
	${BS_PARTICLES_INC_PARTICLES}
	${BS_PARTICLES_SRC_PARTICLES}
	${BS_PARTICLES_INC_RTTI}
	${BS_PARTICLES_INC_NOFILTER}
)
