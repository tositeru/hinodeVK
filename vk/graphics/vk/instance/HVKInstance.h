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
			/// @brief グローバルな拡張プロパティを列挙する
			/// @param[in] layerName 対象となるレイヤー名. nullptrならVulkan実装か明示的に有効化されているレイヤーの拡張プロパティを返す
			/// @param[in] propCount 0なら存在するプロパティをすべて列挙する
			/// @retval std::vector<VkExtensionProperties>
			static std::vector<VkExtensionProperties> sEnumerateExtensionProperties(const char* layerName = nullptr, uint32_t propCount = 0);

		public:
			HVKInstance();
			HVKInstance(HVKInstance&& right)noexcept;
			HVKInstance& operator=(HVKInstance&& right)noexcept;
			~HVKInstance();

			void release()noexcept override;

			/// @brief 作成
			/// @param[in] pInfo VkInstanceCreateInfoを継承したHVKInstanceCreateInfo構造体を用意しているのでそちらを使用してください
			/// @exception HVKException
			void create(VkInstanceCreateInfo* pInfo);

			/// @brief 物理デバイスを列挙する
			/// @param[out] out
			/// @param[in] count
			/// @retval bool
			bool enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& out, uint32_t count)noexcept;

			/// @brief 拡張機能を提供する関数などを取得するための関数
			/// この関数で返された関数ポインタの第1引数はこのインスタンスのVkInstanceか、その子でないといけない
			/// @param[in] name 
			/// @retval PFN_vkVoidFunction
			PFN_vkVoidFunction getProcAddr(const char* name)noexcept;

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
			std::vector<const char*> extensionNames;
			std::vector<const char*> layerNames;

			HVKInstanceCreateInfo()noexcept;
			HVKInstanceCreateInfo(VkApplicationInfo* pAppInfo)noexcept;

			HVKInstanceCreateInfo& setEnableExtensionInfo(const char** names, uint32_t extensionCount)noexcept;
			HVKInstanceCreateInfo& setEnableExtensionInfo(const VkExtensionProperties* props, uint32_t extensionCount)noexcept;

			HVKInstanceCreateInfo& setEnableLayerNames(const char** names, uint32_t layerCount)noexcept;
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