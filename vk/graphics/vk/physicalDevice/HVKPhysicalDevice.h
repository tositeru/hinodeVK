#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <vulkan\vulkan.h>

#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKPhysicalDevice : public IHVKInterface
		{
		public:
			HVKPhysicalDevice();
			HVKPhysicalDevice(VkPhysicalDevice device);

			void release()noexcept override;

			void set(VkPhysicalDevice device);

			VkPhysicalDeviceFeatures getFeatures()noexcept;

			std::vector<VkLayerProperties> enumerateDeviceLayerProperties()noexcept;

			std::vector<VkExtensionProperties> enumerateDeviceExtensionProperties(const char* layerName)noexcept;

			VkFormatProperties getFormatProperties(VkFormat format)noexcept;
			VkResult getImageFormatProperties(VkImageFormatProperties& out, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags)noexcept;
			VkPhysicalDeviceMemoryProperties getMemoryProperties()noexcept;
			VkPhysicalDeviceProperties getProperties()noexcept;

			std::vector<VkQueueFamilyProperties> getQueueFamilyProperties()noexcept;

			std::vector<VkSparseImageFormatProperties> getSparseImageFormatProperties(VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling)noexcept;

			std::unordered_map<std::string, VkExtensionProperties> getDeviceExtensionProperties(const char* layerName = nullptr);

		public:
			bool isGood()const noexcept override;

			VkPhysicalDevice device()noexcept;
			operator VkPhysicalDevice()noexcept { return this->device(); }

		private:
			VkPhysicalDevice mDevice;
		};
	}
}