#pragma

#include <vector>
#include <vulkan\vulkan.h>

#include "../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKInstance : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKInstance(HVKInstance& right)noexcept = delete;
			HVKInstance& operator=(HVKInstance& right)noexcept = delete;

		public:
			HVKInstance();
			HVKInstance(HVKInstance&& right)noexcept;
			HVKInstance& operator=(HVKInstance&& right)noexcept;
			~HVKInstance();

			void release()noexcept override;

			/// @brief 作成
			/// @param[in] pInfo VkInstanceCreateInfoを継承したHVKInstanceCreateInfo構造体を用意しているので
			/// @exception HVKException
			void create(VkInstanceCreateInfo* pInfo);

			/// @brief 物理デバイスを列挙する
			/// @param[out] out
			/// @param[in] count
			/// @retval bool
			bool enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& out, uint32_t count)noexcept;

		public:
			bool isGood()const noexcept;
			VkInstance instance()noexcept;
			operator VkInstance()noexcept { return this->instance(); }

		private:
			VkInstance mInstance;
		};
	}

	namespace graphics
	{
		struct HVKInstanceCreateInfo : public VkInstanceCreateInfo
		{
			HVKInstanceCreateInfo()noexcept;
			HVKInstanceCreateInfo(VkApplicationInfo* pAppInfo)noexcept;

			HVKInstanceCreateInfo& setEnableExtensionInfo(const char** names, uint32_t extensionCount)noexcept;
			HVKInstanceCreateInfo& setEnableLayerCount(const char** names, uint32_t layerCount)noexcept;
		};
	}

	namespace graphics
	{
		struct HVKApplicationInfo : public VkApplicationInfo
		{
			HVKApplicationInfo()noexcept;

			HVKApplicationInfo& setEngine(const char* name, uint32_t version)noexcept;
			HVKApplicationInfo& setApp(const char* name, uint32_t version)noexcept;
		};
	}

}