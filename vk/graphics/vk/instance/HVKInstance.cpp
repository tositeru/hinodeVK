#include "HVKInstance.h"

#include <utility> //for std::move()

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKInstance::HVKInstance()
			: mInstance(nullptr)
		{ }

		HVKInstance::HVKInstance(HVKInstance&& right)noexcept
		{
			*this = std::move(right);
		}

		HVKInstance& HVKInstance::operator=(HVKInstance&& right)noexcept
		{
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
			this->enabledExtensionCount = extensionCount;
			this->ppEnabledExtensionNames = names;
			return *this;
		}

		HVKInstanceCreateInfo& HVKInstanceCreateInfo::setEnableLayerCount(const char** names, uint32_t layerCount)noexcept
		{
			this->enabledLayerCount = layerCount;
			this->ppEnabledLayerNames = names;
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