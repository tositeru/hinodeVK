#include "HVKPhysicalDevice.h"

#include <cassert>

namespace hinode
{
	namespace graphics
	{
		HVKPhysicalDevice::HVKPhysicalDevice()
		{ }

		HVKPhysicalDevice::HVKPhysicalDevice(VkPhysicalDevice device)
			: mDevice(device)
		{ }

		void HVKPhysicalDevice::release()noexcept
		{
			this->mDevice = nullptr;
		}

		void HVKPhysicalDevice::set(VkPhysicalDevice device)
		{
			this->mDevice = device;
		}

		VkPhysicalDeviceFeatures HVKPhysicalDevice::getFeatures()noexcept
		{
			assert(this->isGood());
			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(this->mDevice, &features);
			return features;
		}

		std::vector<VkLayerProperties> HVKPhysicalDevice::enumerateDeviceLayerProperties()noexcept
		{
			assert(this->isGood());

			uint32_t count = 0;
			auto result = vkEnumerateDeviceLayerProperties(*this, &count, nullptr);
			if (result != VK_SUCCESS) {
				return {};
			}
			std::vector<VkLayerProperties> layers(count);
			result = vkEnumerateDeviceLayerProperties(*this, &count, layers.data());
			if (result != VK_SUCCESS) {
				return {};
			}
			return layers;
		}

		std::vector<VkExtensionProperties> HVKPhysicalDevice::enumerateDeviceExtensionProperties(const char* layerName)noexcept
		{
			assert(this->isGood());

			uint32_t count = 0;
			auto result = vkEnumerateDeviceExtensionProperties(*this, layerName, &count, nullptr);
			if (result != VK_SUCCESS) {
				return {};
			}
			std::vector<VkExtensionProperties> props(count);
			result = vkEnumerateDeviceExtensionProperties(*this, layerName, &count, props.data());
			if (result != VK_SUCCESS) {
				return {};
			}
			return props;
		}

		VkFormatProperties HVKPhysicalDevice::getFormatProperties(VkFormat format)noexcept
		{
			assert(this->isGood());

			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(*this, format, &props);
			return props;
		}

		VkResult HVKPhysicalDevice::getImageFormatProperties(VkImageFormatProperties& out, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags)noexcept
		{
			assert(this->isGood());

			return vkGetPhysicalDeviceImageFormatProperties(*this, format, type, tiling, usage, flags, &out);
		}

		VkPhysicalDeviceMemoryProperties HVKPhysicalDevice::getMemoryProperties()noexcept
		{
			assert(this->isGood());

			VkPhysicalDeviceMemoryProperties props;
			vkGetPhysicalDeviceMemoryProperties(*this, &props);
			return props;
		}

		VkPhysicalDeviceProperties HVKPhysicalDevice::getProperties()noexcept
		{
			assert(this->isGood());

			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(*this, &props);
			return props;
		}

		std::vector<VkQueueFamilyProperties> HVKPhysicalDevice::getQueueFamilyProperties()noexcept
		{
			assert(this->isGood());

			uint32_t count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(*this, &count, nullptr);
			std::vector<VkQueueFamilyProperties> props(count);
			vkGetPhysicalDeviceQueueFamilyProperties(*this, &count, props.data());
			return props;
		}

		std::vector<VkSparseImageFormatProperties> HVKPhysicalDevice::getSparseImageFormatProperties(VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling)noexcept
		{
			assert(this->isGood());

			uint32_t count = 0;
			vkGetPhysicalDeviceSparseImageFormatProperties(*this, format, type, samples, usage, tiling, &count, nullptr);
			std::vector<VkSparseImageFormatProperties> props(count);
			vkGetPhysicalDeviceSparseImageFormatProperties(*this, format, type, samples, usage, tiling, &count, props.data());
			return props;
		}

		bool HVKPhysicalDevice::isGood()const noexcept
		{
			return nullptr != this->mDevice;
		}

		VkPhysicalDevice HVKPhysicalDevice::device()noexcept
		{
			return this->mDevice;
		}
	}
}