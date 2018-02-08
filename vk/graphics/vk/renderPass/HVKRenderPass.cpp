#include "HVKRenderPass.h"

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKRenderPass::HVKRenderPass()
			: mRenderPass(VK_NULL_HANDLE)
			, mParentDevice(VK_NULL_HANDLE)
		{ }

		HVKRenderPass::HVKRenderPass(HVKRenderPass&& right)noexcept
			: mRenderPass(right.mRenderPass)
			, mParentDevice(right.mParentDevice)
		{
			right.mRenderPass = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;
		}

		HVKRenderPass& HVKRenderPass::operator=(HVKRenderPass&& right)noexcept
		{
			this->mRenderPass = right.mRenderPass;
			this->mParentDevice = right.mParentDevice;

			right.mRenderPass = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;

			return *this;
		}

		HVKRenderPass::~HVKRenderPass()
		{
			this->release();
		}

		void HVKRenderPass::release()noexcept
		{
			if(this->isGood()) {
				vkDestroyRenderPass(this->mParentDevice, this->mRenderPass, this->allocationCallbacksPointer());
				this->mParentDevice = VK_NULL_HANDLE;
				this->mRenderPass = VK_NULL_HANDLE;
			}
		}

		void HVKRenderPass::create(VkDevice device, VkRenderPassCreateInfo* pInfo)
		{
			this->release();

			auto ret = vkCreateRenderPass(device, pInfo, this->allocationCallbacksPointer(), &this->mRenderPass);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKRenderPass, create, ret) << "ì¬‚ÉŽ¸”s";
			}
			this->mParentDevice = device;
		}

		bool HVKRenderPass::isGood()const noexcept
		{
			return this->mRenderPass != VK_NULL_HANDLE && this->mParentDevice != VK_NULL_HANDLE;
		}

		VkRenderPass HVKRenderPass::renderPass()noexcept
		{
			assert(this->isGood());
			return this->mRenderPass;
		}
	}

	namespace graphics
	{
		HVKRenderPassCreateInfo::HVKRenderPassCreateInfo()noexcept
		{
			this->attachmentCount = 0;
			this->pAttachments = nullptr;
			this->subpassCount = 0;
			this->pSubpasses = nullptr;
			this->dependencyCount = 0;
			this->pDependencies = NULL;

			//ˆÈ‰ºŒÅ’è
			this->sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			this->pNext = NULL;
			this->flags = 0;
		}

		HVKRenderPassCreateInfo& HVKRenderPassCreateInfo::updateAttachments()noexcept
		{
			this->subpassCount = static_cast<decltype(this->subpassCount)>(this->subpasses.size());
			this->pSubpasses = this->subpasses.data();

			return *this;
		}

		HVKRenderPassCreateInfo& HVKRenderPassCreateInfo::updateSubpasses()noexcept
		{
			this->attachmentCount = static_cast<decltype(this->attachmentCount)>(this->attachments.size());
			this->pAttachments = this->attachments.data();

			return *this;
		}

		HVKRenderPassCreateInfo& HVKRenderPassCreateInfo::updateDependencies()noexcept
		{
			this->dependencyCount = static_cast<decltype(this->dependencyCount)>(this->dependencies.size());
			this->pDependencies = this->dependencies.data();

			return *this;
		}
	}

	namespace graphics
	{
		HVKAttachmentDescription::HVKAttachmentDescription()noexcept
		{
			this->format = VK_FORMAT_UNDEFINED;
			this->samples = VK_SAMPLE_COUNT_1_BIT;
			this->loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			this->storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			this->stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			this->stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			this->initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			this->finalLayout = VK_IMAGE_LAYOUT_GENERAL;
			this->flags = 0;

		}

		HVKAttachmentDescription& HVKAttachmentDescription::setFlags(VkAttachmentDescriptionFlags _flags)noexcept
		{
			this->flags = _flags;
			return *this;
		}

		HVKAttachmentDescription& HVKAttachmentDescription::setFormat(VkFormat _format)noexcept
		{
			this->format = _format;
			return *this;
		}

		HVKAttachmentDescription& HVKAttachmentDescription::setSampleCount(VkSampleCountFlagBits _samples)noexcept
		{
			this->samples = _samples;
			return *this;
		}

		HVKAttachmentDescription& HVKAttachmentDescription::setOp(VkAttachmentLoadOp _loadOp, VkAttachmentStoreOp _storeOp)noexcept
		{
			this->loadOp = _loadOp;
			this->storeOp = _storeOp;
			return *this;
		}

		HVKAttachmentDescription& HVKAttachmentDescription::setStencilOp(VkAttachmentLoadOp _loadOp, VkAttachmentStoreOp _storeOp)noexcept
		{
			this->stencilLoadOp = _loadOp;
			this->stencilStoreOp = _storeOp;
			return *this;
		}

		HVKAttachmentDescription& HVKAttachmentDescription::setLayout(VkImageLayout _initialLayout, VkImageLayout _finalLayout)noexcept
		{
			this->initialLayout = _initialLayout;
			this->finalLayout = _finalLayout;
			return *this;
		}
	}

	namespace graphics
	{

		HVKSubpassDescription::HVKSubpassDescription()noexcept
		{
			this->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			this->flags = 0;
			this->inputAttachmentCount = 0;
			this->pInputAttachments = nullptr;
			this->colorAttachmentCount = 0;
			this->pColorAttachments = nullptr;
			this->pResolveAttachments = nullptr;
			this->pDepthStencilAttachment = nullptr;
			this->preserveAttachmentCount = 0;
			this->pPreserveAttachments = nullptr;
		}

		HVKSubpassDescription& HVKSubpassDescription::setPipelineBindPoint(VkPipelineBindPoint bindPoint)noexcept
		{
			this->pipelineBindPoint = bindPoint;
			return *this;
		}

		HVKSubpassDescription& HVKSubpassDescription::setFlags(VkSubpassDescriptionFlags _flags)noexcept
		{
			this->flags = _flags;
			return *this;
		}

		HVKSubpassDescription& HVKSubpassDescription::updateInputAttachments()noexcept
		{
			this->inputAttachmentCount = static_cast<decltype(this->inputAttachmentCount)>(this->inputAttachments.size());
			this->pInputAttachments = this->inputAttachments.data();
			return *this;
		}

		HVKSubpassDescription& HVKSubpassDescription::updateColorAttachments()noexcept
		{
			this->colorAttachmentCount = static_cast<decltype(this->colorAttachmentCount)>(this->colorAttachments.size());
			this->pColorAttachments = this->colorAttachments.data();
			this->pResolveAttachments = this->resolveAttachments.empty() ? nullptr : this->resolveAttachments.data();
			return *this;
		}

		HVKSubpassDescription& HVKSubpassDescription::updateDepthAttachments()noexcept
		{
			this->pDepthStencilAttachment = &this->depthAttachment;
			return *this;
		}

		HVKSubpassDescription& HVKSubpassDescription::updatePreserveAttachments()noexcept
		{
			this->preserveAttachmentCount = static_cast<decltype(this->preserveAttachmentCount)>(this->preserveAttachments.size());
			this->pPreserveAttachments = this->preserveAttachments.data();
			return *this;
		}
	}

	namespace graphics
	{
		HVKAttachmentReference::HVKAttachmentReference()noexcept
			: HVKAttachmentReference(0, VK_IMAGE_LAYOUT_UNDEFINED)
		{ }

		HVKAttachmentReference::HVKAttachmentReference(uint32_t _attachment, VkImageLayout _layout)noexcept
		{
			this->attachment = _attachment;
			this->layout = _layout;
		}
	}

}