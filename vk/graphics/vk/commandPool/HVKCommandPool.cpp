#include "HVKCommandPool.h"

#include <utility>
#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKCommandPool::HVKCommandPool()noexcept
			: mPool(nullptr)
			, mParentDevice(nullptr)
		{ }

		HVKCommandPool::HVKCommandPool(HVKCommandPool&& right)noexcept
		{
			*this = std::move(right);
		}

		HVKCommandPool& HVKCommandPool::operator=(HVKCommandPool&& right)noexcept
		{
			this->mPool = right.mPool;
			this->mParentDevice = right.mParentDevice;

			right.mPool = nullptr;
			right.mParentDevice = nullptr;

			return *this;
		}

		HVKCommandPool::~HVKCommandPool()noexcept
		{
			this->release();
		}

		void HVKCommandPool::release()noexcept
		{
			if (this->mPool) {
				vkDestroyCommandPool(this->mParentDevice, this->mPool, this->allocationCallbacksPointer());
				this->mPool = nullptr;
				this->mParentDevice = nullptr;
			}
		}

		void HVKCommandPool::create(VkDevice device, VkCommandPoolCreateInfo* pInfo)
		{
			this->release();

			auto ret = vkCreateCommandPool(device, pInfo, this->allocationCallbacksPointer(), &this->mPool);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKCommandPool, create, ret);
			}

			this->mParentDevice = device;
		}

		bool HVKCommandPool::isGood()const noexcept
		{
			return this->mPool && this->mParentDevice;
		}

		VkCommandPool& HVKCommandPool::pool()noexcept
		{
			return this->mPool;
		}
	}

	namespace graphics
	{
		HVKCommandPoolCreateInfo::HVKCommandPoolCreateInfo()
			: HVKCommandPoolCreateInfo(-1, 0)
		{ }

		HVKCommandPoolCreateInfo::HVKCommandPoolCreateInfo(int queueFamilyIndex, VkCommandPoolCreateFlags flags)
		{
			this->sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			this->pNext = nullptr;

			this->queueFamilyIndex = queueFamilyIndex;
			this->flags = flags;
		}
	}
}
