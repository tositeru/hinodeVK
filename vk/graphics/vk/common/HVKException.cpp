#include "HVKException.h"

#include "Log.h"
#include <Windows.h>

namespace hinode
{
	namespace graphics
	{
		std::function<void(const HVKException*)> HVKException::sWriteLog = [](const HVKException* pHVKException) {
			Log(Log::eERROR,
				pHVKException->filename().c_str(),
				pHVKException->line(),
				pHVKException->className().c_str(),
				pHVKException->funcName().c_str())
				<< pHVKException->message();
		};

		void HVKException::sSetWroteLogFunc(std::function<void(const HVKException*)> f)
		{
			HVKException::sWriteLog = f;
		}

		HVKException::HVKException(
			const char* filename,
			int line,
			const char* className,
			const char* funcName,
			VkResult result,
			HVKException* pPrevHVKException)
			: mIsAutoWroteLog(true)
			, mFileName(filename)
			, mLine(line)
			, mClassName(className)
			, mFuncName(funcName)
			, mResult(result)
		{
			if (pPrevHVKException) {
				pPrevHVKException->writeLog();
			}
		}

		HVKException::HVKException(HVKException& right)
			: mFileName(right.mFileName)
			, mLine(right.mLine)
			, mClassName(right.mClassName)
			, mFuncName(right.mFuncName)
			, mResult(right.mResult)
		{
			mMessage << right.mMessage.str();
			right.mIsAutoWroteLog = false;
		}

		HVKException::HVKException(HVKException&& right)
			: mFileName(right.mFileName)
			, mLine(right.mLine)
			, mClassName(right.mClassName)
			, mFuncName(right.mFuncName)
			, mResult(right.mResult)
		{
			mMessage << right.mMessage.str();
			right.mIsAutoWroteLog = false;
		}

		HVKException::~HVKException()
		{
			if (this->mIsAutoWroteLog) {
				this->writeLog();
			}
		}

		void HVKException::writeLog()
		{
			sWriteLog(this);
			this->offAutoWroteLog();
		}

		void HVKException::onAutoWroteLog() { this->mIsAutoWroteLog = true; }
		void HVKException::offAutoWroteLog() { this->mIsAutoWroteLog = false; }

		std::stringstream& HVKException::stream() { return this->mMessage; }
		const std::stringstream& HVKException::stream()const { return this->mMessage; }

		const std::string HVKException::message()const { return this->mMessage.str(); }
		const std::string& HVKException::filename()const { return this->mFileName; }
		int HVKException::line()const { return this->mLine; }
		const std::string& HVKException::className()const { return this->mClassName; }
		const std::string& HVKException::funcName()const { return this->mFuncName; }
		VkResult HVKException::result()const { return this->mResult; }
	}
}
