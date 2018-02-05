#pragma once

#include <vulkan\vulkan.h>

#include "../HVKInterface.h"
#include "../allocationCallbacks/HVKAllocationCallbacks.h"
namespace hinode
{
	namespace graphics
	{
		class HVKDeviceMemory : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKDeviceMemory(const HVKDeviceMemory&) = delete;
			HVKDeviceMemory& operator=(const HVKDeviceMemory&) = delete;

		public:
			HVKDeviceMemory();
			HVKDeviceMemory(HVKDeviceMemory&& right)noexcept;
			HVKDeviceMemory& operator=(HVKDeviceMemory&& right)noexcept;
			~HVKDeviceMemory();

			void release()noexcept override;

			/// @brief 作成
			/// @param[in] device
			/// @param[in] pInfo HVKMemoryAllocateInfoを用意していますので、そちらを使うことを推奨します。
			/// @exception HVKExeption
			void create(VkDevice device, VkMemoryAllocateInfo* pInfo);

			VkResult bindImage(VkImage image, VkDeviceSize offset = 0);

		public:
			bool isGood()const noexcept override;
			VkDeviceMemory memory()noexcept;
			operator VkDeviceMemory()noexcept { return this->memory(); }

		private:
			VkDeviceMemory mMemory;
			VkDevice mParentDevice;
		};
	}

	namespace graphics
	{
		struct HVKMemoryAllocateInfo : public VkMemoryAllocateInfo
		{
			HVKMemoryAllocateInfo();
			HVKMemoryAllocateInfo(VkDeviceSize allocationSize, uint32_t memoryTypeIndex);

			/// @brief requirementsMaskのmemoryTypeIndexを調べる
			///
			/// 見つからなければ-1を返す
			/// @param[in] props
			/// @param[in] typeBits
			/// @param[in] requirementsMask チェックするVkMemoryPropertiesFlagBit
			/// @retval uint32_t
			static uint32_t sCheckMemmoryType(VkPhysicalDeviceMemoryProperties props, uint32_t typeBits, VkFlags requirementsMask)noexcept;
		};
	}
}