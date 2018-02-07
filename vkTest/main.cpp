#include <iostream>

#include <graphics\vk\KHR\DefineKHRPlatform.h>
#include <graphics\vk\common\Common.h>
#include <graphics\vk\instance\HVKInstance.h>
#include <graphics\vk\physicalDevice\HVKPhysicalDevice.h>
#include <graphics\vk\device\HVKDevice.h>
#include <graphics\vk\commandPool\HVKCommandPool.h>
#include <graphics\vk\commandBuffer\HVKCommandBuffer.h>
#include <graphics\vk\KHR\swapChain\HVKSwapChainKHR.h>
#include <graphics\vk\KHR\surface\HVKSurfaceKHR.h>
#include <graphics\vk\image\HVKImage.h>
#include <graphics\vk\deviceMemory\HVKDeviceMemory.h>
#include <graphics\vk\buffer\HVKBuffer.h>
#include <graphics\vk\pipelineLayout\HVKPipelineLayout.h>
#include <graphics\vk\descriptorSetLayout\HVKDescriptorSetLayout.h>
#include <graphics\vk\descriptorPool\HVKDescriptorPool.h>
#include <graphics\vk\descriptorSets\HVKDescriptorSets.h>

#include <graphics\vk\utility\math\SimpleMath.h>

#include "winapi\VKWindow\VKWindow.h"

using namespace std;

int main(int argc, char** args)
{
	using namespace hinode::graphics;
	using namespace hinode::math;

	Log::sStandbyLogFile();

	try {
		VKWindow window;
		{
			VKWindow::InitParam initParam(640, 480, L"hinodeVK");
			window.create(initParam);
		}

		HVKInstance instance;
		{
			HVKApplicationInfo appInfo;
			appInfo.setApp("hinodeVK", 0);
			appInfo.setEngine("hinodeVK", 0);
			HVKInstanceCreateInfo instanceCreateInfo;
			instanceCreateInfo.pApplicationInfo = &appInfo;

			instanceCreateInfo.layerNames.push_back("VK_LAYER_LUNARG_standard_validation");
			instanceCreateInfo.updateEnableLayerNames();

			std::vector<VkExtensionProperties> extProps = HVKInstance::sEnumerateExtensionProperties();
			instanceCreateInfo.setEnableExtensionInfo(extProps.data(), static_cast<uint32_t>(extProps.size()));
			instance.create(&instanceCreateInfo);
		}

		HVKSurfaceKHR surface;
		{
			VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
			surfaceCreateInfo.hwnd = window.HWnd();
			surfaceCreateInfo.hinstance = GetModuleHandleW(NULL);
			surface.create(instance, &surfaceCreateInfo);
		}

		std::vector<VkPhysicalDevice> gpus;
		if (!instance.enumeratePhysicalDevices(gpus, 1)) {
			cout << "GPU‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñB" << endl;
			return 0;
		}

		auto gpu = HVKPhysicalDevice(gpus[0]);
		//auto memoryProps = gpu.getMemoryProperties();
		//auto props = gpu.getProperties();

		HVKDeviceQueueCreateInfo queueInfo;
		{
			auto queueFamilyProps = gpu.getQueueFamilyProperties();

			std::vector<float> queuePriority(queueFamilyProps.size(), 0.f);
			queueInfo.setQueuePriorities(queuePriority);

			auto supportsPresent = surface.getSupportPresentOfAllQueueFamily(gpu);
			queueInfo.setQueueFamilyIndexAndPresentQueueIndex(queueFamilyProps, supportsPresent);
		}

		HVKDevice device;
		{
			HVKDeviceCreateInfo deviceCreateInfo(&queueInfo, 1);
			auto deviceProps = gpu.getDeviceExtensionProperties();
			std::vector<const char*> devicePropNames = {"VK_KHR_swapchain"};
			deviceCreateInfo.enabledExtensionCount = static_cast<decltype(deviceCreateInfo.enabledExtensionCount)>(devicePropNames.size());
			deviceCreateInfo.ppEnabledExtensionNames = devicePropNames.data();
			device.create(gpu, &deviceCreateInfo);
		}

		HVKCommandPool commandPool;
		{
			HVKCommandPoolCreateInfo poolInfo(queueInfo.queueFamilyIndex, 0);
			commandPool.create(device, &poolInfo);
		}

		HVKCommandBuffer commandBuffer;
		{
			HVKCommandBufferAllocateInfo commandBufferInfo(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
			commandBuffer.create(device, &commandBufferInfo);
		}

		VkExtent2D swapchainExtent;
		HVKSwapChainKHR swapchain;
		std::vector<HVKImage> images;
		{
			std::vector<VkSurfaceFormatKHR> surfaceFormats;
			VkSurfaceCapabilitiesKHR surfaceCapabilities;
			std::vector<VkPresentModeKHR> presentModes;
			HVKSwapchainCreateInfoKHR swapchainCreateInfo = HVKSwapchainCreateInfoKHR::sCreate(surfaceFormats, surfaceCapabilities, presentModes, gpu, surface);
			if (queueInfo.queueFamilyIndex != queueInfo.presentQueueFamilyIndex) {
				swapchainCreateInfo.setQueueFamilyIndices({ queueInfo.queueFamilyIndex, queueInfo.presentQueueFamilyIndex });
			}
			swapchain.create(device, &swapchainCreateInfo);

			std::vector<VkImage> rawImages = swapchain.getAllImages();
			images = HVKImage::sConvert(device, rawImages.data(), static_cast<uint32_t>(rawImages.size()));
			for (auto& image : images) {
				HVKImageViewCreateInfo viewInfo(VK_IMAGE_VIEW_TYPE_2D, swapchainCreateInfo.imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
				image.addView(&viewInfo);
			}
			rawImages.clear();

			swapchainExtent = swapchainCreateInfo.imageExtent;
		}

		auto deviceMemoryProps = gpu.getMemoryProperties();

		HVKImage depthBuffer;
		HVKDeviceMemory depthBufferMemory;
		{
			HVKImageCreateInfo imageInfo = HVKImageCreateInfo::sMake2D(VK_FORMAT_D16_UNORM, swapchainExtent.width, swapchainExtent.height);
			imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			auto formatProps = gpu.getFormatProperties(imageInfo.format);
			imageInfo.tiling = HVKImageCreateInfo::sCheckTiling(formatProps, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
			assert(imageInfo.tiling != VK_IMAGE_TILING_MAX_ENUM);
			depthBuffer.create(device, &imageInfo);

			auto memoryRequirements = depthBuffer.getMemoryRequirements();
			HVKMemoryAllocateInfo memAlloInfo;
			memAlloInfo.allocationSize = memoryRequirements.size;
			memAlloInfo.memoryTypeIndex = HVKMemoryAllocateInfo::sCheckMemmoryType(deviceMemoryProps, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			assert(memAlloInfo.memoryTypeIndex != -1);
			depthBufferMemory.create(device, &memAlloInfo);
			depthBufferMemory.bindImage(depthBuffer);

			HVKImageViewCreateInfo viewInfo(VK_IMAGE_VIEW_TYPE_2D, imageInfo.format, VK_IMAGE_ASPECT_DEPTH_BIT);
			depthBuffer.addView(&viewInfo);
		}

		const VkDeviceSize uniformBufferSize = sizeof(float4x4);
		HVKBuffer uniformBuf;
		HVKDeviceMemory uniformBufMemory;
		{
			HVKBufferCreateInfo bufInfo(sizeof(float4x4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			uniformBuf.create(device, &bufInfo);

			auto memoryRequirements = uniformBuf.getMemoryRequirements();
			HVKMemoryAllocateInfo memAllocInfo;
			memAllocInfo.allocationSize = memoryRequirements.size;
			memAllocInfo.memoryTypeIndex = HVKMemoryAllocateInfo::sCheckMemmoryType(deviceMemoryProps, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			assert(memAllocInfo.memoryTypeIndex != -1);
			uniformBufMemory.create(device, &memAllocInfo);

			uint8_t* pData;
			auto ret = uniformBufMemory.map((void**)&pData, 0, memoryRequirements.size, 0);
			assert(VK_SUCCESS == ret);

			//memcpy(pData, matrix, sizeof(matrix));

			uniformBufMemory.unmap();

			uniformBufMemory.bindBuffer(uniformBuf);
		}

		HVKDescriptorSetLayout descSetLayout;
		HVKPipelineLayout pipelineLayout;
		{
			HVKDescriptorSetLayoutBinding layoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);

			HVKDescriptorSetLayoutCreateInfo descSetLayoutInfo(&layoutBinding, 1);
			descSetLayout.create(device, &descSetLayoutInfo);

			VkDescriptorSetLayout pLayouts[] = {
				descSetLayout,
			};
			HVKPipelineLayoutCreateInfo pipelineLayoutInfo(pLayouts, 1);
			pipelineLayout.create(device, &pipelineLayoutInfo);
		}

		HVKDescriptorPool descPool;
		HVKDescriptorSets descSets;
		{
			VkDescriptorPoolSize pool[] = {
				HVKDescriptorPoolCreateInfo::sMakePoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
			};
			HVKDescriptorPoolCreateInfo poolInfo(pool, 1, 1);
			descPool.create(device, &poolInfo);

			VkDescriptorSetLayout pLayout[] = {
				descSetLayout.descriptorSetLayout()
			};
			HVKDescriptorSetAllocateInfo descSetInfo(descPool, 1, pLayout);
			descSets.create(device, descPool, descPool.enableFreeDescriptorSet(), &descSetInfo);

			VkDescriptorBufferInfo bufferDescptor;
			bufferDescptor.buffer = uniformBuf;
			bufferDescptor.offset = 0;
			bufferDescptor.range = uniformBufferSize;
			auto  write = HVKWriteDescriptorSet()
				.setDest(0, 0)
				.setBufferInfo(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &bufferDescptor, 1);
			descSets.update(0, &write, 1, nullptr, 0);
		}
		images.clear();

		window.mainLoop();

	} catch (HVKException& e) {
		e.writeLog();
	}

	return 0;
}