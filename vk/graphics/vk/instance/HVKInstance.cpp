#include "HVKInstance.h"

#include <utility> //for std::move()

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		std::vector<VkExtensionProperties> HVKInstance::sEnumerateExtensionProperties(const char* layerName, uint32_t propCount)
		{
			if (propCount == 0) {
				vkEnumerateInstanceExtensionProperties(layerName, &propCount, nullptr);
			}
			std::vector<VkExtensionProperties> ret(propCount);
			vkEnumerateInstanceExtensionProperties(layerName, &propCount, ret.data());
			return ret;
		}

		HVKInstance::HVKInstance()
			: mInstance(nullptr)
		{ }

		HVKInstance::HVKInstance(HVKInstance&& right)noexcept
			: mInstance(right.mInstance)
		{
			right.mInstance = nullptr;
		}

		HVKInstance& HVKInstance::operator=(HVKInstance&& right)noexcept
		{
			this->release();

			this->mInstance = right.mInstance;

			right.mInstance = nullptr;
			return *this;
		}

		HVKInstance::~HVKInstance()
		{
			this->release();
		}

		void HVKInstance::release()noexcept
		{
			if (this->mInstance) {
				vkDestroyInstance(this->mInstance, this->allocationCallbacksPointer());
				this->mInstance = nullptr;
			}
		}

		void HVKInstance::create(VkInstanceCreateInfo* pInfo)
		{
			this->release();

			auto result = vkCreateInstance(pInfo, this->allocationCallbacksPointer(), &this->mInstance);
			if (VK_SUCCESS != result) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKInstance, create, result)
					<< "Vulkanインスタンスの作成に失敗";
			}
		}

		bool HVKInstance::enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& out, uint32_t count)noexcept
		{
			out.clear();
			auto result = vkEnumeratePhysicalDevices(*this, &count, nullptr);
			if (result != VK_SUCCESS) {
				return false;
			}

			out.resize(count);
			result = vkEnumeratePhysicalDevices(*this, &count, out.data());
			out.shrink_to_fit();
			return result == VK_SUCCESS;
		}

		PFN_vkVoidFunction HVKInstance::getProcAddr(const char* name)noexcept
		{
			assert(this->isGood());
			if (name == nullptr) {
				return nullptr;
			}
			return vkGetInstanceProcAddr(this->mInstance, name);
		}

		bool HVKInstance::isGood()const noexcept
		{
			return nullptr != this->mInstance;
		}

		VkInstance HVKInstance::instance()noexcept
		{
			assert(this->isGood());
			return this->mInstance;
		}
	}

	namespace graphics
	{
		HVKInstanceCreateInfo::HVKInstanceCreateInfo()noexcept
			: HVKInstanceCreateInfo(nullptr)
		{}

		HVKInstanceCreateInfo::HVKInstanceCreateInfo(VkApplicationInfo* pAppInfo)noexcept
		{
			this->pApplicationInfo = pAppInfo;

			this->enabledExtensionCount = 0;
			this->ppEnabledExtensionNames = nullptr;

			this->enabledLayerCount = 0;
			this->ppEnabledLayerNames = nullptr;

			//この下固定値
			this->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			this->pNext = nullptr;
			this->flags = 0;
		}

		HVKInstanceCreateInfo& HVKInstanceCreateInfo::setEnableExtensionInfo(const char** names, uint32_t extensionCount)noexcept
		{
			this->extensionNames.resize(extensionCount);
			this->extensionNames.shrink_to_fit();
			for (uint32_t i = 0; i < extensionCount; ++i) {
				this->extensionNames[i] = names[i];
			}
			this->enabledExtensionCount = static_cast<uint32_t>(this->extensionNames.size());
			this->ppEnabledExtensionNames = this->extensionNames.data();
			return *this;
		}

		HVKInstanceCreateInfo& HVKInstanceCreateInfo::setEnableExtensionInfo(const VkExtensionProperties* props, uint32_t extensionCount)noexcept
		{
			this->extensionNames.resize(extensionCount);
			this->extensionNames.shrink_to_fit();
			for (uint32_t i = 0; i < extensionCount; ++i) {
				this->extensionNames[i] = props[i].extensionName;
			}
			this->enabledExtensionCount = static_cast<uint32_t>(this->extensionNames.size());
			this->ppEnabledExtensionNames = this->extensionNames.data();
			return *this;
		}

		HVKInstanceCreateInfo& HVKInstanceCreateInfo::updateEnableExtensionInfo()noexcept
		{
			this->enabledExtensionCount = static_cast<uint32_t>(this->extensionNames.size());
			this->ppEnabledExtensionNames = this->extensionNames.data();
			return *this;
		}

		HVKInstanceCreateInfo& HVKInstanceCreateInfo::setEnableLayerNames(const char** names, uint32_t layerCount)noexcept
		{
			this->layerNames.resize(layerCount);
			this->layerNames.shrink_to_fit();

			for (uint32_t i = 0; i < layerCount; ++i) {
				this->layerNames[i] = names[i];
			}
			this->enabledLayerCount = static_cast<uint32_t>(this->layerNames.size());
			this->ppEnabledLayerNames = this->layerNames.data();

			return *this;
		}

		HVKInstanceCreateInfo& HVKInstanceCreateInfo::updateEnableLayerNames()noexcept
		{
			this->enabledLayerCount = static_cast<uint32_t>(this->layerNames.size());
			this->ppEnabledLayerNames = this->layerNames.data();
			return *this;
		}

	}

	namespace graphics
	{
		HVKApplicationInfo::HVKApplicationInfo()noexcept
		{
			this->sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			this->pNext = nullptr;

			this->apiVersion = VK_API_VERSION_1_0;

			this->setEngine(nullptr, 1);
			this->setApp(nullptr, 1);
		}

		HVKApplicationInfo& HVKApplicationInfo::setEngine(const char* name, uint32_t version)noexcept
		{
			this->pEngineName = name;
			this->engineVersion = version;
			return *this;
		}

		HVKApplicationInfo& HVKApplicationInfo::setApp(const char* name, uint32_t version)noexcept
		{
			this->pApplicationName = name;
			this->applicationVersion = version;
			return *this;
		}
	}

}