//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Audio/BsAudioClip.h"
#include "Resources/BsResources.h"
#include "Audio/BsAudio.h"
#include "Private/RTTI/BsAudioClipRTTI.h"

namespace bs
{
	AudioClipResource::AudioClipResource(const SPtr<DataStream>& samples, UINT32 streamSize, UINT32 numSamples, const AUDIO_CLIP_DESC& desc)
		: Resource(false), mDesc(desc), mNumSamples(numSamples), mStreamSize(streamSize), mStreamData(samples)
	{
		if (samples != nullptr)
			mStreamOffset = (UINT32)samples->tell();

		mKeepSourceData = desc.keepSourceData;
	}

	void AudioClipResource::initialize()
	{
		mLength = mNumSamples / mDesc.numChannels / (float)mDesc.frequency;

		Resource::initialize();
	}

	AudioClipResourceHandle AudioClipResource::create(const SPtr<DataStream>& samples, UINT32 streamSize, UINT32 numSamples, const AUDIO_CLIP_DESC& desc)
	{
		return static_resource_cast<AudioClipResource>(gResources()._createResourceHandle(_createPtr(samples, streamSize, numSamples, desc)));
	}

	SPtr<AudioClipResource> AudioClipResource::_createPtr(const SPtr<DataStream>& samples, UINT32 streamSize, UINT32 numSamples, const AUDIO_CLIP_DESC& desc)
	{
		SPtr<AudioClipResource> newClip = gAudio().createClip(samples, streamSize, numSamples, desc);
		newClip->_setThisPtr(newClip);
		newClip->initialize();

		return newClip;
	}

	SPtr<AudioClipResource> AudioClipResource::createEmpty()
	{
		AUDIO_CLIP_DESC desc;

		SPtr<AudioClipResource> newClip = gAudio().createClip(nullptr, 0, 0, desc);
		newClip->_setThisPtr(newClip);

		return newClip;
	}

	RTTITypeBase* AudioClipResource::getRTTIStatic()
	{
		return AudioClipResourceRTTI::instance();
	}

	RTTITypeBase* AudioClipResource::getRTTI() const
	{
		return getRTTIStatic();
	}
}
