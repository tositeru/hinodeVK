#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#include "../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKDescriptorPool : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKDescriptorPool(HVKDescriptorPool& right)noexcept;
			HVKDescriptorPool& operator=(HVKDescriptorPool& right)noexcept;
		public:
			HVKDescriptorPool();
			HVKDescriptorPool(HVKDescriptorPool&& right)noexcept;
			HVKDescriptorPool& operator=(HVKDescriptorPool&& right)noexcept;
			~HVKDescriptorPool();

			void release()noexcept override;

			/// @brief ì¬
			/// @param[in] device
			/// @param[in] pInfo
			/// @exception HVKException
			void create(VkDevice device, VkDescriptorPoolCreateInfo* pInfo);

			VkResult resetPool(VkDescriptorPoolResetFlags flags = 0);

		public:
			bool isGood()const noexcept;
			VkDescriptorPool pool()noexcept;
			operator VkDescriptorPool()noexcept { return this->pool(); }
			bool enableFreeDescriptorSet()const noexcept;
		private:
			VkDescriptorPool mPool;
			VkDevice mParentDevice;
			bool mEnableFreeDescriptorSet;
		};
	}

	namespace graphics
	{
		struct HVKDescriptorPoolCreateInfo : public VkDescriptorPoolCreateInfo
		{
			HVKDescriptorPoolCreateInfo()noexcept;
			HVKDescriptorPoolCreateInfo(const VkDescriptorPoolSize* sizes, uint32_t sizeCount, uint32_t maxSet)noexcept;

			static VkDescriptorPoolSize sMakePoolSize(VkDescriptorType type, uint32_t count)noexcept;
		};
	}
}