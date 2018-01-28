#include "HVKSwapChainKHR.h"

#include <utility> //for std::move()

#include "../../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		void HVKSwapChainKHR::sCreateSharedSwapchains(std::vector<VkSwapchainKHR>* pOut, VkDevice device, uint32_t count, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* allocationCallback)
		{
			pOut->resize(count);
			pOut->shrink_to_fit();

			auto pAllocationCallback = allocationCallback ? allocationCallback : HVKAllocationCallbacks::sGetDefaultCallbacksPointer();
			auto ret = vkCreateSharedSwapchainsKHR(device, count, pCreateInfos, pAllocationCallback, pOut->data());
			if (ret != VK_SUCCESS) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSwapChainKHR, sCreateSharedSwapchains, ret);
			}
		}

		HVKSwapChainKHR::HVKSwapChainKHR()
			: mSwapChain(nullptr)
			, mParentDevice(nullptr)
		{ }

		HVKSwapChainKHR::HVKSwapChainKHR(HVKSwapChainKHR&& right)noexcept
		{
			this->mSwapChain = right.mSwapChain;
			this->mParentDevice = right.mParentDevice;
		}

		HVKSwapChainKHR& HVKSwapChainKHR::operator=(HVKSwapChainKHR&& right)noexcept
		{
			this->mSwapChain = right.mSwapChain;
			this->mParentDevice = right.mParentDevice;

			right.mSwapChain = nullptr;
			right.mParentDevice = nullptr;
			return *this;
		}

		HVKSwapChainKHR::~HVKSwapChainKHR()
		{
			this->release();
		}

		void HVKSwapChainKHR::release()noexcept
		{
			if (this->mSwapChain != nullptr) {
				vkDestroySwapchainKHR(this->mParentDevice, this->mSwapChain, this->allocationCallbacksPointer());
				this->mSwapChain = nullptr;
				this->mParentDevice = nullptr;
			}
		}

		void HVKSwapChainKHR::create(VkDevice device, VkSwapchainCreateInfoKHR* pCreateInfo)
		{
			auto ret = vkCreateSwapchainKHR(device, pCreateInfo, this->allocationCallbacksPointer(), &this->mSwapChain);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSwapChainKHR, create, ret);
			}
			this->mParentDevice = device;
		}

		VkResult HVKSwapChainKHR::getSwapchainImages(uint32_t* pInOutCount, VkImage* pImages)
		{
			assert(this->isGood() && pInOutCount != nullptr);
			return vkGetSwapchainImagesKHR(this->mParentDevice, this->mSwapChain, pInOutCount, pImages);
		}

		std::vector<VkImage> HVKSwapChainKHR::getAllImages()
		{
			assert(this->isGood());

			uint32_t imageCount = 0;
			auto ret = this->getSwapchainImages(&imageCount, nullptr);
			if (ret != VK_SUCCESS) {
				return {};
			}

			std::vector<VkImage> rawImages(imageCount);
			ret = this->getSwapchainImages(&imageCount, rawImages.data());
			if (ret != VK_SUCCESS) {
				return {};
			}

			return rawImages;
		}

		bool HVKSwapChainKHR::isGood()const noexcept
		{
			return this->mSwapChain != nullptr && this->mParentDevice != nullptr;
		}

		VkSwapchainKHR HVKSwapChainKHR::swapChain()noexcept
		{
			assert(this->isGood());
			return this->mSwapChain;
		}
	}

	namespace graphics
	{
		HVKSwapchainCreateInfoKHR::HVKSwapchainCreateInfoKHR()noexcept
			: HVKSwapchainCreateInfoKHR(nullptr, VK_FORMAT_B8G8R8A8_UNORM, 0u, 0u, 0)
		{ }

		HVKSwapchainCreateInfoKHR::HVKSwapchainCreateInfoKHR(VkSurfaceKHR surface, VkFormat format, uint32_t width, uint32_t height, uint32_t minImageCount)noexcept
		{
			this->surface = surface;
			this->imageFormat = format;
			this->imageExtent.width = width;
			this->imageExtent.height = height;
			this->minImageCount = minImageCount;

			this->presentMode = VK_PRESENT_MODE_FIFO_KHR;
			this->imageArrayLayers = 1;

			this->preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			this->compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;	//機能チェック的な処理が必要になってくる
			this->oldSwapchain = VK_NULL_HANDLE;
			this->clipped = true;
			this->imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
			this->imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			this->imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			this->queueFamilyIndexCount = 0;
			this->pQueueFamilyIndices = nullptr;

			// 以下固定値
			this->sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			this->pNext = nullptr;
			this->flags = 0;
		}

	}

}