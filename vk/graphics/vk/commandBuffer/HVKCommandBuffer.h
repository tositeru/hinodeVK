#pragma once

#include <vulkan\vulkan.h>

#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKCommandBuffer : public IHVKInterface
		{
			HVKCommandBuffer(const HVKCommandBuffer&)=delete;
			HVKCommandBuffer& operator=(const HVKCommandBuffer&) = delete;
		public:
			HVKCommandBuffer()noexcept;
			HVKCommandBuffer(HVKCommandBuffer&& right)noexcept;
			HVKCommandBuffer& operator=(HVKCommandBuffer&& right)noexcept;
			~HVKCommandBuffer()noexcept;

			void release()noexcept override;

			void create(VkDevice device, VkCommandBufferAllocateInfo* pInfo);

		public:
			bool isGood()const noexcept override;
			VkCommandBuffer& buffer()noexcept;
			operator VkCommandBuffer&()noexcept { return this->buffer(); }

		private:
			VkCommandBuffer mCmdBuffer;
			VkDevice mParentDevice;
			VkCommandPool mParentPool;
		};
	}

	namespace graphics
	{
		struct HVKCommandBufferAllocateInfo : public VkCommandBufferAllocateInfo
		{
			HVKCommandBufferAllocateInfo()noexcept;
			HVKCommandBufferAllocateInfo(VkCommandPool cmdPool, VkCommandBufferLevel level)noexcept;
		};
	}
}