//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Prerequisites/BsPrerequisitesUtil.h"

// DLL export
#if BS_PLATFORM == BS_PLATFORM_WIN32
#  if BS_COMPILER == BS_COMPILER_MSVC
#    if defined(BS_STATIC_LIB)
#      define BS_AUDIO_EXPORT
#    else
#      if defined(BS_AUDIO_EXPORTS)
#        define BS_AUDIO_EXPORT __declspec(dllexport)
#      else
#        define BS_AUDIO_EXPORT __declspec(dllimport)
#      endif
#	 endif
#  else
#    if defined(BS_STATIC_LIB)
#      define BS_AUDIO_EXPORT
#    else
#      if defined(BS_AUDIO_EXPORTS)
#        define BS_AUDIO_EXPORT __attribute__ ((dllexport))
#      else
#        define BS_AUDIO_EXPORT __attribute__ ((dllimport))
#      endif
#	 endif
#  endif
#  define BS_AUDIO_HIDDEN
#else
#  define BS_AUDIO_EXPORT __attribute__ ((visibility ("default")))
#  define BS_AUDIO_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

namespace bs
{
	class Audio;
	class AudioClip;
	class AudioListener;
	class AudioSource;
	class AudioManager;
	class AudioClipImportOptions;

	// Desc structs
	struct AUDIO_CLIP_DESC;

	/************************************************************************/
	/* 							Resource references                   		*/
	/************************************************************************/

	template<class T>
	class ResourceHandle;

	typedef ResourceHandle<AudioClip> HAudioClip;

	/************************************************************************/
	/* 						Game object handle references             		*/
	/************************************************************************/

	template<class T>
	class GameObjectHandle;

	// Game object handles
	typedef GameObjectHandle<CAudioSource> HAudioSource;
	typedef GameObjectHandle<CAudioListener> HAudioListener;

	/** Meta-data describing a chunk of audio. */
	struct AudioDataInfo
	{
		UINT32 numSamples; /**< Total number of audio samples in the audio data (includes all channels). */
		UINT32 sampleRate; /**< Number of audio samples per second, per channel. */
		UINT32 numChannels; /**< Number of channels. Each channel has its own set of samples. */
		UINT32 bitDepth; /**< Number of bits per sample. */
	};

	/************************************************************************/
	/* 									RTTI                      			*/
	/************************************************************************/

	/** Type IDs for Audio module RTTI */
	enum TypeID_Audio
	{
		TID_AudioClip = 1111,
		TID_AudioClipImportOptions = 1112,
		TID_CAudioListener = 1113,
		TID_CAudioSource = 1114,
		TID_AudioListener = 1141,
		TID_AudioSource = 1142,
	};
}
