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

		std::vector<VkExtensionProperties> extProps = HVKInstance::sEnumerateExtensionProperties();
		instanceCreateInfo.setEnableExtensionInfo(extProps.data(), static_cast<uint32_t>(extProps.size()));
		HVKInstance instance;
		instance.create(&instanceCreateInfo);

		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.hwnd = window.HWnd();
		surfaceCreateInfo.hinstance = GetModuleHandleW(NULL);
		HVKSurfaceKHR surface;
		surface.create(instance, &surfaceCreateInfo);

		std::vector<VkPhysicalDevice> gpus;
		if (!instance.enumeratePhysicalDevices(gpus, 1)) {
			cout << "GPUが見つかりません。" << endl;
			return 0;
		}
		auto gpu = HVKPhysicalDevice(gpus[0]);
		auto memoryProps = gpu.getMemoryProperties();
		auto props = gpu.getProperties();

		HVKDeviceQueueCreateInfo queueInfo;
		auto queueFamilyProps = gpu.getQueueFamilyProperties();
		std::vector<VkBool32> supportsPresent(queueFamilyProps.size());
		for (uint32_t i = 0; i < queueFamilyProps.size(); i++) {
			vkGetPhysicalDeviceSurfaceSupportKHR(gpus[0], i, surface, &supportsPresent[i]);
		}
		queueInfo.queueFamilyIndex = UINT32_MAX;
		uint32_t presentQueueFamilyIndex = UINT32_MAX;
		for (auto i = 0u; i < queueFamilyProps.size(); ++i) {
			auto& prop = queueFamilyProps[i];
			if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				if (queueInfo.queueFamilyIndex) queueInfo.queueFamilyIndex = i;

				if (supportsPresent[i]) {
					queueInfo.queueFamilyIndex = i;
					presentQueueFamilyIndex = i;
					break;
				}
			}
		}
		if (presentQueueFamilyIndex == UINT32_MAX) {
			for (auto i = 0u; i < supportsPresent.size(); ++i) {
				if (supportsPresent[i]) {
					presentQueueFamilyIndex = i;
					break;
				}
			}
		}

		HVKDeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.pQueueCreateInfos = &queueInfo;
		deviceCreateInfo.queueCreateInfoCount = 1;
		HVKDevice device;
		device.create(gpu, &deviceCreateInfo);

		HVKCommandPoolCreateInfo poolInfo(queueInfo.queueFamilyIndex, 0);
		HVKCommandPool commandPool;
		commandPool.create(device, &poolInfo);

		HVKCommandBufferAllocateInfo commandBufferInfo(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		HVKCommandBuffer commandBuffer;
		commandBuffer.create(device, &commandBufferInfo);

		HVKSwapchainCreateInfoKHR swapchainCreateInfo(surface, VK_FORMAT_B8G8R8A8_UNORM, 640, 480, 2);
		uint32_t queueFamilyIndices[2] = { (uint32_t)queueInfo.queueFamilyIndex, (uint32_t)presentQueueFamilyIndex };
		if (queueInfo.queueFamilyIndex != presentQueueFamilyIndex) {
			//GraphicsとPresentキューのファミリーが異なるケースがあるため、その時は以下のように設定する
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		HVKSwapChainKHR swapchain;
		swapchain.create(device, &swapchainCreateInfo);

		std::vector<VkImage> rawImages = swapchain.getAllImages();
		auto images = HVKImage::sConvert(device, rawImages.data(), rawImages.size());

	} catch (HVKException& e) {
		e.writeLog();
	}

	return 0;
}