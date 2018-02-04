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
			/// @brief vkCreateSharedSwapchainsKHR���g�p���ăX���b�v�`�F�C�����쐬����
			/// �쐬�Ɏ��s�����Ƃ��͗�O�𓊂��܂�
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
		public:
			/// @brief VkSwapChainKHR�̍쐬�ɂ͂�������������������̂ŁAVkSwapchainCreateInfoKHR��ݒ肷��ɂ������ĕK�v�ƂȂ�f�[�^���ꏏ�ɍ쐬����悤�������邽�߂ɍ쐬
			/// @param[out] outSurfaceFormats
			/// @param[out] outSurfaceCapabilities
			/// @param[out] outPresentModes
			/// @param[in] gpu
			/// @param[in] surface
			/// @retval HVKSwapchainCreateInfoKHR
			/// @exception HVKException
			static HVKSwapchainCreateInfoKHR sCreate(std::vector<VkSurfaceFormatKHR>& outSurfaceFormats, VkSurfaceCapabilitiesKHR& outSurfaceCapabilities, std::vector<VkPresentModeKHR>& outPresentModes, VkPhysicalDevice gpu, VkSurfaceKHR surface);

		private:
			/// @brief VkSwapChainKHR�̍쐬�ɂ͂�������������������̂ŁA���̍\���̂��쐬����Ƃ��ɕK�v�ƂȂ���̂���邱�Ƃ����v���邽��private�ɂ��Ă܂�
			HVKSwapchainCreateInfoKHR()noexcept;

		public:
			std::vector<uint32_t> queueFamilyIndices;

			HVKSwapchainCreateInfoKHR& setFormat(const VkSurfaceFormatKHR& surfaceFormat);

			/// @brief pQueueFamilyIndices, queueFamilyIndexCount��ݒ肷��֐�
			/// imageSharingMode��VK_SHARING_MODE_CONCURRENT�ɐݒ肳��܂��B
			///
			/// Graphics��Present�L���[�̃t�@�~���[���قȂ�P�[�X�����邽�߁A���̎��ɂ����֐�
			/// @param[in] indices
			/// @retval HVKSwapchainCreateInfoKHR
			HVKSwapchainCreateInfoKHR& setQueueFamilyIndices(const std::vector<uint32_t>& indices);

			/// @brief surfaceFormats����targetFormat�������̂�T���֐�
			///
			/// ������Ȃ��Ƃ���format = VK_FORMAT_UNDEFINED, colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR��ݒ肵��VkSurfaceFormatKHR��Ԃ��܂�
			/// @param[in] surfaceFormats
			/// @param[in] targetFormat
			/// @retval VkSurfaceFormatKHR
			static VkSurfaceFormatKHR sFindSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& surfaceFormats, VkFormat targetFormat);

			/// @brief sGetSupportedCompositeAlpha�֐��Ɏg�p���邱�Ƃ�z�肵�č쐬
			static const std::array<VkCompositeAlphaFlagBitsKHR, 4> sDefaultFlagsInSortedByPriority;

			/// @brief �T�|�[�g����Ă���compositeAlpha���擾����
			/// flagInSortedByPriority�ɑ��|�[�g����Ă�����̂��Ȃ����VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR��Ԃ��܂�
			/// @param[in] supportedFlag
			/// @param[in] flagInSortedByPriority �ݒ�ΏۂƂȂ�t���O�B�Y�������Ⴂ���̂قǗD�揇�ʂ������Ȃ�܂�
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
