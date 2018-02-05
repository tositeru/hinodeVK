#include "HVKDeviceMemory.h"

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKDeviceMemory::HVKDeviceMemory()
			: mMemory(nullptr)
			, mParentDevice(nullptr)
		{ }

		HVKDeviceMemory::HVKDeviceMemory(HVKDeviceMemory&& right)noexcept
			: mMemory(right.mMemory)
			, mParentDevice(right.mParentDevice)
		{
			right.mMemory = nullptr;
			right.mParentDevice = nullptr;
		}

		HVKDeviceMemory& HVKDeviceMemory::operator=(HVKDeviceMemory&& right)noexcept
		{
			this->mMemory = right.mMemory;
			this->mParentDevice = right.mParentDevice;

			right.mMemory = nullptr;
			right.mParentDevice = nullptr;
			return *this;
		}

		HVKDeviceMemory::~HVKDeviceMemory()
		{
			this->release();
		}


		void HVKDeviceMemory::release()noexcept
		{
			if (nullptr != this->mMemory) {
				vkFreeMemory(this->mParentDevice, this->mMemory, this->allocationCallbacksPointer());
				this->mParentDevice = nullptr;
				this->mMemory = nullptr;
			}
		}

		void HVKDeviceMemory::create(VkDevice device, VkMemoryAllocateInfo* pInfo)
		{
			auto ret = vkAllocateMemory(device, pInfo, this->allocationCallbacksPointer(), &this->mMemory);
			if (ret != VK_SUCCESS) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKDeviceMemory, create, ret) << "ƒƒ‚ƒŠŠm•Û‚ÉŽ¸”s";
			}
			this->mParentDevice = device;
		}

		VkResult HVKDeviceMemory::bindImage(VkImage image, VkDeviceSize offset)
		{
			assert(this->isGood());

			return vkBindImageMemory(this->mParentDevice, image, this->mMemory, offset);
		}

		VkResult HVKDeviceMemory::bindBuffer(VkBuffer buffer, VkDeviceSize offset)
		{
			assert(this->isGood());
			return vkBindBufferMemory(this->mParentDevice, buffer, this->mMemory, offset);
		}

		VkResult HVKDeviceMemory::map(void** ppOutPointer, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags)
		{
			assert(this->isGood());
			return vkMapMemory(this->mParentDevice, this->mMemory, offset, size, flags, ppOutPointer);
		}

		void HVKDeviceMemory::unmap()
		{
			assert(this->isGood());
			vkUnmapMemory(this->mParentDevice, this->mMemory);
		}

		bool HVKDeviceMemory::isGood()const noexcept
		{
			return this->mMemory != nullptr && this->mParentDevice != nullptr;
		}

		VkDeviceMemory HVKDeviceMemory::memory()noexcept
		{
			return this->mMemory;
		}

	}

	namespace graphics
	{
		HVKMemoryAllocateInfo::HVKMemoryAllocateInfo()
			: HVKMemoryAllocateInfo(0, 0)
		{ }

		HVKMemoryAllocateInfo::HVKMemoryAllocateInfo(VkDeviceSize allocationSize, uint32_t memoryTypeIndex)
		{
			this->allocationSize = allocationSize;
			this->memoryTypeIndex = memoryTypeIndex;

			// ˆÈ‰ºAŒÅ’è
			this->sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			this->pNext = NULL;
		}

		uint32_t HVKMemoryAllocateInfo::sCheckMemmoryType(VkPhysicalDeviceMemoryProperties props, uint32_t typeBits, VkFlags requirementsMask)noexcept
		{
			uint32_t result = static_cast<uint32_t >(-1);
			// Search memtypes to find first index with those properties
			for (uint32_t i = 0; i < props.memoryTypeCount; i++) {
				if ((typeBits & 1) == 1) {
					// Type is available, does it match user properties?
					if ((props.memoryTypes[i].propertyFlags & requirementsMask) == requirementsMask) {
						result = i;
						break;
					}
				}
				typeBits >>= 1;
			}
			// No memory types matched, return failure
			return result;
		}
	}
}