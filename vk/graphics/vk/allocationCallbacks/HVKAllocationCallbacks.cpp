#include "HVKAllocationCallbacks.h"

namespace hinode
{
	namespace graphics
	{
		static VkAllocationCallbacks defaultCallbackInstance;
		static VkAllocationCallbacks* pDefaultCallback = nullptr;

		void HVKAllocationCallbacks::sSetDefaultCallbacks(VkAllocationCallbacks callbacks)noexcept
		{
			defaultCallbackInstance = callbacks;
			if (   nullptr != callbacks.pfnAllocation
				&& nullptr != callbacks.pfnFree
				&& nullptr != callbacks.pfnInternalAllocation
				&& nullptr != callbacks.pfnInternalFree
				&& nullptr != callbacks.pfnReallocation
				&& nullptr != callbacks.pUserData ) {
				pDefaultCallback = &defaultCallbackInstance;
			} else {
				pDefaultCallback = nullptr;
			}
		}

		const VkAllocationCallbacks* HVKAllocationCallbacks::sGetDefaultCallbacksPointer()noexcept
		{
			return pDefaultCallback;
		}

		HVKAllocationCallbacks::HVKAllocationCallbacks()
			: mpCallbacks(nullptr)
		{ }

		HVKAllocationCallbacks::~HVKAllocationCallbacks()
		{

		}

		void HVKAllocationCallbacks::setCallbacks(VkAllocationCallbacks* pCallbacks)noexcept
		{
			this->mpCallbacks = pCallbacks;
		}

		const VkAllocationCallbacks* HVKAllocationCallbacks::allocationCallbacksPointer()const noexcept
		{
			return this->mpCallbacks;
		}

	}
}