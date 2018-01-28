#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#include "../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKImage : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKImage(HVKImage& right)noexcept = delete;
			HVKImage& operator=(HVKImage& right)noexcept = delete;

		public:
			static std::vector<HVKImage> sConvert(VkDevice device, VkImage* images, uint32_t count);

		public:
			HVKImage();
			HVKImage(HVKImage&& right)noexcept;
			HVKImage& operator=(HVKImage&& right)noexcept;
			~HVKImage();

			void release()noexcept override;

			/// @brief ì¬
			/// @param[in] device
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkDevice device, VkImageCreateInfo* pCreateInfo);

			/// @brief ‚·‚Å‚Éì¬‚³‚ê‚½‚à‚Ì‚ðÝ’è‚·‚é
			/// @param[in] device
			/// @param[in] image
			void set(VkDevice device, VkImage image);

			/// @brief ƒrƒ…[‚Ìì¬
			/// @param[in] pInfo
			/// @retval size_t
			size_t addView(VkImageViewCreateInfo* pInfo);

		public:
			bool isGood()const noexcept;
			VkImage image()noexcept;
			operator VkImage()noexcept { return this->image(); }
			VkImageView& getView(int index);

		private:
			VkImage mImage;
			VkDevice mParentDevice;
			std::vector<VkImageView> mViews;
		};
	}

	namespace graphics
	{
		struct HVKImageCreateInfo : public VkImageCreateInfo
		{
			HVKImageCreateInfo();
		};

		struct HVKImageViewCreateInfo : public VkImageViewCreateInfo
		{
			HVKImageViewCreateInfo();
			HVKImageViewCreateInfo(VkImage image, VkImageViewType viewType, VkFormat format);
		};
	}
}