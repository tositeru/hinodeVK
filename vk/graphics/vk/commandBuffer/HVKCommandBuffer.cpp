#include "HVKCommandBuffer.h"

#include <utility> // for std::move
#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKCommandBuffer::HVKCommandBuffer()noexcept
			: mCmdBuffer(nullptr)
			, mParentDevice(nullptr)
			, mParentPool(nullptr)
		{}

		HVKCommandBuffer::HVKCommandBuffer(HVKCommandBuffer&& right)noexcept
		{
			*this = std::move(right);
		}

		HVKCommandBuffer& HVKCommandBuffer::operator=(HVKCommandBuffer&& right)noexcept
		{
			this->mCmdBuffer = right.mCmdBuffer;
			this->mParentDevice = right.mParentDevice;
			this->mParentPool = right.mParentPool;

			right.mCmdBuffer = nullptr;
			right.mParentDevice = nullptr;
			right.mParentPool = nullptr;
			return *this;
		}

		HVKCommandBuffer::~HVKCommandBuffer()noexcept
		{
			this->release();
		}

		void HVKCommandBuffer::release()noexcept
		{
			if (this->mCmdBuffer) {
				vkFreeCommandBuffers(this->mParentDevice, this->mParentPool, 1, &this->mCmdBuffer);
				this->mParentDevice = nullptr;
				this->mParentPool = nullptr;
				this->mCmdBuffer = nullptr;
			}
		}

		void HVKCommandBuffer::create(VkDevice device, VkCommandBufferAllocateInfo* pInfo)
		{
			this->release();

			pInfo->commandBufferCount = 1;
			auto ret = vkAllocateCommandBuffers(device, pInfo, &this->mCmdBuffer);
			if (ret != VK_SUCCESS) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKCommandBuffer, create, ret);
			}

			this->mParentDevice = device;
			this->mParentPool = pInfo->commandPool;
		}

		bool HVKCommandBuffer::isGood()const noexcept
		{
			return this->mCmdBuffer && this->mParentDevice;
		}

		VkCommandBuffer& HVKCommandBuffer::buffer()noexcept
		{
			return this->mCmdBuffer;
		}
	}

	namespace graphics
	{
		HVKCommandBufferAllocateInfo::HVKCommandBufferAllocateInfo()noexcept
			: HVKCommandBufferAllocateInfo(nullptr, VK_COMMAND_BUFFER_LEVEL_PRIMARY)
		{ }

		HVKCommandBufferAllocateInfo::HVKCommandBufferAllocateInfo(VkCommandPool cmdPool, VkCommandBufferLevel level)noexcept
		{
			this->commandPool = cmdPool;
			this->level = level;
			this->commandBufferCount = 1;

			//ˆÈ‰º, ŒÅ’è
			this->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			this->pNext = nullptr;
		}
	}

}