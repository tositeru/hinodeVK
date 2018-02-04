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

#include "winapi\VKWindow\VKWindow.h"

using namespace std;

int main(int argc, char** args)
{
	using namespace hinode::graphics;

	Log::sStandbyLogFile();

	try {
		VKWindow window;
		{
			VKWindow::InitParam initParam(640, 480, L"hinodeVK");
			window.create(initParam);
		}

		HVKApplicationInfo appInfo;
		appInfo.setApp("hinodeVK", 0);
		appInfo.setEngine("hinodeVK", 0);
		HVKInstanceCreateInfo instanceCreateInfo;
		instanceCreateInfo.pApplicationInfo = &appInfo;

		std::vector<const char*> enabledInstLayers = { "VK_LAYER_LUNARG_standard_validation" };
		instanceCreateInfo.setEnableLayerNames(enabledInstLayers.data(), static_cast<uint32_t>(enabledInstLayers.size()));

		HVKInstance instance;
		{
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
				HVKImageViewCreateInfo viewInfo(VK_IMAGE_VIEW_TYPE_2D, swapchainCreateInfo.imageFormat);
				image.addView(&viewInfo);
			}
			rawImages.clear();
		}

		images.clear();

		window.mainLoop();

	} catch (HVKException& e) {
		e.writeLog();
	}

	return 0;
}