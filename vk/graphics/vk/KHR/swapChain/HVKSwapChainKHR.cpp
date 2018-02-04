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

		std::vector<VkSurfaceFormatKHR> HVKSwapChainKHR::sGetPhysicalDeviceSurfaceFormats(VkPhysicalDevice gpu, VkSurfaceKHR surface)
		{
			VkResult ret;
			uint32_t surfaceFormatCount = 0;
			ret = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &surfaceFormatCount, nullptr);
			if (ret != VK_SUCCESS) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSwapChainKHR, sGetPhysicalDeviceSurfaceFormats, ret) << "対応しているフォーマットの個数を取得できませんでした。";
			}
			std::vector<VkSurfaceFormatKHR> result(surfaceFormatCount);
			ret = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &surfaceFormatCount, result.data());
			if (ret != VK_SUCCESS) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSwapChainKHR, sGetPhysicalDeviceSurfaceFormats, ret) << "対応しているフォーマットの取得に失敗しました。";
			}
			return result;
		}

		VkSurfaceCapabilitiesKHR HVKSwapChainKHR::sGetPhysicalDeviceSurfaceCapabilities(VkPhysicalDevice gpu, VkSurfaceKHR surface)
		{
			VkSurfaceCapabilitiesKHR result;
			auto ret = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &result);
			if (ret != VK_SUCCESS) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSwapChainKHR, sGetPhysicalDeviceSurfaceCapabilities, ret) << "VkSurfaceCapabilitiesKHRの取得に失敗しました。";
			}
			return result;
		}

		std::vector<VkPresentModeKHR> HVKSwapChainKHR::sGetPresentModes(VkPhysicalDevice gpu, VkSurfaceKHR surface)
		{
			uint32_t presentCount = 0;
			auto ret = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentCount, nullptr);
			if (ret != VK_SUCCESS) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSwapChainKHR, sGetPresentModes, ret) << "対応しているVkPresentModeKHRの個数の取得に失敗しました。";
			}
			std::vector<VkPresentModeKHR> presentModes(presentCount);
			ret = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentCount, presentModes.data());
			if (ret != VK_SUCCESS) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSwapChainKHR, sGetPresentModes, ret) << "VkPresentModeKHRの取得に失敗しました。";
			}
			return presentModes;
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
		HVKSwapchainCreateInfoKHR HVKSwapchainCreateInfoKHR::sCreate(
			std::vector<VkSurfaceFormatKHR>& outSurfaceFormats,
			VkSurfaceCapabilitiesKHR& outSurfaceCapabilities,
			std::vector<VkPresentModeKHR>& outPresentModes,
			VkPhysicalDevice gpu,
			VkSurfaceKHR surface)
		{
			outSurfaceFormats = HVKSwapChainKHR::sGetPhysicalDeviceSurfaceFormats(gpu, surface);
			outSurfaceCapabilities = HVKSwapChainKHR::sGetPhysicalDeviceSurfaceCapabilities(gpu, surface);
			outPresentModes = HVKSwapChainKHR::sGetPresentModes(gpu, surface);

			HVKSwapchainCreateInfoKHR result;
			result.surface = surface;
			result.imageExtent = outSurfaceCapabilities.currentExtent;
			result.setFormat(HVKSwapchainCreateInfoKHR::sFindSurfaceFormat(outSurfaceFormats, VK_FORMAT_B8G8R8A8_UNORM));
			result.preTransform = outSurfaceCapabilities.currentTransform;
			result.imageArrayLayers = std::max(1u, outSurfaceCapabilities.maxImageArrayLayers);
			result.compositeAlpha = HVKSwapchainCreateInfoKHR::sGetSupportedCompositeAlpha(outSurfaceCapabilities.supportedCompositeAlpha, HVKSwapchainCreateInfoKHR::sDefaultFlagsInSortedByPriority);
			if (VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR == result.compositeAlpha) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSwapchainCreateInfoKHR, sCreate, VK_INCOMPLETE) << "compositeAlphaの設定に失敗";
			}
			result.presentMode = outPresentModes[0];

			return result;
		}

		HVKSwapchainCreateInfoKHR::HVKSwapchainCreateInfoKHR()noexcept
		{
			this->surface = nullptr;
			this->imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
			this->imageExtent.width = 0;
			this->imageExtent.height = 0;
			this->minImageCount = 2;

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

		HVKSwapchainCreateInfoKHR& HVKSwapchainCreateInfoKHR::setFormat(const VkSurfaceFormatKHR& surfaceFormat)
		{
			this->imageFormat = surfaceFormat.format;
			this->imageColorSpace = surfaceFormat.colorSpace;
			return *this;
		}

		HVKSwapchainCreateInfoKHR& HVKSwapchainCreateInfoKHR::setQueueFamilyIndices(const std::vector<uint32_t>& indices)
		{
			this->queueFamilyIndices = indices;
			this->queueFamilyIndexCount = static_cast<uint32_t>(this->queueFamilyIndices.size());
			this->pQueueFamilyIndices = this->queueFamilyIndices.data();
			this->imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			return *this;
		}

		VkSurfaceFormatKHR HVKSwapchainCreateInfoKHR::sFindSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats, VkFormat targetFormat)
		{
			for (auto& surFormat : surfaceFormats) {
				surFormat.colorSpace;
				if (surFormat.format == targetFormat) {
					return surFormat;
				}
			}
			return VkSurfaceFormatKHR{ VK_FORMAT_UNDEFINED, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
		}

		const std::array<VkCompositeAlphaFlagBitsKHR, 4> HVKSwapchainCreateInfoKHR::sDefaultFlagsInSortedByPriority = { {
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
		} };

	}

}