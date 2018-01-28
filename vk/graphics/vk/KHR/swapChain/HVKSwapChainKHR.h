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
			/// @brief vkCreateSharedSwapchainsKHR���g�p���ăX���b�v�`�F�C�����쐬����
			/// �쐬�Ɏ��s�����Ƃ��͗�O�𓊂��܂�
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

			/// @brief �X���b�v�`�F�C�����쐬���܂�
			/// @param[in] device
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkDevice device, VkSwapchainCreateInfoKHR* pCreateInfo);

			/// @brief �X���b�v�`�F�C���̃o�b�t�@���擾����
			/// pImages��nullptr�̏ꍇ��pInOutCount�Ƀo�b�t�@�̐����ݒ肳��܂�
			/// @param[inout] pInOutCount
			/// @param[out] pImages 
			/// @retval VkResult
			VkResult getSwapchainImages(uint32_t* pInOutCount, VkImage* pImages);

			/// @brief �X���b�v�`�F�C���ɂ��邷�ׂẴo�b�t�@���擾����
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
