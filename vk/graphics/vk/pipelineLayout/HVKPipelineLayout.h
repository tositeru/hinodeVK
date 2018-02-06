#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#include "../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKPipelineLayout : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKPipelineLayout(HVKPipelineLayout& right)noexcept;
			HVKPipelineLayout& operator=(HVKPipelineLayout& right)noexcept;
		public:
			HVKPipelineLayout();
			HVKPipelineLayout(HVKPipelineLayout&& right)noexcept;
			HVKPipelineLayout& operator=(HVKPipelineLayout&& right)noexcept;
			~HVKPipelineLayout();

			void release()noexcept override;

			/// @brief ì¬
			/// @param[in] device
			/// @param[in] pInfo
			/// @exception HVKException
			void create(VkDevice device, VkPipelineLayoutCreateInfo* pInfo);

		public:
			bool isGood()const noexcept;
			VkPipelineLayout pipelineLayout()noexcept;
			operator VkPipelineLayout()noexcept { return this->pipelineLayout(); }

		private:
			VkPipelineLayout mPipelineLayout;
			VkDevice mParentDevice;
		};
	}

	namespace graphics
	{
		struct HVKPipelineLayoutCreateInfo : public VkPipelineLayoutCreateInfo
		{
			HVKPipelineLayoutCreateInfo()noexcept;
			HVKPipelineLayoutCreateInfo(const VkDescriptorSetLayout* pLayouts, uint32_t layoutCount)noexcept;

		};
	}
}