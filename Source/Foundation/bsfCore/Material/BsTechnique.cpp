//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Material/BsTechnique.h"
#include "Error/BsException.h"
#include "RenderAPI/BsRenderAPI.h"
#include "Renderer/BsRendererManager.h"
#include "Material/BsPass.h"
#include "Renderer/BsRenderer.h"
#include "Managers/BsGpuProgramManager.h"
#include "Private/RTTI/BsTechniqueRTTI.h"

namespace bs
{
	TechniqueBase::TechniqueBase(const String& language, const Vector<StringID>& tags, const ShaderVariation& variation)
		:mLanguage(language), mTags(tags), mVariation(variation)
	{

	}

	bool TechniqueBase::isSupported() const
	{
		if (ct::GpuProgramManager::instance().isLanguageSupported(mLanguage) || mLanguage == "Any")
			return true;

		return false;
	}

	bool TechniqueBase::hasTag(const StringID& tag)
	{
		for(auto& entry : mTags)
		{
			if (entry == tag)
				return true;
		}

		return false;
	}

	Technique::Technique(const String& language, const Vector<StringID>& tags,
		const ShaderVariation& variation, const Vector<SPtr<Pass>>& passes)
		: TechniqueBase(language, tags, variation), mPasses(passes)
	{ }

	Technique::Technique()
		: TechniqueBase("", {}, ShaderVariation())
	{ }

	SPtr<Pass> Technique::getPass(UINT32 idx) const
	{
		if (idx < 0 || idx >= (UINT32)mPasses.size())
			BS_EXCEPT(InvalidParametersException, "Index out of range: " + toString(idx));

		return mPasses[idx];
	}

	void Technique::compile()
	{
		for(auto& pass : mPasses)
			pass->compile();
	}

	void Technique::getCoreDependencies(Vector<CoreObject*>& dependencies)
	{
		for (auto& pass : mPasses)
			dependencies.push_back(pass.get());
	}

	SPtr<Technique> Technique::create(const String& language, const Vector<SPtr<Pass>>& passes)
	{
		Technique* technique = new (bs_alloc<Technique>()) Technique(language, {}, ShaderVariation(), passes);
		SPtr<Technique> techniquePtr = bs_core_ptr<Technique>(technique);
		techniquePtr->_setThisPtr(techniquePtr);
		techniquePtr->initialize();

		return techniquePtr;
	}

	SPtr<Technique> Technique::create(const String& language, const Vector<StringID>& tags, const ShaderVariation& variation, const Vector<SPtr<Pass>>& passes)
	{
		Technique* technique = new (bs_alloc<Technique>()) Technique(language, tags, variation, passes);
		SPtr<Technique> techniquePtr = bs_core_ptr<Technique>(technique);
		techniquePtr->_setThisPtr(techniquePtr);
		techniquePtr->initialize();

		return techniquePtr;
	}

	SPtr<Technique> Technique::createEmpty()
	{
		Technique* technique = new (bs_alloc<Technique>()) Technique();
		SPtr<Technique> techniquePtr = bs_core_ptr<Technique>(technique);
		techniquePtr->_setThisPtr(techniquePtr);

		return techniquePtr;
	}

	RTTITypeBase* Technique::getRTTIStatic()
	{
		return TechniqueRTTI::instance();
	}

	RTTITypeBase* Technique::getRTTI() const
	{
		return Technique::getRTTIStatic();
	}
}
