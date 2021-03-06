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

			/// @brief 作成
			/// @param[in] device
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkDevice device, VkImageCreateInfo* pCreateInfo);

			/// @brief スワップチェインのイメージを設定する
			///
			/// この関数で設定されたVkImageはrelease関数では破棄されません。
			/// @param[in] device
			/// @param[in] image
			void setSwapChainImage(VkDevice device, VkImage image);

			/// @brief ビューの作成
			/// @param[in] pInfo
			/// @retval size_t
			/// @exception HVKException
			size_t addView(VkImageViewCreateInfo* pInfo);

			VkMemoryRequirements getMemoryRequirements();

		public:
			bool isGood()const noexcept;
			VkImage image()noexcept;
			operator VkImage()noexcept { return this->image(); }
			VkImageView& getView(int index);

		private:
			bool mIsSwapChainImage;
			VkImage mImage;
			VkDevice mParentDevice;
			std::vector<VkImageView> mViews;
		};
	}

	namespace graphics
	{
		struct HVKImageCreateInfo : public VkImageCreateInfo
		{
			HVKImageCreateInfo()noexcept;
			HVKImageCreateInfo(VkImageType type, VkFormat format, uint32_t width, uint32_t height, uint32_t depth)noexcept;

			/// @brief 使用できるものがなければ,VK_IMAGE_TILING_MAX_ENUMを返します。
			static VkImageTiling sCheckTiling(VkFormatProperties props, VkFormatFeatureFlagBits checkFlag)noexcept;

			static uint32_t sCalMiplevel(uint32_t width, uint32_t height)noexcept;

			static HVKImageCreateInfo sMake2D(VkFormat format, uint32_t width, uint32_t height)noexcept;
		};
	}

	namespace graphics
	{
		struct HVKImageViewCreateInfo : public VkImageViewCreateInfo
		{
			HVKImageViewCreateInfo();
			HVKImageViewCreateInfo(VkImageViewType viewType, VkFormat format, VkImageAspectFlagBits aspectFlag);
		};
	}
}