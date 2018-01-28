#pragma once

#include "../HVKInterface.h"
#include "../allocationCallbacks/HVKAllocationCallbacks.h"

namespace hinode
{
	namespace graphics
	{
		class HVKDevice : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKDevice(HVKDevice& right)noexcept = delete;
			HVKDevice& operator=(HVKDevice& right)noexcept = delete;

		public:
			HVKDevice();
			HVKDevice(HVKDevice&& right)noexcept;
			HVKDevice& operator=(HVKDevice&& right)noexcept;
			~HVKDevice();

			void release()noexcept override;

			void create(VkPhysicalDevice physicalDevice, VkDeviceCreateInfo* pInfo);

			VkResult waitIdle()noexcept;

		public:
			bool isGood()const noexcept;
			VkDevice device()noexcept;
			operator VkDevice()noexcept { return this->device(); }

		private:
			VkDevice mDevice;
		};
	}

	namespace graphics
	{
		struct HVKDeviceCreateInfo : public VkDeviceCreateInfo
		{
			HVKDeviceCreateInfo();
			HVKDeviceCreateInfo(VkDeviceQueueCreateInfo* pQueueInfos, uint32_t queueInfoCount);
		};
	}
	namespace graphics
	{
		struct HVKDeviceQueueCreateInfo : public VkDeviceQueueCreateInfo
		{
			HVKDeviceQueueCreateInfo();
			HVKDeviceQueueCreateInfo(uint32_t familyIndex, uint32_t count);
		};
	}

}
