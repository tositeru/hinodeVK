#include "HVKDescriptorSetLayout.h"

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKDescriptorSetLayout::HVKDescriptorSetLayout()
			: mDescSetLayout(VK_NULL_HANDLE)
			, mParentDevice(VK_NULL_HANDLE)
		{ }

		HVKDescriptorSetLayout::HVKDescriptorSetLayout(HVKDescriptorSetLayout&& right)noexcept
			: mDescSetLayout(right.mDescSetLayout)
			, mParentDevice(right.mParentDevice)
		{
			right.mDescSetLayout = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;
		}

		HVKDescriptorSetLayout& HVKDescriptorSetLayout::operator=(HVKDescriptorSetLayout&& right)noexcept
		{
			this->mDescSetLayout = right.mDescSetLayout;
			this->mParentDevice = right.mParentDevice;
			right.mDescSetLayout = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;

			return *this;
		}

		HVKDescriptorSetLayout::~HVKDescriptorSetLayout()
		{
			this->release();
		}

		void HVKDescriptorSetLayout::release()noexcept
		{
			if (this->isGood()) {
				vkDestroyDescriptorSetLayout(this->mParentDevice, this->mDescSetLayout, this->allocationCallbacksPointer());
				this->mDescSetLayout = VK_NULL_HANDLE;
				this->mParentDevice = VK_NULL_HANDLE;
			}
		}

		void HVKDescriptorSetLayout::create(VkDevice device, VkDescriptorSetLayoutCreateInfo* pInfo)
		{
			this->release();

			auto ret = vkCreateDescriptorSetLayout(device, pInfo, this->allocationCallbacksPointer(), &this->mDescSetLayout);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKDescriptorSetLayout, create, ret) << "作成に失敗しました";
			}
			this->mParentDevice = device;
		}

		bool HVKDescriptorSetLayout::isGood()const noexcept
		{
			return this->mDescSetLayout != VK_NULL_HANDLE && this->mParentDevice != VK_NULL_HANDLE;
		}

		VkDescriptorSetLayout HVKDescriptorSetLayout::descriptorSetLayout()noexcept
		{
			return this->mDescSetLayout;
		}
	}

	namespace graphics
	{
		HVKDescriptorSetLayoutCreateInfo::HVKDescriptorSetLayoutCreateInfo()noexcept
			: HVKDescriptorSetLayoutCreateInfo(nullptr, 0)
		{ }

		HVKDescriptorSetLayoutCreateInfo::HVKDescriptorSetLayoutCreateInfo(const VkDescriptorSetLayoutBinding* pBindings, uint32_t bindingCount)noexcept
		{
			this->bindingCount = bindingCount;
			this->pBindings = pBindings;
			this->flags = 0; //今のところコア機能にはない感じ？

			//以下固定
			this->sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			this->pNext = NULL;
		}
	}

	namespace graphics
	{
		HVKDescriptorSetLayoutBinding::HVKDescriptorSetLayoutBinding()noexcept
			: HVKDescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_SAMPLER, 0, VK_SHADER_STAGE_ALL)
		{ }

		HVKDescriptorSetLayoutBinding::HVKDescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType type, uint32_t count, VkShaderStageFlags stageFlags)noexcept
		{
			this->binding = binding;
			this->descriptorType = type;
			this->descriptorCount = count;

			this->stageFlags = stageFlags;
			this->pImmutableSamplers = nullptr;
		}
	}
}