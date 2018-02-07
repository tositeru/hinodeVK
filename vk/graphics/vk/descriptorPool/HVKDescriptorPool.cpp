#include "HVKDescriptorPool.h"

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKDescriptorPool::HVKDescriptorPool()
			: mPool(VK_NULL_HANDLE)
			, mParentDevice(VK_NULL_HANDLE)
			, mEnableFreeDescriptorSet(false)
		{}

		HVKDescriptorPool::HVKDescriptorPool(HVKDescriptorPool&& right)noexcept
			: mPool(right.mPool)
			, mParentDevice(right.mParentDevice)
			, mEnableFreeDescriptorSet(right.mEnableFreeDescriptorSet)
		{
			right.mPool = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;
			right.mEnableFreeDescriptorSet = false;
		}

		HVKDescriptorPool& HVKDescriptorPool::operator=(HVKDescriptorPool&& right)noexcept
		{
			this->mPool = right.mPool;
			this->mParentDevice = right.mParentDevice;
			this->mEnableFreeDescriptorSet = right.mEnableFreeDescriptorSet;

			right.mPool = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;
			right.mEnableFreeDescriptorSet = false;

			return *this;
		}

		HVKDescriptorPool::~HVKDescriptorPool()
		{
			this->release();
		}

		void HVKDescriptorPool::release()noexcept
		{
			if (this->isGood()) {
				vkDestroyDescriptorPool(this->mParentDevice, this->mPool, this->allocationCallbacksPointer());
				this->mParentDevice = VK_NULL_HANDLE;
				this->mPool = VK_NULL_HANDLE;
				this->mEnableFreeDescriptorSet = false;
			}
		}

		void HVKDescriptorPool::create(VkDevice device, VkDescriptorPoolCreateInfo* pInfo)
		{
			this->release();

			auto ret = vkCreateDescriptorPool(device, pInfo, this->allocationCallbacksPointer(), &this->mPool);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKDescriptorPool, create, ret);
			}
			this->mParentDevice = device;
		}

		VkResult HVKDescriptorPool::resetPool(VkDescriptorPoolResetFlags flags)
		{
			assert(this->isGood());
			return vkResetDescriptorPool(this->mParentDevice, this->mPool, flags);
		}

		bool HVKDescriptorPool::isGood()const noexcept
		{
			return this->mPool != VK_NULL_HANDLE && this->mParentDevice != VK_NULL_HANDLE;
		}

		VkDescriptorPool HVKDescriptorPool::pool()noexcept
		{
			return this->mPool;
		}

		bool HVKDescriptorPool::enableFreeDescriptorSet()const noexcept
		{
			assert(this->isGood());
			return this->mEnableFreeDescriptorSet;
		}
	}

	namespace graphics
	{
		HVKDescriptorPoolCreateInfo::HVKDescriptorPoolCreateInfo()noexcept
			: HVKDescriptorPoolCreateInfo(nullptr, 0, 0)
		{}

		HVKDescriptorPoolCreateInfo::HVKDescriptorPoolCreateInfo(const VkDescriptorPoolSize* sizes, uint32_t sizeCount, uint32_t maxSet)noexcept
		{
			this->maxSets = maxSet;
			this->poolSizeCount = sizeCount;
			this->pPoolSizes = sizes;

			this->flags = 0;

			//ˆÈ‰ºŒÅ’è
			this->sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			this->pNext = NULL;
		}

		VkDescriptorPoolSize HVKDescriptorPoolCreateInfo::sMakePoolSize(VkDescriptorType type, uint32_t count)noexcept
		{
			VkDescriptorPoolSize type_count;
			type_count.type = type;
			type_count.descriptorCount = count;
			return type_count;
		}
	}

}