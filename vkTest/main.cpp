#include <iostream>

#include <graphics\vk\common\Common.h>
#include <graphics\vk\instance\HVKInstance.h>
#include <graphics\vk\physicalDevice\HVKPhysicalDevice.h>
#include <graphics\vk\device\HVKDevice.h>
#include <graphics\vk\commandPool\HVKCommandPool.h>
#include <graphics\vk\commandBuffer\HVKCommandBuffer.h>

using namespace std;

int main(int argc, char** args)
{
	using namespace hinode::graphics;

	Log::sStandbyLogFile();

	try {
		HVKApplicationInfo appInfo;
		appInfo.setApp("hinodeVK", 0);
		appInfo.setEngine("hinodeVK", 0);
		HVKInstanceCreateInfo instanceCreateInfo;
		HVKInstance instance;
		instance.create(&instanceCreateInfo);

		std::vector<VkPhysicalDevice> gpus;
		if (!instance.enumeratePhysicalDevices(gpus, 1)) {
			cout << "GPU‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñB" << endl;
			return 0;
		}
		auto gpu = HVKPhysicalDevice(gpus[0]);
		auto memoryProps = gpu.getMemoryProperties();
		auto props = gpu.getProperties();

		HVKDeviceQueueCreateInfo queueInfo;
		auto queueFamilyProps = gpu.getQueueFamilyProperties();
		for (auto i = 0u; i < queueFamilyProps.size(); ++i) {
			auto& prop = queueFamilyProps[i];
			if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				queueInfo.queueFamilyIndex = i;
				break;
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
	} catch (HVKException& e) {
		e.writeLog();
	}

	return 0;
}