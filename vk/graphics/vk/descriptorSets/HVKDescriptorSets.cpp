#include "HVKDescriptorSets.h"

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKDescriptorSets::HVKDescriptorSets()
			: mParentDevice(VK_NULL_HANDLE)
			, mParentPool(VK_NULL_HANDLE)
			, mEnableFree(false)
		{}

		HVKDescriptorSets::HVKDescriptorSets(HVKDescriptorSets&& right)noexcept
			: mSets(std::move(right.mSets))
			, mParentDevice(right.mParentDevice)
			, mParentPool(right.mParentPool)
			, mEnableFree(right.mEnableFree)
		{
			right.mSets.clear();
			right.mSets.shrink_to_fit();
			right.mParentDevice = VK_NULL_HANDLE;
			right.mParentPool = VK_NULL_HANDLE;
			right.mEnableFree = false;
		}

		HVKDescriptorSets& HVKDescriptorSets::operator=(HVKDescriptorSets&& right)noexcept
		{
			this->mSets = std::move(right.mSets);
			this->mParentDevice = right.mParentDevice;
			this->mParentPool = right.mParentPool;
			this->mEnableFree = right.mEnableFree;

			right.mSets.clear();
			right.mSets.shrink_to_fit();
			right.mParentDevice = VK_NULL_HANDLE;
			right.mParentPool = VK_NULL_HANDLE;
			right.mEnableFree = false;

			return *this;
		}

		HVKDescriptorSets::~HVKDescriptorSets()
		{
			this->release();
		}

		void HVKDescriptorSets::release()noexcept
		{
			if (this->isGood()) {
				if (this->mEnableFree) {
					vkFreeDescriptorSets(this->mParentDevice, this->mParentPool, static_cast<uint32_t>(this->mSets.size()), this->mSets.data());
				}
				this->mSets.clear();
				this->mSets.shrink_to_fit();
				this->mParentDevice = VK_NULL_HANDLE;
				this->mParentPool = VK_NULL_HANDLE;
				this->mEnableFree = false;
			}
		}

		void HVKDescriptorSets::create(VkDevice device, VkDescriptorPool pool, bool enableFreeDescriptorSet, VkDescriptorSetAllocateInfo* pInfo)
		{
			this->release();

			pInfo->descriptorPool = pool;
			this->mSets.resize(pInfo->descriptorSetCount);
			auto ret = vkAllocateDescriptorSets(device, pInfo, this->mSets.data());
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKDescriptorSets, create, ret) << "ì¬‚ÉŽ¸”s";
			}

			this->mParentDevice = device;
			this->mParentPool = pool;
			this->mEnableFree = enableFreeDescriptorSet;
		}

		void HVKDescriptorSets::update(int index, VkWriteDescriptorSet* pWriteInfo, uint32_t writeCount, VkCopyDescriptorSet* pCopyInfo, uint32_t copyCount)
		{
			assert(this->isGood());

			auto target = this->get(index);
			for (uint32_t i = 0; i < writeCount; ++i) {
				pWriteInfo[i].dstSet = target;
			}
			for (uint32_t i = 0; i < copyCount; ++i) {
				pCopyInfo[i].dstSet = target;
			}
			vkUpdateDescriptorSets(this->mParentDevice, writeCount, pWriteInfo, copyCount, pCopyInfo);
		}

		bool HVKDescriptorSets::isGood()const noexcept
		{
			return this->mParentDevice != VK_NULL_HANDLE && this->mParentPool != VK_NULL_HANDLE && !this->mSets.empty();
		}

		VkDescriptorSet& HVKDescriptorSets::get(int index)
		{
			assert(this->isGood());
			return this->mSets[index];
		}

		std::vector<VkDescriptorSet>& HVKDescriptorSets::descriptorSets()noexcept
		{
			assert(this->isGood());
			return this->mSets;
		}
	}

	namespace graphics
	{
		HVKDescriptorSetAllocateInfo::HVKDescriptorSetAllocateInfo()noexcept
			: HVKDescriptorSetAllocateInfo(VK_NULL_HANDLE, 0, nullptr)
		{}

		HVKDescriptorSetAllocateInfo::HVKDescriptorSetAllocateInfo(VkDescriptorPool pool, uint32_t descSetCount, const VkDescriptorSetLayout* pLayouts)noexcept
		{
			this->descriptorPool = pool;
			this->descriptorSetCount = descSetCount;
			this->pSetLayouts = pLayouts;

			// ˆÈ‰ºŒÅ’è
			this->sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			this->pNext = NULL;
		}
	}

	namespace graphics
	{
		HVKWriteDescriptorSet::HVKWriteDescriptorSet()
		{
			this->dstSet = VK_NULL_HANDLE;
			this->descriptorCount = 0;
			this->descriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;
			this->pBufferInfo = nullptr;
			this->pImageInfo = nullptr;
			this->pTexelBufferView = nullptr;
			this->dstArrayElement = 0;
			this->dstBinding = 0;

			//ˆÈ‰ºŒÅ’è
			this->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			this->pNext = NULL;
		}

		HVKWriteDescriptorSet& HVKWriteDescriptorSet::setDest(uint32_t destBinding, uint32_t arrayElement)noexcept
		{
			this->dstBinding = destBinding;
			this->dstArrayElement = arrayElement;
			return *this;
		}

		HVKWriteDescriptorSet& HVKWriteDescriptorSet::setImageInfo(VkDescriptorType type, const VkDescriptorImageInfo* pImageInfos, uint32_t infoCount)noexcept
		{
			this->descriptorType = type;
			this->pImageInfo = pImageInfos;
			this->descriptorCount = infoCount;
			return *this;
		}

		HVKWriteDescriptorSet& HVKWriteDescriptorSet::setBufferInfo(VkDescriptorType type, const VkDescriptorBufferInfo* pBufferInfos, uint32_t infoCount)noexcept
		{
			this->descriptorType = type;
			this->pBufferInfo = pBufferInfos;
			this->descriptorCount = infoCount;
			return *this;
		}

		HVKWriteDescriptorSet& HVKWriteDescriptorSet::setTexelBufferView(VkDescriptorType type, const VkBufferView* pTexelBufferViews, uint32_t infoCount)noexcept
		{
			this->descriptorType = type;
			this->pTexelBufferView = pTexelBufferViews;
			this->descriptorCount = infoCount;
			return *this;
		}
	}

	namespace graphics
	{
		HVKCopyDescriptorSet::HVKCopyDescriptorSet()
		{
			this->descriptorCount = 0;
			this->dstSet = VK_NULL_HANDLE;
			this->dstBinding = 0;
			this->dstArrayElement = 0;
			this->srcSet = VK_NULL_HANDLE;
			this->srcBinding = 0;
			this->srcArrayElement = 0;

			//ˆÈ‰ºŒÅ’è
			this->sType = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET;
			this->pNext = nullptr;
		}

		HVKCopyDescriptorSet& HVKCopyDescriptorSet::setDescCount(uint32_t count)noexcept
		{
			this->descriptorCount = count;
			return *this;
		}

		HVKCopyDescriptorSet& HVKCopyDescriptorSet::setDest(VkDescriptorSet desc, uint32_t binding, uint32_t arrayElement)noexcept
		{
			this->dstSet = desc;
			this->dstBinding = binding;
			this->dstArrayElement = arrayElement;
			return *this;
		}

		HVKCopyDescriptorSet& HVKCopyDescriptorSet::setSrc(VkDescriptorSet desc, uint32_t binding, uint32_t arrayElement)noexcept
		{
			this->srcSet = desc;
			this->srcBinding = binding;
			this->srcArrayElement = arrayElement;
			return *this;
		}
	}

}