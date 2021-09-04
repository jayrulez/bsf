//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsCorePrerequisites.h"
#include "Localization/BsStringHandle.h"
#include "Localization/BsStringTableManager.h"

namespace bs
{
	StringHandle::StringHandle()
	{
		mStringData = StringTableManager::instance().getTable(0)->getStringData(u8"");

		if (mStringData->numParameters > 0)
			mParameters = bs_newN<String>(mStringData->numParameters);
	}

	StringHandle::StringHandle(UINT32 stringTableId)
	{
		mStringData = StringTableManager::instance().getTable(stringTableId)->getStringData(u8"");

		if (mStringData->numParameters > 0)
			mParameters = bs_newN<String>(mStringData->numParameters);
	}

	StringHandle::StringHandle(const String& identifierString, UINT32 stringTableId)
	{
		mStringData = StringTableManager::instance().getTable(stringTableId)->getStringData(identifierString);

		if (mStringData->numParameters > 0)
			mParameters = bs_newN<String>(mStringData->numParameters);
	}

	StringHandle::StringHandle(const String& identifierString, const String& defaultString, UINT32 stringTableId)
	{
		StringTableResourceHandle table = StringTableManager::instance().getTable(stringTableId);
		table->setString(identifierString, StringTableResource::DEFAULT_LANGUAGE, defaultString);

		mStringData = table->getStringData(identifierString);

		if (mStringData->numParameters > 0)
			mParameters = bs_newN<String>(mStringData->numParameters);
	}

	StringHandle::StringHandle(const StringHandle& copy)
	{
		mStringData = copy.mStringData;
		mIsDirty = copy.mIsDirty;
		mCachedString = copy.mCachedString;

		if (copy.mStringData->numParameters > 0)
		{
			mParameters = bs_newN<String>(mStringData->numParameters);
			if (copy.mParameters != nullptr)
			{
				for (UINT32 i = 0; i < mStringData->numParameters; i++)
					mParameters[i] = copy.mParameters[i];
			}

			mStringPtr = &mCachedString;
		}
		else
		{
			mParameters = nullptr;
			mStringPtr = &mStringData->string;
		}
	}

	StringHandle::~StringHandle()
	{
		if (mParameters != nullptr)
			bs_deleteN(mParameters, mStringData->numParameters);
	}

	StringHandle::operator const String& () const
	{
		return getValue();
	}

	StringHandle& StringHandle::operator=(const StringHandle& rhs)
	{
		if (mParameters != nullptr)
		{
			bs_deleteN(mParameters, mStringData->numParameters);
			mParameters = nullptr;
		}

		mStringData = rhs.mStringData;
		mIsDirty = rhs.mIsDirty;
		mCachedString = rhs.mCachedString;

		if (rhs.mStringData->numParameters > 0)
		{
			mParameters = bs_newN<String>(mStringData->numParameters);
			if (rhs.mParameters != nullptr)
			{
				for (UINT32 i = 0; i < mStringData->numParameters; i++)
					mParameters[i] = rhs.mParameters[i];
			}

			mStringPtr = &mCachedString;
		}
		else
		{
			mParameters = nullptr;
			mStringPtr = &mStringData->string;
		}

		return *this;
	}

	const String& StringHandle::getValue() const
	{
		if (mIsDirty)
		{
			if (mParameters != nullptr)
			{
				mStringData->concatenateString(mCachedString, mParameters, mStringData->numParameters);
				mStringPtr = &mCachedString;
			}
			else
			{
				mStringPtr = &mStringData->string;
			}

			mIsDirty = false;
		}

		return *mStringPtr;
	}

	void StringHandle::setParameter(UINT32 idx, const String& value)
	{
		if (idx >= mStringData->numParameters)
			return;

		mParameters[idx] = value;
		mIsDirty = true;
	}

	const StringHandle& StringHandle::dummy()
	{
		static StringHandle dummyVal;

		return dummyVal;
	}
}
