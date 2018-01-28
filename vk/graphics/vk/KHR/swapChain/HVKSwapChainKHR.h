#pragma once

#include <vulkan\vulkan.h>
#include <vector>

#include "../../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKSwapChainKHR : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKSwapChainKHR(HVKSwapChainKHR& right)noexcept = delete;
			HVKSwapChainKHR& operator=(HVKSwapChainKHR& right)noexcept = delete;

		public:
			/// @brief vkCreateSharedSwapchainsKHRを使用してスワップチェインを作成する
			/// 作成に失敗したときは例外を投げます
			/// @param[out] pOut
			/// @param[in] device
			/// @param[in] count
			/// @param[in] pCreateInfos
			/// @param[in] allocationCallbacks
			/// @exception HVKException
			static void sCreateSharedSwapchains(std::vector<VkSwapchainKHR>* pOut, VkDevice device, uint32_t count, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* allocationCallbacks = nullptr);

		public:
			HVKSwapChainKHR();
			HVKSwapChainKHR(HVKSwapChainKHR&& right)noexcept;
			HVKSwapChainKHR& operator=(HVKSwapChainKHR&& right)noexcept;
			~HVKSwapChainKHR();

			void release()noexcept override;

			/// @brief スワップチェインを作成します
			/// @param[in] device
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkDevice device, VkSwapchainCreateInfoKHR* pCreateInfo);

			/// @brief スワップチェインのバッファを取得する
			/// pImagesがnullptrの場合はpInOutCountにバッファの数が設定されます
			/// @param[inout] pInOutCount
			/// @param[out] pImages 
			/// @retval VkResult
			VkResult getSwapchainImages(uint32_t* pInOutCount, VkImage* pImages);

			/// @brief スワップチェインにあるすべてのバッファを取得する
			/// @retval std::vector<VkImage>
			std::vector<VkImage> getAllImages();

		public:
			bool isGood()const noexcept;
			VkSwapchainKHR swapChain()noexcept;
			operator VkSwapchainKHR()noexcept { return this->swapChain(); }

		private:
			VkSwapchainKHR mSwapChain;
			VkDevice mParentDevice;
		};
	}

	namespace graphics
	{
		struct HVKSwapchainCreateInfoKHR : public VkSwapchainCreateInfoKHR
		{
			HVKSwapchainCreateInfoKHR()noexcept;
			HVKSwapchainCreateInfoKHR(VkSurfaceKHR surface, VkFormat format, uint32_t width, uint32_t height, uint32_t minImageCount)noexcept;
		};
	}
}
