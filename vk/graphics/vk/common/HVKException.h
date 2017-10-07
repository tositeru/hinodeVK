#pragma once

#include <vulkan\vulkan.h>

#include <sstream>
#include <functional>

namespace hinode
{
	namespace graphics
	{
		class HVKException
		{
		public:
			static void sSetWroteLogFunc(std::function<void(const HVKException*)> f);
		private:
			static std::function<void(const HVKException*)> sWriteLog;

		public:
			explicit HVKException(const char* filename, int line, const char* className, const char* funcName, VkResult result, HVKException* pPrevHVKException = nullptr);
			HVKException(HVKException& right);
			HVKException(HVKException&& right);
			~HVKException();

			void writeLog();

			template<typename T>
			HVKException& operator<<(T v)
			{
				this->mMessage << v;
				return *this;
			}

			void onAutoWroteLog();
			void offAutoWroteLog();

			std::stringstream& stream();
			const std::stringstream& stream()const;

			const std::string& filename()const;
			int line()const;
			const std::string& className()const;
			const std::string& funcName()const;
			const std::string message()const;
			VkResult result()const;

		private:
			std::string mFileName;
			int mLine;
			std::string mClassName;
			std::string mFuncName;
			std::stringstream mMessage;
			bool mIsAutoWroteLog;
			VkResult mResult;
		};

#define HINODE_GRAPHICS_CREATE_EXCEPTION(className, funcName, vkResult) \
	 ::hinode::graphics:: HVKException (__FILE__, __LINE__, #className, #funcName, vkResult)
#define HINODE_GRAPHICS_CREATE_EXCEPTION_WITH_PREV_HVKException(className, funcName, prevHVKException) \
	 ::hinode::graphics:: HVKException (__FILE__, __LINE__, #className, #funcName, &prevHVKException)

	}
}
