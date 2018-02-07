#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#include "../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKDescriptorSets : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKDescriptorSets(HVKDescriptorSets& right)noexcept;
			HVKDescriptorSets& operator=(HVKDescriptorSets& right)noexcept;
		public:
			HVKDescriptorSets();
			HVKDescriptorSets(HVKDescriptorSets&& right)noexcept;
			HVKDescriptorSets& operator=(HVKDescriptorSets&& right)noexcept;
			~HVKDescriptorSets();

			void release()noexcept override;

			/// @brief çÏê¨
			/// @param[in] device
			/// @param[in] pool
			/// @param[in] enableFreeDescriptorSet
			/// @param[in] pInfo
			/// @exception HVKException
			void create(VkDevice device, VkDescriptorPool pool, bool enableFreeDescriptorSet, VkDescriptorSetAllocateInfo* pInfo);

			/// @brief
			/// @param[in] index ê›íËëŒè€ÇÃìYÇ¶éö
			/// @param[in] pWriteInfo
			/// @param[in] writeCount
			/// @param[in] pCopyInfo
			/// @param[in] copyCount
			void update(int index, VkWriteDescriptorSet* pWriteInfo, uint32_t writeCount, VkCopyDescriptorSet* pCopyInfo, uint32_t copyCount);

		public:
			bool isGood()const noexcept;
			VkDescriptorSet& get(int index);
			std::vector<VkDescriptorSet>& descriptorSets()noexcept;

		private:
			std::vector<VkDescriptorSet> mSets;
			VkDevice mParentDevice;
			VkDescriptorPool mParentPool;
			bool mEnableFree;

		};
	}

	namespace graphics
	{
		struct HVKDescriptorSetAllocateInfo : public VkDescriptorSetAllocateInfo
		{
			HVKDescriptorSetAllocateInfo()noexcept;
			HVKDescriptorSetAllocateInfo(VkDescriptorPool pool, uint32_t descSetCount, const VkDescriptorSetLayout* pLayouts)noexcept;
		};
	}

	namespace graphics
	{
		struct HVKWriteDescriptorSet : public VkWriteDescriptorSet
		{
			HVKWriteDescriptorSet();

			HVKWriteDescriptorSet& setDest(uint32_t destBinding, uint32_t arrayElement)noexcept;
			HVKWriteDescriptorSet& setImageInfo(VkDescriptorType type, const VkDescriptorImageInfo* pImageInfos, uint32_t infoCount)noexcept;
			HVKWriteDescriptorSet& setBufferInfo(VkDescriptorType type, const VkDescriptorBufferInfo* pBufferInfos, uint32_t infoCount)noexcept;
			HVKWriteDescriptorSet& setTexelBufferView(VkDescriptorType type, const VkBufferView* pTexelBufferViews, uint32_t infoCount)noexcept;
		};
	}

	namespace graphics
	{
		struct HVKCopyDescriptorSet : public VkCopyDescriptorSet
		{
			HVKCopyDescriptorSet();

			HVKCopyDescriptorSet& setDescCount(uint32_t count)noexcept;
			HVKCopyDescriptorSet& setDest(VkDescriptorSet desc, uint32_t destBinding, uint32_t arrayElement)noexcept;
			HVKCopyDescriptorSet& setSrc(VkDescriptorSet desc, uint32_t destBinding, uint32_t arrayElement)noexcept;
		};
	}
}
