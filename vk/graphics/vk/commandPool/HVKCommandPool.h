#pragma once

#include <vulkan\vulkan.h>
#include "../HVKInterface.h"
#include "../allocationCallbacks/HVKAllocationCallbacks.h"

namespace hinode
{
	namespace graphics
	{
		class HVKCommandPool : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKCommandPool(const HVKCommandPool&) = delete;
			HVKCommandPool& operator=(const HVKCommandPool&) = delete;
		public:
			HVKCommandPool()noexcept;
			HVKCommandPool(HVKCommandPool&& right)noexcept;
			HVKCommandPool& operator=(HVKCommandPool&& right)noexcept;
			~HVKCommandPool()noexcept;

			void release()noexcept override;

			void create(VkDevice device, VkCommandPoolCreateInfo* pInfo);

		public:
			bool isGood()const noexcept;
			VkCommandPool& pool()noexcept;
			operator VkCommandPool()noexcept { return this->pool(); }

		private:
			VkCommandPool mPool;
			VkDevice mParentDevice;
		};
	}

	namespace graphics
	{
		struct HVKCommandPoolCreateInfo : public VkCommandPoolCreateInfo
		{
			HVKCommandPoolCreateInfo();
			HVKCommandPoolCreateInfo(int queueFamilyIndex, VkCommandPoolCreateFlags flags);
		};
	}
}