#include "HVKPipelineLayout.h"

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKPipelineLayout::HVKPipelineLayout()
			: mPipelineLayout(VK_NULL_HANDLE)
			, mParentDevice(VK_NULL_HANDLE)
		{ }

		HVKPipelineLayout::HVKPipelineLayout(HVKPipelineLayout&& right)noexcept
			: mPipelineLayout(right.mPipelineLayout)
			, mParentDevice(right.mParentDevice)
		{
			right.mPipelineLayout = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;
		}

		HVKPipelineLayout& HVKPipelineLayout::operator=(HVKPipelineLayout&& right)noexcept
		{
			this->mPipelineLayout = right.mPipelineLayout;
			this->mParentDevice = right.mParentDevice;
			right.mPipelineLayout = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;

			return *this;
		}

		HVKPipelineLayout::~HVKPipelineLayout()
		{
			this->release();
		}

		void HVKPipelineLayout::release()noexcept
		{
			if (this->isGood()) {
				vkDestroyPipelineLayout(this->mParentDevice, this->mPipelineLayout, this->allocationCallbacksPointer());
				this->mPipelineLayout = VK_NULL_HANDLE;
				this->mParentDevice = VK_NULL_HANDLE;
			}
		}

		void HVKPipelineLayout::create(VkDevice device, VkPipelineLayoutCreateInfo* pInfo)
		{
			this->release();

			auto ret = vkCreatePipelineLayout(device, pInfo, this->allocationCallbacksPointer(), &this->mPipelineLayout);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKPipelineLayout, create, ret) << "ì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½";
			}
			this->mParentDevice = device;
		}

		bool HVKPipelineLayout::isGood()const noexcept
		{
			return this->mPipelineLayout != VK_NULL_HANDLE && this->mParentDevice != VK_NULL_HANDLE;
		}

		VkPipelineLayout HVKPipelineLayout::pipelineLayout()noexcept
		{
			return this->mPipelineLayout;
		}
	}

	namespace graphics
	{
		HVKPipelineLayoutCreateInfo::HVKPipelineLayoutCreateInfo()noexcept
			: HVKPipelineLayoutCreateInfo(nullptr, 0)
		{ }

		HVKPipelineLayoutCreateInfo::HVKPipelineLayoutCreateInfo(const VkDescriptorSetLayout* pLayouts, uint32_t layoutCount)noexcept
		{
			this->pushConstantRangeCount = 0;
			this->pPushConstantRanges = NULL;
			this->setLayoutCount = layoutCount;
			this->pSetLayouts = pLayouts;

			this->flags = 0;	//«—ˆ‚Ì‚½‚ß‚É—\–ñ‚³‚ê‚Ä‚¢‚é

			//ˆÈ‰º“¯—l
			this->sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			this->pNext = NULL;
		}
	}
}