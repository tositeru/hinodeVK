#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#include "../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKRenderPass : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKRenderPass(HVKRenderPass& right)noexcept;
			HVKRenderPass& operator=(HVKRenderPass& right)noexcept;
		public:
			HVKRenderPass();
			HVKRenderPass(HVKRenderPass&& right)noexcept;
			HVKRenderPass& operator=(HVKRenderPass&& right)noexcept;
			~HVKRenderPass();

			void release()noexcept override;

			/// @brief ì¬
			/// @param[in] device
			/// @param[in] pInfo
			/// @exception HVKException
			void create(VkDevice device, VkRenderPassCreateInfo* pInfo);

		public:
			bool isGood()const noexcept;
			VkRenderPass renderPass()noexcept;
			operator VkRenderPass()noexcept { return this->renderPass(); }

		private:
			VkRenderPass mRenderPass;
			VkDevice mParentDevice;
		};
	}

	namespace graphics
	{
		struct HVKRenderPassCreateInfo : public VkRenderPassCreateInfo
		{
			std::vector<VkAttachmentDescription> attachments;
			std::vector<VkSubpassDescription> subpasses;
			std::vector<VkSubpassDependency> dependencies;

			HVKRenderPassCreateInfo()noexcept;

			HVKRenderPassCreateInfo& updateAttachments()noexcept;
			HVKRenderPassCreateInfo& updateSubpasses()noexcept;
			HVKRenderPassCreateInfo& updateDependencies()noexcept;
		};
	}

	namespace graphics
	{
		struct HVKAttachmentDescription : public VkAttachmentDescription
		{
			HVKAttachmentDescription()noexcept;

			HVKAttachmentDescription& setFlags(VkAttachmentDescriptionFlags flags)noexcept;
			HVKAttachmentDescription& setFormat(VkFormat format)noexcept;
			HVKAttachmentDescription& setSampleCount(VkSampleCountFlagBits samples)noexcept;
			HVKAttachmentDescription& setOp(VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp)noexcept;
			HVKAttachmentDescription& setStencilOp(VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp)noexcept;
			HVKAttachmentDescription& setLayout(VkImageLayout initialLayout, VkImageLayout finalLayout)noexcept;
		};
	}

	namespace graphics
	{
		struct HVKSubpassDescription : public VkSubpassDescription
		{
			std::vector<VkAttachmentReference> inputAttachments;
			std::vector<VkAttachmentReference> colorAttachments;
			std::vector<VkAttachmentReference> resolveAttachments;
			VkAttachmentReference depthAttachment;
			std::vector<uint32_t> preserveAttachments;

			HVKSubpassDescription()noexcept;

			HVKSubpassDescription& setPipelineBindPoint(VkPipelineBindPoint bindPoint)noexcept;
			HVKSubpassDescription& setFlags(VkSubpassDescriptionFlags flags)noexcept;

			HVKSubpassDescription& updateInputAttachments()noexcept;
			HVKSubpassDescription& updateColorAttachments()noexcept;
			HVKSubpassDescription& updateDepthAttachments()noexcept;
			HVKSubpassDescription& updatePreserveAttachments()noexcept;
		};
	}

	namespace graphics
	{
		struct HVKAttachmentReference : public VkAttachmentReference
		{
			HVKAttachmentReference()noexcept;
			HVKAttachmentReference(uint32_t attachment, VkImageLayout layout)noexcept;
		};
	}
}