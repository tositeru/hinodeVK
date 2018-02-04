#pragma once

#include <vulkan\vulkan.h>
#include <vector>
#include <array>

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

			static std::vector<VkSurfaceFormatKHR> sGetPhysicalDeviceSurfaceFormats(VkPhysicalDevice gpu, VkSurfaceKHR surface);

			static VkSurfaceCapabilitiesKHR sGetPhysicalDeviceSurfaceCapabilities(VkPhysicalDevice gpu, VkSurfaceKHR surface);

			static std::vector<VkPresentModeKHR> sGetPresentModes(VkPhysicalDevice gpu, VkSurfaceKHR surface);

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
		public:
			/// @brief VkSwapChainKHRの作成にはいくつか制限事項があるので、VkSwapchainCreateInfoKHRを設定するにあたって必要となるデータも一緒に作成するよう強制するために作成
			/// @param[out] outSurfaceFormats
			/// @param[out] outSurfaceCapabilities
			/// @param[out] outPresentModes
			/// @param[in] gpu
			/// @param[in] surface
			/// @retval HVKSwapchainCreateInfoKHR
			/// @exception HVKException
			static HVKSwapchainCreateInfoKHR sCreate(std::vector<VkSurfaceFormatKHR>& outSurfaceFormats, VkSurfaceCapabilitiesKHR& outSurfaceCapabilities, std::vector<VkPresentModeKHR>& outPresentModes, VkPhysicalDevice gpu, VkSurfaceKHR surface);

		private:
			/// @brief VkSwapChainKHRの作成にはいくつか制限事項があるので、この構造体を作成するときに必要となるものを作ることを強要するためprivateにしてます
			HVKSwapchainCreateInfoKHR()noexcept;

		public:
			std::vector<uint32_t> queueFamilyIndices;

			HVKSwapchainCreateInfoKHR& setFormat(const VkSurfaceFormatKHR& surfaceFormat);

			/// @brief pQueueFamilyIndices, queueFamilyIndexCountを設定する関数
			/// imageSharingModeはVK_SHARING_MODE_CONCURRENTに設定されます。
			///
			/// GraphicsとPresentキューのファミリーが異なるケースがあるため、その時につかう関数
			/// @param[in] indices
			/// @retval HVKSwapchainCreateInfoKHR
			HVKSwapchainCreateInfoKHR& setQueueFamilyIndices(const std::vector<uint32_t>& indices);

			/// @brief surfaceFormatsからtargetFormatをもつものを探す関数
			///
			/// 見つからないときはformat = VK_FORMAT_UNDEFINED, colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHRを設定したVkSurfaceFormatKHRを返します
			/// @param[in] surfaceFormats
			/// @param[in] targetFormat
			/// @retval VkSurfaceFormatKHR
			static VkSurfaceFormatKHR sFindSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats, VkFormat targetFormat);

			/// @brief sGetSupportedCompositeAlpha関数に使用することを想定して作成
			static const std::array<VkCompositeAlphaFlagBitsKHR, 4> sDefaultFlagsInSortedByPriority;

			/// @brief サポートされているcompositeAlphaを取得する
			/// flagInSortedByPriorityに巣ポートされているものがなければVK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHRを返します
			/// @param[in] supportedFlag
			/// @param[in] flagInSortedByPriority 設定対象となるフラグ。添え字が若いものほど優先順位が高くなります
			/// @retval VkCompositeAlphaFlagBitsKHR
			template<size_t FlagCount>
			static VkCompositeAlphaFlagBitsKHR sGetSupportedCompositeAlpha(VkCompositeAlphaFlagsKHR supportedFlag, const std::array<VkCompositeAlphaFlagBitsKHR, FlagCount>& flagInSortedByPriority)
			{
				VkCompositeAlphaFlagBitsKHR result = VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR;
				for (auto& flag : flagInSortedByPriority) {
					if (flag & supportedFlag) {
						result = flag;
						break;
					}
				}
				return result;
			}
		};
	}
}
