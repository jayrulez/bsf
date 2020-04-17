//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Material/BsShader.h"
#include "Material/BsTechnique.h"
#include "Error/BsException.h"
#include "Debug/BsDebug.h"
#include "Private/RTTI/BsShaderRTTI.h"
#include "Resources/BsResources.h"
#include "RenderAPI/BsGpuParams.h"
#include "Material/BsPass.h"
#include "RenderAPI/BsSamplerState.h"
#include "Image/BsTexture.h"

namespace bs
{
	RTTITypeBase* SubShader::getRTTIStatic()
	{
		return SubShaderRTTI::instance();
	}

	RTTITypeBase* SubShader::getRTTI() const
	{
		return SubShader::getRTTIStatic();
	}

	template<bool Core>
	TSHADER_DESC<Core>::TSHADER_DESC()
		:queueSortType(QueueSortType::None), queuePriority(0), separablePasses(false), flags(0)
	{

	}

	template<bool Core>
	void TSHADER_DESC<Core>::addParameter(SHADER_DATA_PARAM_DESC paramDesc, UINT8* defaultValue)
	{
		if(paramDesc.type == GPDT_STRUCT && paramDesc.elementSize <= 0)
		{
			BS_LOG(Error, Material, "You need to provide a non-zero element size for a struct parameter.");
			return;
		}

		const auto iterFind = dataParams.find(paramDesc.name);
		if(iterFind != dataParams.end())
			return;

		if (defaultValue != nullptr)
		{
			paramDesc.defaultValueIdx = (UINT32)dataDefaultValues.size();
			UINT32 defaultValueSize = Shader::getDataParamSize(paramDesc.type);

			dataDefaultValues.resize(paramDesc.defaultValueIdx + defaultValueSize);
			memcpy(&dataDefaultValues[paramDesc.defaultValueIdx], defaultValue, defaultValueSize);
		}
		else
			paramDesc.defaultValueIdx = (UINT32)-1;

		dataParams[paramDesc.name] = paramDesc;
	}

	template<bool Core>
	void TSHADER_DESC<Core>::addParameter(SHADER_OBJECT_PARAM_DESC paramDesc)
	{
		UINT32 defaultValueIdx = (UINT32)-1;

		addParameterInternal(std::move(paramDesc), defaultValueIdx);
	}

	template<bool Core>
	void TSHADER_DESC<Core>::addParameter(SHADER_OBJECT_PARAM_DESC paramDesc, const SamplerStateType& defaultValue)
	{
		UINT32 defaultValueIdx = (UINT32)-1;
		if (Shader::isSampler(paramDesc.type) && defaultValue != nullptr)
		{
			defaultValueIdx = (UINT32)samplerDefaultValues.size();
			samplerDefaultValues.push_back(defaultValue);
		}

		addParameterInternal(std::move(paramDesc), defaultValueIdx);
	}

	template<bool Core>
	void TSHADER_DESC<Core>::addParameter(SHADER_OBJECT_PARAM_DESC paramDesc, const HTexture& defaultValue)
	{
		UINT32 defaultValueIdx = (UINT32)-1;
		if (Shader::isTexture(paramDesc.type) && defaultValue != nullptr)
		{
			defaultValueIdx = (UINT32)textureDefaultValues.size();
			textureDefaultValues.push_back(defaultValue);
		}

		addParameterInternal(std::move(paramDesc), defaultValueIdx);
	}

	template<bool Core>
	void TSHADER_DESC<Core>::addParameterInternal(SHADER_OBJECT_PARAM_DESC paramDesc, UINT32 defaultValueIdx)
	{
		Map<String, SHADER_OBJECT_PARAM_DESC>* DEST_LOOKUP[] = { &textureParams, &bufferParams, &samplerParams };
		UINT32 destIdx = 0;
		if (Shader::isBuffer(paramDesc.type))
			destIdx = 1;
		else if (Shader::isSampler(paramDesc.type))
			destIdx = 2;

		Map<String, SHADER_OBJECT_PARAM_DESC>& paramsMap = *DEST_LOOKUP[destIdx];

		auto iterFind = paramsMap.find(paramDesc.name);
		if (iterFind == paramsMap.end())
		{
			paramDesc.defaultValueIdx = defaultValueIdx;
			paramsMap[paramDesc.name] = paramDesc;
		}
		else
		{
			SHADER_OBJECT_PARAM_DESC& desc = iterFind->second;

			// If same name but different properties, we ignore this param
			if (desc.type != paramDesc.type || desc.rendererSemantic != paramDesc.rendererSemantic)
				return;

			Vector<String>& gpuVariableNames = desc.gpuVariableNames;
			bool found = false;
			for (UINT32 i = 0; i < (UINT32)gpuVariableNames.size(); i++)
			{
				if (gpuVariableNames[i] == paramDesc.gpuVariableName)
				{
					found = true;
					break;
				}
			}

			if (!found)
				gpuVariableNames.push_back(paramDesc.gpuVariableName);
		}
	}

	template<bool Core>
	void TSHADER_DESC<Core>::setParameterAttribute(const String& name, const SHADER_PARAM_ATTRIBUTE& attrib)
	{
		SHADER_DATA_PARAM_DESC* paramDescData = nullptr;

		const auto findIterData = dataParams.find(name);
		if (findIterData != dataParams.end())
			paramDescData = &findIterData->second;

		SHADER_OBJECT_PARAM_DESC* paramDescObj = nullptr;
		if(!paramDescData)
		{
			const auto findIterTexture = textureParams.find(name);
			if (findIterTexture != textureParams.end())
				paramDescObj = &findIterTexture->second;

			if (!paramDescObj)
			{
				const auto findIterSampler = samplerParams.find(name);
				if (findIterSampler != samplerParams.end())
					paramDescObj = &findIterSampler->second;
			}

			if (!paramDescObj)
			{
				const auto findIterBuffer = bufferParams.find(name);
				if (findIterBuffer != bufferParams.end())
					paramDescObj = &findIterBuffer->second;
			}
		}

		SHADER_PARAM_COMMON* paramDesc = paramDescData;
		if(!paramDesc)
			paramDesc = paramDescObj;

		if(!paramDesc)
		{
			BS_LOG(Warning, Material, "Attempting to apply a shader parameter attribute to a non-existing parameter.");
			return;
		}

		if(attrib.type == ShaderParamAttributeType::SpriteUV)
		{
			if(paramDescObj)
			{
				BS_LOG(Warning, Material, "Attempting to apply SpriteUV attribute to an object parameter is not supported.");
				return;
			}

			if(paramDescData->type != GPDT_FLOAT4)
			{
				BS_LOG(Warning, Material, "SpriteUV attribute can only be applied to 4D vectors.");
				return;
			}
		}

		// Look for duplicate attributes
		UINT32 curAttribIdx = paramDesc->attribIdx;
		bool found = false;
		while(curAttribIdx != (UINT32)-1)
		{
			SHADER_PARAM_ATTRIBUTE& curAttrib = paramAttributes[curAttribIdx];
			if(curAttrib.type == attrib.type)
			{
				curAttrib = attrib;

				found = true;
				break;
			}

			curAttribIdx = curAttrib.nextParamIdx;
		}

		if(!found)
		{
			const auto attribIdx = (UINT32)paramAttributes.size();
			paramAttributes.emplace_back(attrib);

			if (paramDesc->attribIdx != (UINT32)-1)
				paramAttributes.back().nextParamIdx = paramDesc->attribIdx;

			paramDesc->attribIdx = attribIdx;
		}
	}

	template<bool Core>
	void TSHADER_DESC<Core>::setParamBlockAttribs(const String& name, bool shared, GpuBufferUsage usage,
		StringID rendererSemantic)
	{
		SHADER_PARAM_BLOCK_DESC desc;
		desc.name = name;
		desc.shared = shared;
		desc.usage = usage;
		desc.rendererSemantic = rendererSemantic;

		paramBlocks[name] = desc;
	}

	template struct TSHADER_DESC<false>;

	template<bool Core>
	TShader<Core>::TShader(UINT32 id)
		:mId(id)
	{ }

	template<bool Core>
	TShader<Core>::TShader(const String& name, const TSHADER_DESC<Core>& desc, UINT32 id)
		:mName(name), mDesc(desc), mId(id)
	{ }

	template<bool Core>
	TShader<Core>::~TShader()
	{ }

	template<bool Core>
	GpuParamType TShader<Core>::getParamType(const String& name) const
	{
		auto findIterData = mDesc.dataParams.find(name);
		if (findIterData != mDesc.dataParams.end())
			return GPT_DATA;

		auto findIterTexture = mDesc.textureParams.find(name);
		if (findIterTexture != mDesc.textureParams.end())
			return GPT_TEXTURE;

		auto findIterBuffer = mDesc.bufferParams.find(name);
		if (findIterBuffer != mDesc.bufferParams.end())
			return GPT_BUFFER;

		auto findIterSampler = mDesc.samplerParams.find(name);
		if (findIterSampler != mDesc.samplerParams.end())
			return GPT_SAMPLER;

		BS_EXCEPT(InternalErrorException, "Cannot find the parameter with the name: " + name);
		return GPT_DATA;
	}

	template<bool Core>
	const SHADER_DATA_PARAM_DESC& TShader<Core>::getDataParamDesc(const String& name) const
	{
		auto findIterData = mDesc.dataParams.find(name);
		if (findIterData != mDesc.dataParams.end())
			return findIterData->second;

		BS_EXCEPT(InternalErrorException, "Cannot find the parameter with the name: " + name);
		static SHADER_DATA_PARAM_DESC dummy;
		return dummy;
	}

	template<bool Core>
	const SHADER_OBJECT_PARAM_DESC& TShader<Core>::getTextureParamDesc(const String& name) const
	{
		auto findIterObject = mDesc.textureParams.find(name);
		if (findIterObject != mDesc.textureParams.end())
			return findIterObject->second;

		BS_EXCEPT(InternalErrorException, "Cannot find the parameter with the name: " + name);
		static SHADER_OBJECT_PARAM_DESC dummy;
		return dummy;
	}

	template<bool Core>
	const SHADER_OBJECT_PARAM_DESC& TShader<Core>::getSamplerParamDesc(const String& name) const
	{
		auto findIterObject = mDesc.samplerParams.find(name);
		if (findIterObject != mDesc.samplerParams.end())
			return findIterObject->second;

		BS_EXCEPT(InternalErrorException, "Cannot find the parameter with the name: " + name);
		static SHADER_OBJECT_PARAM_DESC dummy;
		return dummy;
	}

	template<bool Core>
	const SHADER_OBJECT_PARAM_DESC& TShader<Core>::getBufferParamDesc(const String& name) const
	{
		auto findIterObject = mDesc.bufferParams.find(name);
		if (findIterObject != mDesc.bufferParams.end())
			return findIterObject->second;

		BS_EXCEPT(InternalErrorException, "Cannot find the parameter with the name: " + name);
		static SHADER_OBJECT_PARAM_DESC dummy;
		return dummy;
	}

	template<bool Core>
	bool TShader<Core>::hasDataParam(const String& name) const
	{
		auto findIterData = mDesc.dataParams.find(name);
		if (findIterData != mDesc.dataParams.end())
			return true;

		return false;
	}

	template<bool Core>
	bool TShader<Core>::hasTextureParam(const String& name) const
	{
		auto findIterObject = mDesc.textureParams.find(name);
		if (findIterObject != mDesc.textureParams.end())
			return true;

		return false;
	}

	template<bool Core>
	bool TShader<Core>::hasSamplerParam(const String& name) const
	{
		auto findIterObject = mDesc.samplerParams.find(name);
		if (findIterObject != mDesc.samplerParams.end())
			return true;

		return false;
	}

	template<bool Core>
	bool TShader<Core>::hasBufferParam(const String& name) const
	{
		auto findIterObject = mDesc.bufferParams.find(name);
		if (findIterObject != mDesc.bufferParams.end())
			return true;

		return false;
	}

	template<bool Core>
	bool TShader<Core>::hasParamBlock(const String& name) const
	{
		auto findIterObject = mDesc.paramBlocks.find(name);
		if (findIterObject != mDesc.paramBlocks.end())
			return true;

		return false;
	}

	template<bool Core>
	HTexture TShader<Core>::getDefaultTexture(UINT32 index) const
	{
		if (index < (UINT32)mDesc.textureDefaultValues.size())
			return mDesc.textureDefaultValues[index];

		return HTexture();
	}

	template<bool Core>
	typename TShader<Core>::SamplerStateType TShader<Core>::getDefaultSampler(UINT32 index) const
	{
		if (index < (UINT32)mDesc.samplerDefaultValues.size())
			return mDesc.samplerDefaultValues[index];

		return SamplerStateType();
	}

	template<bool Core>
	UINT8* TShader<Core>::getDefaultValue(UINT32 index) const
	{
		if (index < (UINT32)mDesc.dataDefaultValues.size())
			return (UINT8*)&mDesc.dataDefaultValues[index];

		return nullptr;
	}

	template<bool Core>
	Vector<SPtr<Technique>> TShader<Core>::getCompatibleTechniques() const
	{
		Vector<SPtr<Technique>> output;
		for (auto& technique : mDesc.techniques)
		{
			if (technique->isSupported())
				output.push_back(technique);
		}

		return output;
	}

	template<bool Core>
	Vector<SPtr<Technique>> TShader<Core>::getCompatibleTechniques(
		const ShaderVariation& variation, bool exact) const
	{
		Vector<SPtr<Technique>> output;
		for (auto& technique : mDesc.techniques)
		{
			if (technique->isSupported() && technique->getVariation().matches(variation, exact))
				output.push_back(technique);
		}

		return output;
	}

	template class TShader < false > ;


	std::atomic<UINT32> Shader::mNextShaderId;

	Shader::Shader(const String& name, const SHADER_DESC& desc, UINT32 id)
		:TShader(name, desc, id)
	{
		mMetaData = bs_shared_ptr_new<ShaderMetaData>();
	}

	Shader::Shader(UINT32 id)
		:TShader(id)
	{ }

	void Shader::setIncludeFiles(const Vector<String>& includes)
	{
		SPtr<ShaderMetaData> meta = std::static_pointer_cast<ShaderMetaData>(getMetaData());
		meta->includes = includes;
	}

	void Shader::getCoreDependencies(Vector<CoreObject*>& dependencies)
	{
		for (auto& technique : mDesc.techniques)
			dependencies.push_back(technique.get());
	}

	bool Shader::isSampler(GpuParamObjectType type)
	{
		switch(type)
		{
			case GPOT_SAMPLER1D:
			case GPOT_SAMPLER2D:
			case GPOT_SAMPLER3D:
			case GPOT_SAMPLERCUBE:
			case GPOT_SAMPLER2DMS:
				return true;
			default:
				return false;
		}
	}

	bool Shader::isTexture(GpuParamObjectType type)
	{
		switch(type)
		{
		case GPOT_TEXTURE1D:
		case GPOT_TEXTURE2D:
		case GPOT_TEXTURE3D:
		case GPOT_TEXTURECUBE:
		case GPOT_TEXTURE2DMS:
		case GPOT_TEXTURE1DARRAY:
		case GPOT_TEXTURE2DARRAY:
		case GPOT_TEXTURE2DMSARRAY:
		case GPOT_TEXTURECUBEARRAY:
			return true;
		default:
			return false;
		}
	}

	bool Shader::isLoadStoreTexture(GpuParamObjectType type)
	{
		switch (type)
		{
		case GPOT_RWTEXTURE1D:
		case GPOT_RWTEXTURE2D:
		case GPOT_RWTEXTURE3D:
		case GPOT_RWTEXTURE2DMS:
		case GPOT_RWTEXTURE1DARRAY:
		case GPOT_RWTEXTURE2DARRAY:
		case GPOT_RWTEXTURE2DMSARRAY:
			return true;
		default:
			return false;
		}
	}

	bool Shader::isBuffer(GpuParamObjectType type)
	{
		switch(type)
		{
		case GPOT_BYTE_BUFFER:
		case GPOT_STRUCTURED_BUFFER:
		case GPOT_RWBYTE_BUFFER:
		case GPOT_RWAPPEND_BUFFER:
		case GPOT_RWCONSUME_BUFFER:
		case GPOT_RWSTRUCTURED_BUFFER:
		case GPOT_RWSTRUCTURED_BUFFER_WITH_COUNTER:
		case GPOT_RWTYPED_BUFFER:
			return true;
		default:
			return false;
		}
	}

	UINT32 Shader::getDataParamSize(GpuParamDataType type)
	{
		static const GpuDataParamInfos PARAM_SIZES;

		UINT32 idx = (UINT32)type;
		if (idx < sizeof(GpuParams::PARAM_SIZES.lookup))
			return GpuParams::PARAM_SIZES.lookup[idx].size;

		return 0;
	}

	HShader Shader::createHandle(const String& name, const SHADER_DESC& desc)
	{
		SPtr<Shader> newShader = create(name, desc);

		return static_resource_cast<Shader>(gResources()._createResourceHandle(newShader));
	}

	SPtr<Shader> Shader::create(const String& name, const SHADER_DESC& desc)
	{
		UINT32 id = Shader::mNextShaderId.fetch_add(1, std::memory_order_relaxed);
		assert(id < std::numeric_limits<UINT32>::max() && "Created too many shaders, reached maximum id.");

		SPtr<Shader> newShader = bs_core_ptr<Shader>(new (bs_alloc<Shader>()) Shader(name, desc, id));
		newShader->_setThisPtr(newShader);
		newShader->initialize();

		return newShader;
	}

	SPtr<Shader> Shader::createEmpty()
	{
		UINT32 id = Shader::mNextShaderId.fetch_add(1, std::memory_order_relaxed);
		assert(id < std::numeric_limits<UINT32>::max() && "Created too many shaders, reached maximum id.");

		SPtr<Shader> newShader = bs_core_ptr<Shader>(new (bs_alloc<Shader>()) Shader(id));
		newShader->_setThisPtr(newShader);

		return newShader;
	}

	RTTITypeBase* Shader::getRTTIStatic()
	{
		return ShaderRTTI::instance();
	}

	RTTITypeBase* Shader::getRTTI() const
	{
		return Shader::getRTTIStatic();
	}

	RTTITypeBase* ShaderMetaData::getRTTIStatic()
	{
		return ShaderMetaDataRTTI::instance();
	}

	RTTITypeBase* ShaderMetaData::getRTTI() const
	{
		return ShaderMetaData::getRTTIStatic();
	}
}
