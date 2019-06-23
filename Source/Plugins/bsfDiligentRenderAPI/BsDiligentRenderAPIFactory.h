//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "Managers/BsRenderAPIFactory.h"
#include "Managers/BsRenderAPIManager.h"

namespace bs { namespace ct
{
	/** @addtogroup DiligentRenderAPI
	 *  @{
	 */
	/**	Handles creation of the Diligent render system. */
	class DiligentRenderAPIFactory final : public RenderAPIFactory
	{
	public:
		static constexpr const char* SystemName = "bsfDiligentRenderAPI";

		/** @copydoc RenderAPIFactory::create */
		void create() override;

		/** @copydoc RenderAPIFactory::name */
		const char* name() const override { return SystemName; }

	private:

		/**	Registers the factory with the render system manager when constructed. */
		class InitOnStart
		{
		public:
			InitOnStart()
			{
				static SPtr<RenderAPIFactory> newFactory;
				if(newFactory == nullptr)
				{
					newFactory = bs_shared_ptr_new<DiligentRenderAPIFactory>();
					RenderAPIManager::instance().registerFactory(newFactory);
				}
			}
		};

		static InitOnStart initOnStart; // Makes sure factory is registered on program start
	};

	/** @} */
}}
