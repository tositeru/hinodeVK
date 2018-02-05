#include "HVKImage.h"

#include <utility> //for std::move()

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		std::vector<HVKImage> HVKImage::sConvert(VkDevice device, VkImage* images, uint32_t count)
		{
			std::vector<HVKImage> ret(count);
			for (auto i = 0u; i < count; ++i) {
				ret[i].setSwapChainImage(device, images[i]);
			}
			return ret;
		}

		HVKImage::HVKImage()
			: mIsSwapChainImage(false)
			, mImage(nullptr)
			, mParentDevice(nullptr)
		{ }

		HVKImage::HVKImage(HVKImage&& right)noexcept
			: mIsSwapChainImage(right.mIsSwapChainImage)
			, mImage(right.mImage)
			, mParentDevice(right.mParentDevice)
		{
			right.mImage = nullptr;
			right.mParentDevice = nullptr;
		}

		HVKImage& HVKImage::operator=(HVKImage&& right)noexcept
		{
			this->release();
			this->mIsSwapChainImage = right.mIsSwapChainImage;
			this->mImage = right.mImage;
			this->mParentDevice = right.mParentDevice;
			this->mViews = right.mViews;

			right.mImage = nullptr;
			right.mParentDevice = nullptr;
			right.mViews.clear();
			right.mViews.shrink_to_fit();
			return *this;
		}

		HVKImage::~HVKImage()
		{
			this->release();
		}

		void HVKImage::release()noexcept
		{
			if (this->mImage != nullptr) {
				for (auto& view : this->mViews) {
					vkDestroyImageView(this->mParentDevice, view, this->allocationCallbacksPointer());
				}
				this->mViews.clear();
				this->mViews.shrink_to_fit();

				if (!this->mIsSwapChainImage) {
					vkDestroyImage(this->mParentDevice, this->mImage, this->allocationCallbacksPointer());
				}
				this->mIsSwapChainImage = false;
				this->mParentDevice = nullptr;
				this->mImage = nullptr;
			}
		}

		void HVKImage::create(VkDevice device, VkImageCreateInfo* pCreateInfo)
		{
			this->release();

			auto ret = vkCreateImage(device, pCreateInfo, this->allocationCallbacksPointer(), &this->mImage);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKImage, create, ret) << "作成に失敗しました";
			}

			this->mParentDevice = device;
		}

		void HVKImage::setSwapChainImage(VkDevice device, VkImage image)
		{
			this->release();

			this->mIsSwapChainImage = true;
			this->mParentDevice = device;
			this->mImage = image;
		}

		size_t HVKImage::addView(VkImageViewCreateInfo* pInfo)
		{
			assert(this->isGood());

			pInfo->image = this->mImage;
			VkImageView view;
			auto ret = vkCreateImageView(this->mParentDevice, pInfo, this->allocationCallbacksPointer(), &view);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKImage, addView, ret) << "ビューの作成に失敗しました";
			}
			this->mViews.push_back(view);
			return this->mViews.size() - 1;
		}

		VkMemoryRequirements HVKImage::getMemoryRequirements()
		{
			assert(this->isGood());

			VkMemoryRequirements result;
			vkGetImageMemoryRequirements(this->mParentDevice, this->mImage, &result);
			return result;
		}

		bool HVKImage::isGood()const noexcept
		{
			return this->mImage != nullptr;
		}

		VkImage HVKImage::image()noexcept
		{
			assert(this->isGood());
			return this->mImage;
		}

		VkImageView& HVKImage::getView(int index)
		{
			assert(this->isGood());
			return this->mViews[index];
		}
	}

	namespace graphics
	{
		HVKImageCreateInfo::HVKImageCreateInfo()noexcept
			: HVKImageCreateInfo(VK_IMAGE_TYPE_2D, VK_FORMAT_UNDEFINED, 1, 1, 1)
		{ }

		HVKImageCreateInfo::HVKImageCreateInfo(VkImageType type, VkFormat format, uint32_t width, uint32_t height, uint32_t depth)noexcept
		{
			this->flags = 0; // VkImageCreateFlagBits から値を設定する
			this->format = format; // VkFormat から値を設定する
			this->extent.width = width;		// 1以上を設定
			this->extent.height = height;	// 1以上を設定
			this->extent.depth = depth;		// 1以上を設定
			this->imageType = type; // VkImageType から値を設定する
			this->mipLevels = 1;		// 1以上を設定
			this->arrayLayers = 1;		// 1以上を設定
			this->samples = VK_SAMPLE_COUNT_1_BIT; //VkSampleCountFlagBits を設定
			this->tiling = VK_IMAGE_TILING_OPTIMAL;	//テクセルのメモリ配置の設定 VkImageTiling を参照
			this->usage = 0;	//イメージの使用法を設定 VkImageUsageFlagBits を組み合わせて設定する

			this->initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // VkImageLayout を使用する

			//キュー間の周りの設定
			this->sharingMode = VK_SHARING_MODE_EXCLUSIVE;	// VkSharingMode を使用
			this->queueFamilyIndexCount = 0;
			this->pQueueFamilyIndices = nullptr;

			//以下固定
			this->sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			this->pNext = nullptr;
		}

		VkImageTiling HVKImageCreateInfo::sCheckTiling(VkFormatProperties props, VkFormatFeatureFlagBits checkFlag)noexcept
		{
			VkImageTiling tiling = VK_IMAGE_TILING_MAX_ENUM;
			if (props.linearTilingFeatures & checkFlag) {
				tiling = VK_IMAGE_TILING_LINEAR;
			} else if (props.optimalTilingFeatures & checkFlag) {
				tiling = VK_IMAGE_TILING_OPTIMAL;
			}
			return tiling;
		}

		uint32_t HVKImageCreateInfo::sCalMiplevel(uint32_t width, uint32_t height)noexcept
		{
			uint32_t result = 0;
			while (1 < width || 1 < height) {
				width /= 2; height /= 2;
				result++;
			}
			return result + 1;
		}

		HVKImageCreateInfo HVKImageCreateInfo::sMake2D(VkFormat format, uint32_t width, uint32_t height)noexcept
		{
			return HVKImageCreateInfo(VK_IMAGE_TYPE_2D, format, width, height, static_cast<uint32_t>(1u));
		}

		//
		//	struct HVKImageViewCreateInfo
		//

		HVKImageViewCreateInfo::HVKImageViewCreateInfo()
			: HVKImageViewCreateInfo(VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_UNDEFINED, VK_IMAGE_ASPECT_FLAG_BITS_MAX_ENUM)
		{ }

		HVKImageViewCreateInfo::HVKImageViewCreateInfo(VkImageViewType viewType, VkFormat format, VkImageAspectFlagBits aspectFlag)
		{
			this->image = nullptr;
			this->viewType = viewType;
			this->format = format;

			this->components.r = VK_COMPONENT_SWIZZLE_R;
			this->components.g = VK_COMPONENT_SWIZZLE_G;
			this->components.b = VK_COMPONENT_SWIZZLE_B;
			this->components.a = VK_COMPONENT_SWIZZLE_A;

			this->subresourceRange.aspectMask = aspectFlag;
			this->subresourceRange.baseMipLevel = 0;
			this->subresourceRange.levelCount = 1;
			this->subresourceRange.baseArrayLayer = 0;
			this->subresourceRange.layerCount = 1;

			//以下固定
			this->sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			this->pNext = nullptr;
			this->flags = 0;
		}

	}
}