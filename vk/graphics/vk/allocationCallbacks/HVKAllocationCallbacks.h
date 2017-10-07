#pragma once

#include <vulkan\vulkan.h>

namespace hinode
{
	namespace graphics
	{
		class HVKAllocationCallbacks
		{
		public:
			static void sSetDefaultCallbacks(VkAllocationCallbacks callbacks)noexcept;
			static const VkAllocationCallbacks* sGetDefaultCallbacksPointer()noexcept;

		public:
			HVKAllocationCallbacks();
			virtual ~HVKAllocationCallbacks();

			void setCallbacks(VkAllocationCallbacks* pCallbacks)noexcept;

		public:
			const VkAllocationCallbacks* allocationCallbacksPointer()const noexcept;

		private:
			VkAllocationCallbacks* mpCallbacks;
		};
	}
}