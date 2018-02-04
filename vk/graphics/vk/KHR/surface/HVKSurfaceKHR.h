#pragma once

#include "../DefineKHRPlatform.h"
#include <vulkan\vulkan.h>
#include <vector>

#include "../../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKSurfaceKHR : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKSurfaceKHR(HVKSurfaceKHR& right)noexcept = delete;
			HVKSurfaceKHR& operator=(HVKSurfaceKHR& right)noexcept = delete;

		public:
			HVKSurfaceKHR();
			HVKSurfaceKHR(HVKSurfaceKHR&& right)noexcept;
			HVKSurfaceKHR& operator=(HVKSurfaceKHR&& right)noexcept;
			~HVKSurfaceKHR();

			void release()noexcept override;

#ifdef VK_USE_PLATFORM_WIN32_KHR
			/// @brief Windows環境下でのVkSurfaceKHRの作成
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkWin32SurfaceCreateInfoKHR* pCreateInfo);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
			/// @brief Android環境下でのVkSurfaceKHRの作成
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkAndroidSurfaceCreateInfoKHR* pCreateInfo);
#elif defined(VK_USE_PLATFORM_MIR_KHR)
			/// @brief Mir(Linux)環境下でのVkSurfaceKHRの作成
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkMirSurfaceCreateInfoKHR* pCreateInfo);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
			/// @brief Wayland(Linux)環境下でのVkSurfaceKHRの作成
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkWaylandSurfaceCreateInfoKHR* pCreateInfo);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
			/// @brief XCBを使ったX11 window環境下でのVkSurfaceKHRの作成
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkXcbSurfaceCreateInfoKHR* pCreateInfo);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
			/// @brief Xlibを使ったX11 window環境下でのVkSurfaceKHRの作成
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkXlibSurfaceCreateInfoKHR* pCreateInfo);
#else
#error DefineKHRPlatform.hで環境を指定してください
#endif
			/// @brief 指定したVkPhysicalDeviceのキューファミリーがこの画面に表示することが可能かを返す
			/// @param[in] gpu
			/// @param[in] queueFamilyIndex
			/// @retval bool
			bool isSuppportPresent(VkPhysicalDevice gpu, uint32_t queueFamilyIndex);

			/// @brief gpuの全キューファミリーの画面の表示の可能性を返す
			/// @param[in] gpu
			/// @retval std::vector<bool>
			std::vector<bool> getSupportPresentOfAllQueueFamily(VkPhysicalDevice gpu);

		public:
			bool isGood()const noexcept;
			VkSurfaceKHR surface()noexcept;
			operator VkSurfaceKHR()noexcept { return this->surface(); }

		private:
			VkSurfaceKHR mSurface;
			VkInstance mParentInstance;

		};
	}
}