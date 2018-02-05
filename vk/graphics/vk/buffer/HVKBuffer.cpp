#include "HVKBuffer.h"

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{

		HVKBuffer::HVKBuffer()
			: mBuffer(nullptr)
			, mParentDevice(nullptr)
		{ }

		HVKBuffer::HVKBuffer(HVKBuffer&& right)noexcept
			: mBuffer(right.mBuffer)
			, mParentDevice(right.mParentDevice)
			, mViews(std::move(right.mViews))
		{
			right.mBuffer = nullptr;
			right.mParentDevice = nullptr;
			right.mViews.clear();
			right.mViews.shrink_to_fit();
		}

		HVKBuffer& HVKBuffer::operator=(HVKBuffer&& right)noexcept
		{
			this->mBuffer = right.mBuffer;
			this->mParentDevice = right.mParentDevice;
			this->mViews = std::move(right.mViews);

			right.mBuffer = nullptr;
			right.mParentDevice = nullptr;
			right.mViews.clear();
			right.mViews.shrink_to_fit();

			return *this;
		}

		HVKBuffer::~HVKBuffer()
		{
			this->release();
		}

		void HVKBuffer::release()noexcept
		{
			if (nullptr != this->mBuffer) {
				for (auto& view : this->mViews) {
					vkDestroyBufferView(this->mParentDevice, view, this->allocationCallbacksPointer());
				}
				this->mViews.clear();
				this->mViews.shrink_to_fit();

				vkDestroyBuffer(this->mParentDevice, this->mBuffer, this->allocationCallbacksPointer());
				this->mParentDevice = nullptr;
				this->mBuffer = nullptr;
			}
		}

		void HVKBuffer::create(VkDevice device, VkBufferCreateInfo* pInfo)
		{
			this->release();

			auto ret = vkCreateBuffer(device, pInfo, this->allocationCallbacksPointer(), &this->mBuffer);
			if (ret != VK_SUCCESS) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKBuffer, create, ret) << "ì¬Ž¸”s";
			}
			this->mParentDevice = device;
		}

		size_t HVKBuffer::addView(VkBufferViewCreateInfo* pInfo)
		{
			assert(this->isGood());

			pInfo->buffer = this->mBuffer;
			VkBufferView view;
			auto ret = vkCreateBufferView(this->mParentDevice, pInfo, this->allocationCallbacksPointer(), &view);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKBuffer, addView, ret) << "ƒrƒ…[‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½";
			}
			this->mViews.push_back(view);
			return this->mViews.size() - 1;
		}

		VkMemoryRequirements HVKBuffer::getMemoryRequirements()
		{
			assert(this->isGood());
			VkMemoryRequirements result;
			vkGetBufferMemoryRequirements(this->mParentDevice, this->mBuffer, &result);
			return result;
		}

		bool HVKBuffer::isGood()const noexcept
		{
			return this->mBuffer != nullptr && this->mParentDevice != nullptr;
		}

		VkBuffer HVKBuffer::buffer()noexcept
		{
			assert(this->isGood());
			return this->mBuffer;
		}

		VkBufferView HVKBuffer::view(int index)
		{
			assert(this->isGood());
			return this->mViews[index];
		}
	}

	namespace graphics
	{
		HVKBufferCreateInfo::HVKBufferCreateInfo()noexcept
			: HVKBufferCreateInfo(0, VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM)
		{ }

		HVKBufferCreateInfo::HVKBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage)noexcept
		{
			this->usage = usage;
			this->size = size;

			this->queueFamilyIndexCount = 0;
			this->pQueueFamilyIndices = NULL;
			this->sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			this->flags = 0;

			//ˆÈ‰ºŒÅ’è
			this->sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			this->pNext = NULL;
		}
	}
}
