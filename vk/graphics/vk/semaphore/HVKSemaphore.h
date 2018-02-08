#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#include "../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKSemaphore : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKSemaphore(HVKSemaphore& right)noexcept;
			HVKSemaphore& operator=(HVKSemaphore& right)noexcept;
		public:
			HVKSemaphore();
			HVKSemaphore(HVKSemaphore&& right)noexcept;
			HVKSemaphore& operator=(HVKSemaphore&& right)noexcept;
			~HVKSemaphore();

			void release()noexcept override;

			/// @brief ì¬
			/// @param[in] device
			/// @param[in] pInfo
			/// @exception HVKException
			void create(VkDevice device, VkSemaphoreCreateInfo* pInfo);

		public:
			bool isGood()const noexcept;
			VkSemaphore semaphore()noexcept;
			operator VkSemaphore()noexcept { return this->semaphore(); }

		private:
			VkSemaphore mSemaphore;
			VkDevice mParentDevice;
		};
	}

	namespace graphics
	{
		struct HVKSemaphoreCreateInfo : public VkSemaphoreCreateInfo
		{
			HVKSemaphoreCreateInfo()noexcept;
			HVKSemaphoreCreateInfo(VkSemaphoreCreateFlags flags)noexcept;
		};
	}
}