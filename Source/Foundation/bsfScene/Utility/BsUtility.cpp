//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Utility/BsUtility.h"
#include "Reflection/BsRTTIType.h"
#include "Scene/BsSceneObject.h"

namespace bs
{
	UINT32 Utility::getSceneObjectDepth(const HSceneObject& so)
	{
		HSceneObject parent = so->getParent();
		
		UINT32 depth = 0;
		while (parent != nullptr)
		{
			depth++;
			parent = parent->getParent();
		}

		return depth;
	}

	Vector<HComponent> Utility::findComponents(const HSceneObject& object, UINT32 typeId)
	{
		Vector<HComponent> output;

		Stack<HSceneObject> todo;
		todo.push(object);

		while(!todo.empty())
		{
			HSceneObject curSO = todo.top();
			todo.pop();

			const Vector<HComponent>& components = curSO->getComponents();
			for(auto& entry : components)
			{
				if (entry->getRTTI()->getRTTIId() == typeId)
					output.push_back(entry);
			}

			UINT32 numChildren = curSO->getNumChildren();
			for (UINT32 i = 0; i < numChildren; i++)
				todo.push(curSO->getChild(i));
		}

		return output;
	}

	class CoreSerializationContextRTTI :
		public RTTIType<CoreSerializationContext, SerializationContext, CoreSerializationContextRTTI>
	{
		const String& getRTTIName() override
		{
			static String name = "CoreSerializationContext";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_CoreSerializationContext;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			BS_EXCEPT(InternalErrorException, "Cannot instantiate an abstract class.");
			return nullptr;
		}
	};

	RTTITypeBase* CoreSerializationContext::getRTTIStatic()
	{
		return CoreSerializationContextRTTI::instance();
	}

	RTTITypeBase* CoreSerializationContext::getRTTI() const
	{
		return getRTTIStatic();
	}

}
