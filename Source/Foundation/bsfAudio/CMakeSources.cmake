set(BS_AUDIO_INC_AUDIO
	"bsfAudio/Audio/BsAudio.h"
	"bsfAudio/Audio/BsAudioClip.h"
	"bsfAudio/Audio/BsAudioListener.h"
	"bsfAudio/Audio/BsAudioSource.h"
	"bsfAudio/Audio/BsAudioClipImportOptions.h"
	"bsfAudio/Audio/BsAudioUtility.h"
	"bsfAudio/Audio/BsAudioManager.h"
)

set(BS_AUDIO_SRC_AUDIO
	"bsfAudio/Audio/BsAudio.cpp"
	"bsfAudio/Audio/BsAudioClip.cpp"
	"bsfAudio/Audio/BsAudioListener.cpp"
	"bsfAudio/Audio/BsAudioSource.cpp"
	"bsfAudio/Audio/BsAudioClipImportOptions.cpp"
	"bsfAudio/Audio/BsAudioUtility.cpp"
	"bsfAudio/Audio/BsAudioManager.cpp"
)

set(BS_AUDIO_INC_COMPONENTS
	"bsfAudio/Components/BsCAudioListener.h"
	"bsfAudio/Components/BsCAudioSource.h"
)

set(BS_AUDIO_SRC_COMPONENTS
	"bsfAudio/Components/BsCAudioListener.cpp"
	"bsfAudio/Components/BsCAudioSource.cpp"
)

set(BS_AUDIO_INC_RTTI
	"bsfAudio/Private/RTTI/BsAudioClipRTTI.h"
	"bsfAudio/Private/RTTI/BsAudioSourceRTTI.h"
	"bsfAudio/Private/RTTI/BsAudioListenerRTTI.h"
	"bsfAudio/Private/RTTI/BsCAudioSourceRTTI.h"
	"bsfAudio/Private/RTTI/BsCAudioListenerRTTI.h"
	"bsfAudio/Private/RTTI/BsAudioClipImportOptionsRTTI.h"
)

set(BS_AUDIO_INC_NOFILTER
	"bsfAudio/BsAudioPrerequisites.h"
)

source_group("Audio" FILES ${BS_AUDIO_INC_AUDIO} ${BS_AUDIO_SRC_AUDIO})
source_group("Components" FILES ${BS_AUDIO_INC_COMPONENTS} ${BS_AUDIO_SRC_COMPONENTS})
source_group("RTTI" FILES ${BS_AUDIO_INC_RTTI})
source_group("" FILES ${BS_AUDIO_INC_NOFILTER})

set(BS_AUDIO_SRC
	${BS_AUDIO_INC_AUDIO}
	${BS_AUDIO_SRC_AUDIO}
	${BS_AUDIO_INC_COMPONENTS}
	${BS_AUDIO_SRC_COMPONENTS}
	${BS_AUDIO_INC_RTTI}
	${BS_AUDIO_INC_NOFILTER}
)
