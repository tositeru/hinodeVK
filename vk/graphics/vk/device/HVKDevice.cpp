#include "HVKDevice.h"

#include <utility> // for std::move

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKDevice::HVKDevice()
			: mDevice(nullptr)
		{ }

		HVKDevice::HVKDevice(HVKDevice&& right)noexcept
		{
			*this = std::move(right);
		}

		HVKDevice& HVKDevice::operator=(HVKDevice&& right)noexcept
		{
			this->release();

			this->mDevice = right.mDevice;

			right.mDevice = nullptr;
			return *this;
		}

		HVKDevice::~HVKDevice()
		{
			this->release();
		}

		void HVKDevice::release()noexcept
		{
			if (this->mDevice) {
				vkDeviceWaitIdle(this->mDevice);
				vkDestroyDevice(this->mDevice, this->allocationCallbacksPointer());
			}
		}

		void HVKDevice::create(VkPhysicalDevice physicalDevice, VkDeviceCreateInfo* pInfo)
		{
			this->release();

			VkResult result = vkCreateDevice(physicalDevice, pInfo, this->allocationCallbacksPointer(), &this->mDevice);
			if (VK_SUCCESS != result) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKDevice, create, result)
					<< "�f�o�C�X�̍쐬�Ɏ��s";
			}
		}

		VkResult HVKDevice::waitIdle()noexcept
		{
			assert(this->isGood());
			return vkDeviceWaitIdle(this->mDevice);
		}

		bool HVKDevice::isGood()const noexcept
		{
			return nullptr != this->mDevice;
		}

		VkDevice HVKDevice::device()noexcept
		{
			return this->mDevice;
		}
	}

	namespace graphics
	{
		HVKDeviceCreateInfo::HVKDeviceCreateInfo()
			: HVKDeviceCreateInfo(nullptr, 0)
		{ }

		HVKDeviceCreateInfo::HVKDeviceCreateInfo(VkDeviceQueueCreateInfo* pQueueInfos, uint32_t queueInfoCount)
		{
			this->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			this->pNext = nullptr;
			this->flags = 0;

			this->queueCreateInfoCount = queueInfoCount;
			this->pQueueCreateInfos = pQueueInfos;
			this->pEnabledFeatures = nullptr;

			this->enabledExtensionCount = 0;
			this->ppEnabledExtensionNames = nullptr;

			//�ȉ��@���C���[�͕����f�o�C�X�쐬���ɐݒ肷�邱�Ƃ𐄏����Ă���
			this->enabledLayerCount = 0;
			this->ppEnabledLayerNames = nullptr;
		}
	}

	namespace graphics
	{
		HVKDeviceQueueCreateInfo::HVKDeviceQueueCreateInfo()
			: HVKDeviceQueueCreateInfo(0, 1)
		{}

		HVKDeviceQueueCreateInfo::HVKDeviceQueueCreateInfo(uint32_t familyIndex, uint32_t count)
		{
			this->queueCount = count;
			this->queueFamilyIndex = familyIndex;
			this->pQueuePriorities = nullptr;

			//�Œ�l
			this->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			this->pNext = nullptr;
			this->flags = 0;
		}

		HVKDeviceQueueCreateInfo& HVKDeviceQueueCreateInfo::setQueuePriorities(const std::vector<float>& priorities)
		{
			this->queuePriorities = priorities;
			this->pQueuePriorities = this->queuePriorities.data();
			this->queueCount = static_cast<decltype(queueCount)>(this->queuePriorities.size());
			return *this;
		}
	}
}