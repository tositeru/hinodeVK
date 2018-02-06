#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#include "../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKDescriptorSetLayout : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKDescriptorSetLayout(HVKDescriptorSetLayout& right)noexcept;
			HVKDescriptorSetLayout& operator=(HVKDescriptorSetLayout& right)noexcept;
		public:
			HVKDescriptorSetLayout();
			HVKDescriptorSetLayout(HVKDescriptorSetLayout&& right)noexcept;
			HVKDescriptorSetLayout& operator=(HVKDescriptorSetLayout&& right)noexcept;
			~HVKDescriptorSetLayout();

			void release()noexcept override;

			/// @brief ì¬
			/// @param[in] device
			/// @param[in] pInfo
			/// @exception HVKException
			void create(VkDevice device, VkDescriptorSetLayoutCreateInfo* pInfo);

		public:
			bool isGood()const noexcept;
			VkDescriptorSetLayout descriptorSetLayout()noexcept;
			operator VkDescriptorSetLayout()noexcept { return this->descriptorSetLayout(); }

		private:
			VkDescriptorSetLayout mDescSetLayout;
			VkDevice mParentDevice;
		};
	}

	namespace graphics
	{
		struct HVKDescriptorSetLayoutCreateInfo : public VkDescriptorSetLayoutCreateInfo
		{
			HVKDescriptorSetLayoutCreateInfo()noexcept;
			HVKDescriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutBinding* pBindings, uint32_t bindingCount)noexcept;
		};
	}

	namespace graphics
	{
		struct HVKDescriptorSetLayoutBinding : public VkDescriptorSetLayoutBinding
		{
			HVKDescriptorSetLayoutBinding()noexcept;
			HVKDescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType type, uint32_t count, VkShaderStageFlags stageFlags)noexcept;
		};
	}
}