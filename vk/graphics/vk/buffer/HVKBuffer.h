#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#include "../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKBuffer : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKBuffer(HVKBuffer& right)noexcept;
			HVKBuffer& operator=(HVKBuffer& right)noexcept;
		public:
			HVKBuffer();
			HVKBuffer(HVKBuffer&& right)noexcept;
			HVKBuffer& operator=(HVKBuffer&& right)noexcept;
			~HVKBuffer();

			void release()noexcept override;

			/// @brief ì¬
			/// @param[in] device
			/// @param[in] pInfo
			/// @exception HVKException
			void create(VkDevice device, VkBufferCreateInfo* pInfo);

			/// @brief ƒrƒ…[‚Ìì¬
			/// @param[in] pInfo
			/// @retval size_t
			/// @exception HVKException
			size_t addView(VkBufferViewCreateInfo* pInfo);

			VkMemoryRequirements getMemoryRequirements();

		public:
			bool isGood()const noexcept;
			VkBuffer buffer()noexcept;
			operator VkBuffer()noexcept { return this->buffer(); }
			VkBufferView view(int index);

		private:
			VkBuffer mBuffer;
			VkDevice mParentDevice;
			std::vector<VkBufferView> mViews;
		};
	}

	namespace graphics
	{
		struct HVKBufferCreateInfo : public VkBufferCreateInfo
		{
			HVKBufferCreateInfo()noexcept;
			HVKBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage)noexcept;

		};
	}
}