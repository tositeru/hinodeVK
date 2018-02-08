#include "HVKSemaphore.h"

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKSemaphore::HVKSemaphore()
			: mSemaphore(VK_NULL_HANDLE)
			, mParentDevice(VK_NULL_HANDLE)
		{}

		HVKSemaphore::HVKSemaphore(HVKSemaphore&& right)noexcept
			: mSemaphore(right.mSemaphore)
			, mParentDevice(right.mParentDevice)
		{
			right.mSemaphore = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;
		}

		HVKSemaphore& HVKSemaphore::operator=(HVKSemaphore&& right)noexcept
		{
			this->mSemaphore = right.mSemaphore;
			this->mParentDevice = right.mParentDevice;

			right.mSemaphore = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;

			return *this;
		}

		HVKSemaphore::~HVKSemaphore()
		{
			this->release();
		}

		void HVKSemaphore::release()noexcept
		{
			if (this->isGood()) {
				vkDestroySemaphore(this->mParentDevice, this->mSemaphore, this->allocationCallbacksPointer());
				this->mSemaphore = VK_NULL_HANDLE;
				this->mParentDevice = VK_NULL_HANDLE;
			}
		}

		void HVKSemaphore::create(VkDevice device, VkSemaphoreCreateInfo* pInfo)
		{
			this->release();

			auto ret = vkCreateSemaphore(device, pInfo, this->allocationCallbacksPointer(), &this->mSemaphore);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSemaphore, create, ret) << "ì¬‚ÉŽ¸”s";
			}
			this->mParentDevice = device;
		}

		bool HVKSemaphore::isGood()const noexcept
		{
			return this->mSemaphore != VK_NULL_HANDLE && this->mParentDevice != VK_NULL_HANDLE;
		}

		VkSemaphore HVKSemaphore::semaphore()noexcept
		{
			assert(this->isGood());
			return this->mSemaphore;
		}
	}

	namespace graphics
	{
		HVKSemaphoreCreateInfo::HVKSemaphoreCreateInfo()noexcept
			: HVKSemaphoreCreateInfo(0)
		{}

		HVKSemaphoreCreateInfo::HVKSemaphoreCreateInfo(VkSemaphoreCreateFlags flags)noexcept
		{
			this->flags = flags;

			//ˆÈ‰ºŒÅ’è
			this->sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			this->pNext = NULL;
		}
	}
}