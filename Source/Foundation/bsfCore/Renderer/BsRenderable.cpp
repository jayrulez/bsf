//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Renderer/BsRenderable.h"
#include "Private/RTTI/BsRenderableRTTI.h"
#include "RTTI/BsMathRTTI.h"
#include "Scene/BsSceneObject.h"
#include "Mesh/BsMesh.h"
#include "Material/BsMaterial.h"
#include "Math/BsBounds.h"
#include "Renderer/BsRenderer.h"
#include "Animation/BsAnimation.h"
#include "Animation/BsMorphShapes.h"
#include "RenderAPI/BsGpuBuffer.h"
#include "Animation/BsAnimationManager.h"
#include "Scene/BsSceneManager.h"
#include "CoreThread/BsCoreObjectSync.h"

namespace bs
{
	template<class T>
	bool isMeshValid(const T& mesh) { return false; }

	template<>
	bool isMeshValid(const HMesh& mesh) { return mesh.isLoaded(); }

	template<>
	bool isMeshValid(const SPtr<Mesh>& mesh) { return mesh != nullptr; }

	template<bool Core>
	TRenderable<Core>::TRenderable()
	{
		mMaterials.resize(1);
	}

	template <bool Core>
	void TRenderable<Core>::setTransform(const Transform& transform)
	{
		if (mMobility != ObjectMobility::Movable)
			return;

		mTransform = transform;
		mTfrmMatrix = transform.getMatrix();
		mTfrmMatrixNoScale = Matrix4::TRS(transform.getPosition(), transform.getRotation(), Vector3::ONE);

		_markCoreDirty(ActorDirtyFlag::Transform);
	}

	template<bool Core>
	void TRenderable<Core>::setMesh(const MeshType& mesh)
	{
		mMesh = mesh;

		int numSubMeshes = 0;
		if (isMeshValid(mesh))
			numSubMeshes = mesh->getProperties().getNumSubMeshes();

		mMaterials.resize(numSubMeshes);

		onMeshChanged();

		_markDependenciesDirty();
		_markResourcesDirty();
		_markCoreDirty();
	}

	template<bool Core>
	void TRenderable<Core>::setMaterial(UINT32 idx, const MaterialType& material)
	{
		if (idx >= (UINT32)mMaterials.size())
			return;

		mMaterials[idx] = material;

		_markDependenciesDirty();
		_markResourcesDirty();
		_markCoreDirty();
	}

	template<bool Core>
	void TRenderable<Core>::setMaterials(const Vector<MaterialType>& materials)
	{
		UINT32 numMaterials = (UINT32)mMaterials.size();
		UINT32 min = std::min(numMaterials, (UINT32)materials.size());

		for (UINT32 i = 0; i < min; i++)
			mMaterials[i] = materials[i];

		for (UINT32 i = min; i < numMaterials; i++)
			mMaterials[i] = nullptr;

		_markDependenciesDirty();
		_markResourcesDirty();
		_markCoreDirty();
	}

	template<bool Core>
	void TRenderable<Core>::setMaterial(const MaterialType& material)
	{
		setMaterial(0, material);
	}

	template <bool Core>
	typename TRenderable<Core>::MaterialType TRenderable<Core>::getMaterial(UINT32 idx) const
	{
		if(idx >= (UINT32)mMaterials.size())
			return nullptr;

		return mMaterials[idx];
	}

	template<bool Core>
	void TRenderable<Core>::setLayer(UINT64 layer)
	{
		const bool isPow2 = layer && !((layer - 1) & layer);

		if (!isPow2)
		{
			BS_LOG(Warning, Renderer, "Invalid layer provided. Only one layer bit may be set. Ignoring.");
			return;
		}

		mLayer = layer;
		_markCoreDirty();
	}	
	
	template<bool Core>
	void TRenderable<Core>::setOverrideBounds(const AABox& bounds)
	{
		mOverrideBounds = bounds;

		if(mUseOverrideBounds)
			_markCoreDirty();
	}

	template<bool Core>
	void TRenderable<Core>::setUseOverrideBounds(bool enable)
	{
		if (mUseOverrideBounds == enable)
			return;

		mUseOverrideBounds = enable;
		_markCoreDirty();
	}

	template<bool Core>
	void TRenderable<Core>::setWriteVelocity(bool enable)
	{
		if (mWriteVelocity == enable)
			return;

		mWriteVelocity = enable;
		_markCoreDirty();
	}

	template<bool Core>
	void TRenderable<Core>::setCullDistanceFactor(float factor)
	{
		mCullDistanceFactor = factor;

		_markCoreDirty();
	}

	template class TRenderable < false >;


	Renderable::Renderable()
		:mRendererId(0), mAnimationId((UINT64)-1), mMorphShapeVersion(0)
	{
	}

	Renderable::~Renderable()
	{
		if (mActive)
			ct::gRenderer()->notifyRenderableRemoved(this);
	}

	void Renderable::initialize()
	{
		ct::gRenderer()->notifyRenderableAdded(this);

		CoreObject::initialize();

		// Since we don't pass any information along to the core thread object on its construction, make sure the data
		// sync executes
		_markCoreDirty();

		// If any resources were deserialized before initialization, make sure the listener is notified
		_markResourcesDirty();
	}


	void Renderable::setAnimation(const SPtr<Animation>& animation)
	{
		mAnimation = animation;
		refreshAnimation();

		_markCoreDirty();
	}

	Bounds Renderable::getBounds() const
	{
		if(mUseOverrideBounds)
		{
			Sphere sphere(mOverrideBounds.getCenter(), mOverrideBounds.getRadius());

			Bounds bounds(mOverrideBounds, sphere);
			bounds.transformAffine(mTfrmMatrix);

			return bounds;
		}

		HMesh mesh = getMesh();

		if (!mesh.isLoaded())
		{
			const Transform& tfrm = getTransform();

			AABox box(tfrm.getPosition(), tfrm.getPosition());
			Sphere sphere(tfrm.getPosition(), 0.0f);

			return Bounds(box, sphere);
		}
		else
		{
			Bounds bounds = mesh->getProperties().getBounds();
			bounds.transformAffine(mTfrmMatrix);

			return bounds;
		}
	}

	SPtr<GpuBuffer> createBoneMatrixBuffer(UINT32 numBones)
	{
		GPU_BUFFER_DESC desc;
		desc.elementCount = numBones * 3;
		desc.elementSize = 0;
		desc.type = GBT_STANDARD;
		desc.format = BF_32X4F;
		desc.usage = GBU_DYNAMIC;

		SPtr<GpuBuffer> buffer = GpuBuffer::create(desc);
		UINT8* dest = (UINT8*)buffer->lock(0, numBones * 3 * sizeof(Vector4), GBL_WRITE_ONLY_DISCARD);

		// Initialize bone transforms to identity, so the object renders properly even if no animation is animating it
		for (UINT32 i = 0; i < numBones; i++)
		{
			memcpy(dest, &Matrix4::IDENTITY, 12 * sizeof(float)); // Assuming row-major format
			dest += 12 * sizeof(float);
		}

		buffer->unlock();

		return buffer;
	}

	void Renderable::createAnimationBuffers()
	{
		if (mAnimType == RenderableAnimType::Skinned || mAnimType == RenderableAnimType::SkinnedMorph)
		{
			SPtr<Skeleton> skeleton = mMesh->getSkeleton();
			UINT32 numBones = skeleton != nullptr ? skeleton->getNumBones() : 0;

			if (numBones > 0)
			{
				mBoneMatrixBuffer = createBoneMatrixBuffer(numBones);

				if (mWriteVelocity)
					mBonePrevMatrixBuffer = createBoneMatrixBuffer(numBones);
				else
					mBonePrevMatrixBuffer = nullptr;
			}
			else
			{
				mBoneMatrixBuffer = nullptr;
				mBonePrevMatrixBuffer = nullptr;
			}
		}
		else
		{
			mBoneMatrixBuffer = nullptr;
			mBonePrevMatrixBuffer = nullptr;
		}

		if (mAnimType == RenderableAnimType::Morph || mAnimType == RenderableAnimType::SkinnedMorph)
		{
			// Note: Not handling velocity writing for morph animations

			SPtr<MorphShapes> morphShapes = mMesh->getMorphShapes();

			UINT32 vertexSize = sizeof(Vector3) + sizeof(UINT32);
			UINT32 numVertices = morphShapes->getNumVertices();

			VERTEX_BUFFER_DESC desc;
			desc.vertexSize = vertexSize;
			desc.numVerts = numVertices;
			desc.usage = GBU_DYNAMIC;

			SPtr<VertexBuffer> vertexBuffer = VertexBuffer::create(desc);

			UINT32 totalSize = vertexSize * numVertices;
			UINT8* dest = (UINT8*)vertexBuffer->lock(0, totalSize, GBL_WRITE_ONLY_DISCARD);
			memset(dest, 0, totalSize);
			vertexBuffer->unlock();

			mMorphShapeBuffer = vertexBuffer;
		}
		else
			mMorphShapeBuffer = nullptr;

		mMorphShapeVersion = 0;
	}

	void Renderable::updateAnimationBuffers(const EvaluatedAnimationData& animData)
	{
		if (mAnimationId == (UINT64)-1)
			return;

		const EvaluatedAnimationData::AnimInfo* animInfo = nullptr;

		auto iterFind = animData.infos.find(mAnimationId);
		if (iterFind != animData.infos.end())
			animInfo = &iterFind->second;

		if (animInfo == nullptr)
			return;

		if (mAnimType == RenderableAnimType::Skinned || mAnimType == RenderableAnimType::SkinnedMorph)
		{
			const EvaluatedAnimationData::PoseInfo& poseInfo = animInfo->poseInfo;

			if (mWriteVelocity)
				std::swap(mBoneMatrixBuffer, mBonePrevMatrixBuffer);

			// Note: If multiple elements are using the same animation (not possible atm), this buffer should be shared by
			// all such elements
			UINT8* dest = (UINT8*)mBoneMatrixBuffer->lock(0, poseInfo.numBones * 3 * sizeof(Vector4), GBL_WRITE_ONLY_DISCARD);
			for (UINT32 j = 0; j < poseInfo.numBones; j++)
			{
				const Matrix4& transform = animData.transforms[poseInfo.startIdx + j];
				memcpy(dest, &transform, 12 * sizeof(float)); // Assuming row-major format

				dest += 12 * sizeof(float);
			}

			mBoneMatrixBuffer->unlock();
		}

		if (mAnimType == RenderableAnimType::Morph || mAnimType == RenderableAnimType::SkinnedMorph)
		{
			if (mMorphShapeVersion != animInfo->morphShapeInfo.version)
			{
				SPtr<MeshData> meshData = animInfo->morphShapeInfo.meshData;

				UINT32 bufferSize = meshData->getSize();
				UINT8* data = meshData->getData();

				mMorphShapeBuffer->writeData(0, bufferSize, data, BWT_DISCARD);
				mMorphShapeVersion = animInfo->morphShapeInfo.version;
			}
		}
	}

	void Renderable::updatePrevFrameAnimationBuffers()
	{
		if (!mWriteVelocity)
			return;

		if (mAnimType == RenderableAnimType::Skinned || mAnimType == RenderableAnimType::SkinnedMorph)
			std::swap(mBoneMatrixBuffer, mBonePrevMatrixBuffer);
	}

	void Renderable::onMeshChanged()
	{
		refreshAnimation();
	}

	void Renderable::refreshAnimation()
	{
		if (mAnimation == nullptr)
		{
			mAnimType = RenderableAnimType::None;
			return;
		}

		if (mMesh.isLoaded(false))
		{
			SPtr<Skeleton> skeleton = mMesh->getSkeleton();
			SPtr<MorphShapes> morphShapes = mMesh->getMorphShapes();

			if (skeleton != nullptr && morphShapes != nullptr)
				mAnimType = RenderableAnimType::SkinnedMorph;
			else if (skeleton != nullptr)
				mAnimType = RenderableAnimType::Skinned;
			else if (morphShapes != nullptr)
				mAnimType = RenderableAnimType::Morph;
			else
				mAnimType = RenderableAnimType::None;

			mAnimation->setSkeleton(mMesh->getSkeleton());
			mAnimation->setMorphShapes(mMesh->getMorphShapes());
		}
		else
		{
			mAnimType = RenderableAnimType::None;

			mAnimation->setSkeleton(nullptr);
			mAnimation->setMorphShapes(nullptr);
		}
	}

	void Renderable::_updateState(const SceneObject& so, bool force)
	{
		UINT32 curHash = so.getTransformHash();
		if (curHash != mHash || force)
		{
			// If skinned animation, don't include own transform since that will be handled by root bone animation
			bool ignoreOwnTransform;
			if (mAnimType == RenderableAnimType::Skinned || mAnimType == RenderableAnimType::SkinnedMorph)
				ignoreOwnTransform = mAnimation->_getAnimatesRoot();
			else
				ignoreOwnTransform = false;

			if (ignoreOwnTransform)
			{
				// Note: Technically we're checking child's hash but using parent's transform. Ideally we check the parent's
				// hash to reduce the number of required updates.
				HSceneObject parentSO = so.getParent();
				if (parentSO != nullptr)
					setTransform(parentSO->getTransform());
				else
					setTransform(Transform());
			}
			else
				setTransform(so.getTransform());

			mHash = curHash;
		}

		// Hash now matches so transform won't be applied twice, so we can just call base class version
		SceneActor::_updateState(so, force);
	}

	void Renderable::_markCoreDirty(ActorDirtyFlag flag)
	{
		markCoreDirty((UINT32)flag);
		notifyUpdated((UINT32)flag);
	}

	void Renderable::_markDependenciesDirty()
	{
		markDependenciesDirty();
	}

	void Renderable::_markResourcesDirty()
	{
		markListenerResourcesDirty();
	}

	void Renderable::getCoreDependencies(Vector<CoreObject*>& dependencies)
	{
		if (mMesh.isLoaded())
			dependencies.push_back(mMesh.get());

		for (auto& material : mMaterials)
		{
			if (material.isLoaded())
				dependencies.push_back(material.get());
		}
	}

	void Renderable::onDependencyDirty(CoreObject* dependency, UINT32 dirtyFlags)
	{
		if(mMesh.isLoaded(false) && mMesh.get() == dependency)
		{
			CoreObject::onDependencyDirty(dependency, dirtyFlags);
			return;
		}

		if(((UINT32)MaterialDirtyFlags::Shader & dirtyFlags) != 0)
			CoreObject::onDependencyDirty(dependency, dirtyFlags);
	}

	void Renderable::getListenerResources(Vector<HResource>& resources)
	{
		if (mMesh != nullptr)
			resources.push_back(mMesh);

		for (auto& material : mMaterials)
		{
			if (material != nullptr)
				resources.push_back(material);
		}
	}

	void Renderable::notifyResourceLoaded(const HResource& resource)
	{
		if (resource == mMesh)
			onMeshChanged();

		markDependenciesDirty();
		markCoreDirty();
	}

	void Renderable::notifyResourceChanged(const HResource& resource)
	{
		if(resource == mMesh)
			onMeshChanged();

		markDependenciesDirty();
		markCoreDirty();
	}

	SPtr<Renderable> Renderable::create()
	{
		SPtr<Renderable> handlerPtr = createEmpty();
		handlerPtr->initialize();

		return handlerPtr;
	}

	SPtr<Renderable> Renderable::createEmpty()
	{
		Renderable* handler = new (bs_alloc<Renderable>()) Renderable();
		SPtr<Renderable> handlerPtr = bs_core_ptr<Renderable>(handler);
		handlerPtr->_setThisPtr(handlerPtr);

		return handlerPtr;
	}

	RTTITypeBase* Renderable::getRTTIStatic()
	{
		return RenderableRTTI::instance();
	}

	RTTITypeBase* Renderable::getRTTI() const
	{
		return Renderable::getRTTIStatic();
	}


	void Renderable::notifyUpdated(UINT32 dirtyFlags)
	{
		bool oldIsActive = mActive;
		UINT32 updateEverythingFlag = (UINT32)ActorDirtyFlag::Everything
			| (UINT32)ActorDirtyFlag::Active
			| (UINT32)ActorDirtyFlag::Dependency;

		if ((dirtyFlags & updateEverythingFlag) != 0)
		{
			createAnimationBuffers();

			// Create special vertex declaration if using morph shapes
			if (mAnimType == RenderableAnimType::Morph || mAnimType == RenderableAnimType::SkinnedMorph)
			{
				SPtr<VertexDataDesc> vertexDesc = VertexDataDesc::create();
				*vertexDesc = *mMesh->getVertexDesc();

				vertexDesc->addVertElem(VET_FLOAT3, VES_POSITION, 1, 1);
				vertexDesc->addVertElem(VET_UBYTE4_NORM, VES_NORMAL, 1, 1);

				mMorphVertexDeclaration = VertexDeclaration::create(vertexDesc);
			}
			else
				mMorphVertexDeclaration = nullptr;

			if (oldIsActive != mActive)
			{
				if (mActive)
					ct::gRenderer()->notifyRenderableAdded(this);
				else
					ct::gRenderer()->notifyRenderableRemoved(this);
			}
			else
			{
				ct::gRenderer()->notifyRenderableRemoved(this);
				ct::gRenderer()->notifyRenderableAdded(this);
			}
		}
		else if ((dirtyFlags & (UINT32)ActorDirtyFlag::Mobility) != 0)
		{
			ct::gRenderer()->notifyRenderableRemoved(this);
			ct::gRenderer()->notifyRenderableAdded(this);
		}
		else if ((dirtyFlags & (UINT32)ActorDirtyFlag::Transform) != 0)
		{
			if (mActive)
				ct::gRenderer()->notifyRenderableUpdated(this);
		}
	}
}
