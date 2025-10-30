set(BS_ANIMATION_INC_ANIMATION
	"bsfAnimation/Animation/BsAnimation.h"
	"bsfAnimation/Animation/BsAnimationClip.h"
	"bsfAnimation/Animation/BsAnimationCurve.h"
	"bsfAnimation/Animation/BsAnimationManager.h"
	"bsfAnimation/Animation/BsAnimationUtility.h"
	"bsfAnimation/Animation/BsMorphShapes.h"
)

set(BS_ANIMATION_SRC_ANIMATION
	"bsfAnimation/Animation/BsAnimation.cpp"
	"bsfAnimation/Animation/BsAnimationClip.cpp"
	"bsfAnimation/Animation/BsAnimationCurve.cpp"
	"bsfAnimation/Animation/BsAnimationManager.cpp"
	"bsfAnimation/Animation/BsAnimationUtility.cpp"
	"bsfAnimation/Animation/BsMorphShapes.cpp"
)

set(BS_ANIMATION_INC_COMPONENTS
	"bsfAnimation/Components/BsCAnimation.h"
	"bsfAnimation/Components/BsCBone.h"
)

set(BS_ANIMATION_SRC_COMPONENTS
	"bsfAnimation/Components/BsCAnimation.cpp"
	"bsfAnimation/Components/BsCBone.cpp"
)

set(BS_ANIMATION_INC_RTTI
	"bsfAnimation/Private/RTTI/BsAnimationClipRTTI.h"
	"bsfAnimation/Private/RTTI/BsAnimationCurveRTTI.h"
	"bsfAnimation/Private/RTTI/BsCAnimationRTTI.h"
	"bsfAnimation/Private/RTTI/BsCBoneRTTI.h"
	"bsfAnimation/Private/RTTI/BsMorphShapesRTTI.h"
)

set(BS_ANIMATION_INC_NOFILTER
	"bsfAnimation/BsAnimationPrerequisites.h"
)

source_group("Animation" FILES ${BS_ANIMATION_INC_ANIMATION} ${BS_ANIMATION_SRC_ANIMATION})
source_group("Components" FILES ${BS_ANIMATION_INC_COMPONENTS} ${BS_ANIMATION_SRC_COMPONENTS})
source_group("RTTI" FILES ${BS_ANIMATION_INC_RTTI})
source_group("" FILES ${BS_ANIMATION_INC_NOFILTER})

set(BS_ANIMATION_SRC
	${BS_ANIMATION_INC_ANIMATION}
	${BS_ANIMATION_SRC_ANIMATION}
	${BS_ANIMATION_INC_COMPONENTS}
	${BS_ANIMATION_SRC_COMPONENTS}
	${BS_ANIMATION_INC_RTTI}
	${BS_ANIMATION_INC_NOFILTER}
)
