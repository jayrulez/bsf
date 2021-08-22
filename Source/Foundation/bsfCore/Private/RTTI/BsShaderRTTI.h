//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStringIDRTTI.h"
#include "RTTI/BsStringRTTI.h"
#include "RTTI/BsStdRTTI.h"
#include "RTTI/BsSmallVectorRTTI.h"
#include "RTTI/BsFlagsRTTI.h"
#include "Material/BsShader.h"
#include "Material/BsMaterial.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	template<> struct RTTIPlainType<SHADER_DATA_PARAM_DESC>
	{
		enum { id = TID_SHADER_DATA_PARAM_DESC }; enum { hasDynamicSize = 1 };

		static BitLength toMemory(const SHADER_DATA_PARAM_DESC& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			static constexpr UINT32 VERSION = 1;

			return rtti_write_with_size_header(stream, data, compress, [&data, &stream]()
			{
				BitLength size = 0;

				size += rtti_write(data.arraySize, stream);
				size += rtti_write(data.rendererSemantic, stream);
				size += rtti_write(data.type, stream);
				size += rtti_write(data.name, stream);
				size += rtti_write(data.gpuVariableName, stream);
				size += rtti_write(data.elementSize, stream);
				size += rtti_write(data.defaultValueIdx, stream);
				size += rtti_write(VERSION, stream);
				size += rtti_write(data.attribIdx, stream);

				return size;
			});
		}

		static BitLength fromMemory(SHADER_DATA_PARAM_DESC& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			BitLength sizeRead = rtti_read_size_header(stream, compress, size);

			sizeRead += rtti_read(data.arraySize, stream);
			sizeRead += rtti_read(data.rendererSemantic, stream);
			sizeRead += rtti_read(data.type, stream);
			sizeRead += rtti_read(data.name, stream);
			sizeRead += rtti_read(data.gpuVariableName, stream);
			sizeRead += rtti_read(data.elementSize, stream);
			sizeRead += rtti_read(data.defaultValueIdx, stream);

			// There's more to read, meaning we're reading a newer version of the format
			// (In the first version, version field is missing, so we check this way).
			if(sizeRead < size)
			{
				uint32_t version = 0;
				rtti_read(version, stream);
				switch(version)
				{
				case 1:
					rtti_read(data.attribIdx, stream);
					break;
				default:
					BS_LOG(Error, RTTI, "Unknown version. Unable to deserialize.");
					break;
				}
			}

			return size;
		}

		static BitLength getSize(const SHADER_DATA_PARAM_DESC& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = rtti_size(data.arraySize) + rtti_size(data.rendererSemantic) + rtti_size(data.type) +
				rtti_size(data.name) + rtti_size(data.gpuVariableName) + rtti_size(data.elementSize) +
				rtti_size(data.defaultValueIdx) + rtti_size(data.attribIdx) + sizeof(uint32_t);

			rtti_add_header_size(dataSize, compress);
			return dataSize;
		}
	};

	template<> struct RTTIPlainType<SHADER_OBJECT_PARAM_DESC>
	{
		enum { id = TID_SHADER_OBJECT_PARAM_DESC }; enum { hasDynamicSize = 1 };

		static BitLength toMemory(const SHADER_OBJECT_PARAM_DESC& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			static constexpr uint32_t VERSION = 1;

			return rtti_write_with_size_header(stream, data, compress, [&data, &stream]()
			{
				BitLength size = 0;
				size += rtti_write(data.rendererSemantic, stream);
				size += rtti_write(data.type, stream);
				size += rtti_write(data.name, stream);
				size += rtti_write(data.gpuVariableNames, stream);
				size += rtti_write(data.defaultValueIdx, stream);
				size += rtti_write(VERSION, stream);
				size += rtti_write(data.attribIdx, stream);

				return size;
			});
		}

		static BitLength fromMemory(SHADER_OBJECT_PARAM_DESC& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			BitLength sizeRead = rtti_read_size_header(stream, compress, size);

			sizeRead += rtti_read(data.rendererSemantic, stream);
			sizeRead += rtti_read(data.type, stream);
			sizeRead += rtti_read(data.name, stream);
			sizeRead += rtti_read(data.gpuVariableNames, stream);
			sizeRead += rtti_read(data.defaultValueIdx, stream);

			// There's more to read, meaning we're reading a newer version of the format
			// (In the first version, version field is missing, so we check this way).
			if(sizeRead < size)
			{
				uint32_t version = 0;
				rtti_read(version, stream);
				switch(version)
				{
				case 1:
					rtti_read(data.attribIdx, stream);
					break;
				default:
					BS_LOG(Error, RTTI, "Unknown version. Unable to deserialize.");
					break;
				}
			}

			return size;
		}

		static BitLength getSize(const SHADER_OBJECT_PARAM_DESC& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = rtti_size(data.rendererSemantic) + rtti_size(data.type) +
				rtti_size(data.name) + rtti_size(data.gpuVariableNames) +
				rtti_size(data.defaultValueIdx) + rtti_size(data.attribIdx) + sizeof(uint32_t);

			rtti_add_header_size(dataSize, compress);
			return dataSize;
		}
	};

	template<> struct RTTIPlainType<SHADER_PARAM_BLOCK_DESC>
	{
		enum { id = TID_SHADER_PARAM_BLOCK_DESC }; enum { hasDynamicSize = 1 };

		static BitLength toMemory(const SHADER_PARAM_BLOCK_DESC& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return rtti_write_with_size_header(stream, data, compress, [&data, &stream]()
			{
				BitLength size = 0;
				size += rtti_write(data.shared, stream);
				size += rtti_write(data.usage, stream);
				size += rtti_write(data.name, stream);
				size += rtti_write(data.rendererSemantic, stream);

				return size;
			});
		}

		static BitLength fromMemory(SHADER_PARAM_BLOCK_DESC& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			rtti_read_size_header(stream, compress, size);

			rtti_read(data.shared, stream);
			rtti_read(data.usage, stream);
			rtti_read(data.name, stream);
			rtti_read(data.rendererSemantic, stream);

			return size;
		}

		static BitLength getSize(const SHADER_PARAM_BLOCK_DESC& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = rtti_size(data.shared) + rtti_size(data.usage) +
				rtti_size(data.name) + rtti_size(data.rendererSemantic);

			rtti_add_header_size(dataSize, compress);
			return dataSize;
		}
	};

	template<> struct RTTIPlainType<SHADER_PARAM_ATTRIBUTE>
	{
		enum { id = TID_SHADER_PARAM_ATTRIBUTE }; enum { hasDynamicSize = 1 };

		static BitLength toMemory(const SHADER_PARAM_ATTRIBUTE& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			static constexpr UINT32 VERSION = 0;

			return rtti_write_with_size_header(stream, data, compress, [&data, &stream]()
			{
				BitLength size = 0;
				size += rtti_write(VERSION, stream);
				size += rtti_write(data.type, stream);
				size += rtti_write(data.value, stream);
				size += rtti_write(data.nextParamIdx, stream);

				return size;
			});
		}

		static BitLength fromMemory(SHADER_PARAM_ATTRIBUTE& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			rtti_read_size_header(stream, compress, size);

			uint32_t version = 0;
			rtti_read(version, stream);

			switch(version)
			{
			case 0:
				rtti_read(data.type, stream);
				rtti_read(data.value, stream);
				rtti_read(data.nextParamIdx, stream);
				break;
			default:
				BS_LOG(Error, RTTI, "Unknown version. Unable to deserialize.");
				break;
			}

			return size;
		}

		static BitLength getSize(const SHADER_PARAM_ATTRIBUTE& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = rtti_size(data.type) + rtti_size(data.value) +
				rtti_size(data.nextParamIdx) + sizeof(uint32_t);

			rtti_add_header_size(dataSize, compress);
			return dataSize;
		}
	};

	template<>	struct RTTIPlainType<ShaderVariationParamValue>
	{
		enum { id = TID_ShaderVariationParamValue }; enum { hasDynamicSize = 1 };

		/** @copydoc RTTIPlainType::toMemory */
		static BitLength toMemory(const ShaderVariationParamValue& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			static constexpr uint8_t VERSION = 0;

			return rtti_write_with_size_header(stream, data, compress, [&data, &stream]()
			{
				BitLength size = 0;
				size += rtti_write(VERSION, stream);
				size += rtti_write(data.name, stream);
				size += rtti_write(data.value, stream);

				return size;
			});
		}

		/** @copydoc RTTIPlainType::fromMemory */
		static BitLength fromMemory(ShaderVariationParamValue& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			rtti_read_size_header(stream, compress, size);

			uint8_t version;
			rtti_read(version, stream);
			assert(version == 0);

			rtti_read(data.name, stream);
			rtti_read(data.value, stream);

			return size;
		}

		/** @copydoc RTTIPlainType::getSize */
		static BitLength getSize(const ShaderVariationParamValue& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = sizeof(uint8_t);
			dataSize += rtti_size(data.name);
			dataSize += rtti_size(data.value);

			rtti_add_header_size(dataSize, compress);
			return dataSize;
		}
	};

	template<>	struct RTTIPlainType<ShaderVariationParamInfo>
	{
		enum { id = TID_ShaderVariationParamInfo }; enum { hasDynamicSize = 1 };

		/** @copydoc RTTIPlainType::toMemory */
		static BitLength toMemory(const ShaderVariationParamInfo& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			static constexpr uint8_t VERSION = 0;

			return rtti_write_with_size_header(stream, data, compress, [&data, &stream]()
			{
				BitLength size = 0;
				size += rtti_write(VERSION, stream);
				size += rtti_write(data.name, stream);
				size += rtti_write(data.identifier, stream);
				size += rtti_write(data.isInternal, stream);
				size += rtti_write(data.values, stream);

				return size;
			});
		}

		/** @copydoc RTTIPlainType::fromMemory */
		static BitLength fromMemory(ShaderVariationParamInfo& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength size;
			rtti_read_size_header(stream, compress, size);

			uint8_t version;
			rtti_read(version, stream);
			assert(version == 0);

			rtti_read(data.name, stream);
			rtti_read(data.identifier, stream);
			rtti_read(data.isInternal, stream);
			rtti_read(data.values, stream);

			return size;
		}

		/** @copydoc RTTIPlainType::getSize */
		static BitLength getSize(const ShaderVariationParamInfo& data, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			BitLength dataSize = sizeof(uint8_t);
			dataSize += rtti_size(data.name);
			dataSize += rtti_size(data.identifier);
			dataSize += rtti_size(data.isInternal);
			dataSize += rtti_size(data.values);

			rtti_add_header_size(dataSize, compress);
			return dataSize;
		}
	};

	class BS_CORE_EXPORT SubShaderResourceRTTI : public RTTIType<SubShader, IReflectable, SubShaderResourceRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN(name, 0)
			BS_RTTI_MEMBER_REFLPTR(shader, 1)
		BS_END_RTTI_MEMBERS

	public:
		const String& getRTTIName() override
		{
			static String name = "SubShader";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_SubShader;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return bs_shared_ptr_new<SubShader>();
		}
	};

	class BS_CORE_EXPORT ShaderResourceRTTI : public RTTIType<ShaderResource, Resource, ShaderResourceRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_REFLPTR_ARRAY_NAMED(mTechniques, mDesc.techniques, 0)
			BS_RTTI_MEMBER_PLAIN(mName, 1)

			BS_RTTI_MEMBER_PLAIN_NAMED(mQueueSortType, mDesc.queueSortType, 7)
			BS_RTTI_MEMBER_PLAIN_NAMED(mQueuePriority, mDesc.queuePriority, 8)
			BS_RTTI_MEMBER_PLAIN_NAMED(mSeparablePasses, mDesc.separablePasses, 9)

			BS_RTTI_MEMBER_PLAIN_NAMED(mDataDefaultValues, mDesc.dataDefaultValues, 10)
			BS_RTTI_MEMBER_REFL_ARRAY_NAMED(mTextureDefaultValues, mDesc.textureDefaultValues, 11)
			BS_RTTI_MEMBER_REFLPTR_ARRAY_NAMED(mSamplerDefaultValues, mDesc.samplerDefaultValues, 12)

			BS_RTTI_MEMBER_PLAIN_NAMED(mFlags, mDesc.flags, 13)
			BS_RTTI_MEMBER_REFL_ARRAY_NAMED(mSubShaders, mDesc.subShaders, 14)

			BS_RTTI_MEMBER_PLAIN_ARRAY_NAMED(mParamAttributes, mDesc.paramAttributes, 15)
			BS_RTTI_MEMBER_PLAIN_ARRAY_NAMED(mVariationParams, mDesc.variationParams, 16)
		BS_END_RTTI_MEMBERS

		SHADER_DATA_PARAM_DESC& getDataParam(ShaderResource* obj, UINT32 idx)
		{
			auto iter = obj->mDesc.dataParams.begin();
			for(UINT32 i = 0; i < idx; i++) ++iter;

			return iter->second;
		}

		void setDataParam(ShaderResource* obj, UINT32 idx, SHADER_DATA_PARAM_DESC& val) { obj->mDesc.dataParams[val.name] = val; }
		UINT32 getDataParamsArraySize(ShaderResource* obj) { return (UINT32)obj->mDesc.dataParams.size(); }
		void setDataParamsArraySize(ShaderResource* obj, UINT32 size) {  } // Do nothing

		SHADER_OBJECT_PARAM_DESC& getTextureParam(ShaderResource* obj, UINT32 idx)
		{
			auto iter = obj->mDesc.textureParams.begin();
			for(UINT32 i = 0; i < idx; i++) ++iter;

			return iter->second;
		}

		void setTextureParam(ShaderResource* obj, UINT32 idx, SHADER_OBJECT_PARAM_DESC& val) { obj->mDesc.textureParams[val.name] = val; }
		UINT32 getTextureParamsArraySize(ShaderResource* obj) { return (UINT32)obj->mDesc.textureParams.size(); }
		void setTextureParamsArraySize(ShaderResource* obj, UINT32 size) {  } // Do nothing

		SHADER_OBJECT_PARAM_DESC& getSamplerParam(ShaderResource* obj, UINT32 idx)
		{
			auto iter = obj->mDesc.samplerParams.begin();
			for (UINT32 i = 0; i < idx; i++) ++iter;

			return iter->second;
		}

		void setSamplerParam(ShaderResource* obj, UINT32 idx, SHADER_OBJECT_PARAM_DESC& val) { obj->mDesc.samplerParams[val.name] = val; }
		UINT32 getSamplerParamsArraySize(ShaderResource* obj) { return (UINT32)obj->mDesc.samplerParams.size(); }
		void setSamplerParamsArraySize(ShaderResource* obj, UINT32 size) {  } // Do nothing

		SHADER_OBJECT_PARAM_DESC& getBufferParam(ShaderResource* obj, UINT32 idx)
		{
			auto iter = obj->mDesc.bufferParams.begin();
			for (UINT32 i = 0; i < idx; i++) ++iter;

			return iter->second;
		}

		void setBufferParam(ShaderResource* obj, UINT32 idx, SHADER_OBJECT_PARAM_DESC& val) { obj->mDesc.bufferParams[val.name] = val; }
		UINT32 getBufferParamsArraySize(ShaderResource* obj) { return (UINT32)obj->mDesc.bufferParams.size(); }
		void setBufferParamsArraySize(ShaderResource* obj, UINT32 size) {  } // Do nothing

		SHADER_PARAM_BLOCK_DESC& getParamBlock(ShaderResource* obj, UINT32 idx)
		{
			auto iter = obj->mDesc.paramBlocks.begin();
			for (UINT32 i = 0; i < idx; i++) ++iter;

			return iter->second;
		}

		void setParamBlock(ShaderResource* obj, UINT32 idx, SHADER_PARAM_BLOCK_DESC& val) { obj->mDesc.paramBlocks[val.name] = val; }
		UINT32 getParamBlocksArraySize(ShaderResource* obj) { return (UINT32)obj->mDesc.paramBlocks.size(); }
		void setParamBlocksArraySize(ShaderResource* obj, UINT32 size) {  } // Do nothing

	public:
		ShaderResourceRTTI()
		{
			addPlainArrayField("mDataParams", 2, &ShaderResourceRTTI::getDataParam, &ShaderResourceRTTI::getDataParamsArraySize,
				&ShaderResourceRTTI::setDataParam, &ShaderResourceRTTI::setDataParamsArraySize);
			addPlainArrayField("mTextureParams", 3, &ShaderResourceRTTI::getTextureParam, &ShaderResourceRTTI::getTextureParamsArraySize,
				&ShaderResourceRTTI::setTextureParam, &ShaderResourceRTTI::setTextureParamsArraySize);
			addPlainArrayField("mSamplerParams", 4, &ShaderResourceRTTI::getSamplerParam, &ShaderResourceRTTI::getSamplerParamsArraySize,
				&ShaderResourceRTTI::setSamplerParam, &ShaderResourceRTTI::setSamplerParamsArraySize);
			addPlainArrayField("mBufferParams", 5, &ShaderResourceRTTI::getBufferParam, &ShaderResourceRTTI::getBufferParamsArraySize,
				&ShaderResourceRTTI::setBufferParam, &ShaderResourceRTTI::setBufferParamsArraySize);
			addPlainArrayField("mParamBlocks", 6, &ShaderResourceRTTI::getParamBlock, &ShaderResourceRTTI::getParamBlocksArraySize,
				&ShaderResourceRTTI::setParamBlock, &ShaderResourceRTTI::setParamBlocksArraySize);
		}

		void onDeserializationEnded(IReflectable* obj, SerializationContext* context) override
		{
			ShaderResource* shader = static_cast<ShaderResource*>(obj);
			shader->initialize();
		}

		const String& getRTTIName() override
		{
			static String name = "Shader";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_Shader;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return ShaderResource::createEmpty();
		}
	};

	class BS_CORE_EXPORT ShaderMetaDataRTTI : public RTTIType < ShaderMetaData, ResourceMetaData, ShaderMetaDataRTTI >
	{
	private:
		Vector<String>& getIncludes(ShaderMetaData* obj) { return obj->includes; }
		void setIncludes(ShaderMetaData* obj, Vector<String>& includes) { obj->includes = includes; }

	public:
		ShaderMetaDataRTTI()
		{
			addPlainField("includes", 0, &ShaderMetaDataRTTI::getIncludes, &ShaderMetaDataRTTI::setIncludes);
		}

		const String& getRTTIName() override
		{
			static String name = "ShaderMetaData";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_ShaderMetaData;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return bs_shared_ptr_new<ShaderMetaData>();
		}
	};

	/** @} */
	/** @endcond */
}
