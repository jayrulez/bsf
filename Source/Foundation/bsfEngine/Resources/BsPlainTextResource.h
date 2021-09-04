//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsPrerequisites.h"
#include "Resources/BsResource.h"

namespace bs
{
	/** @addtogroup Resources-Engine
	 *  @{
	 */

	/**	A resource containing plain text data. */
	class BS_EXPORT PlainTextResource : public Resource
	{
	public:
		/**	Returns the text contained in the resource. */
		const WString& getString() const { return mString; }

		/**	Modifies the text contained in the resource. */
		void setString(const WString& data) { mString = data; }

		/**	Creates a new text file resource with the specified string. */
		static PlainTextResourceHandle create(const WString& data);

		/** @name Internal
		 *  @{
		 */

		/**
		 * Creates an include file resource with the specified include string.
		 *
		 * @note	Internal method. Use create() for normal use.
		 */
		static SPtr<PlainTextResource> _createPtr(const WString& data);

		/** @} */
	private:
		PlainTextResource(const WString& data);

		WString mString;

		/************************************************************************/
		/* 								SERIALIZATION                      		*/
		/************************************************************************/
	public:
		friend class PlainTextResourceRTTI;
		static RTTITypeBase* getRTTIStatic();
		virtual RTTITypeBase* getRTTI() const override;
	};

	/** @} */
}
