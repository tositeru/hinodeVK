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
			/// @brief WindowsŠÂ‹«‰º‚Å‚ÌVkSurfaceKHR‚Ìì¬
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkWin32SurfaceCreateInfoKHR* pCreateInfo);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
			/// @brief AndroidŠÂ‹«‰º‚Å‚ÌVkSurfaceKHR‚Ìì¬
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkAndroidSurfaceCreateInfoKHR* pCreateInfo);
#elif defined(VK_USE_PLATFORM_MIR_KHR)
			/// @brief Mir(Linux)ŠÂ‹«‰º‚Å‚ÌVkSurfaceKHR‚Ìì¬
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkMirSurfaceCreateInfoKHR* pCreateInfo);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
			/// @brief Wayland(Linux)ŠÂ‹«‰º‚Å‚ÌVkSurfaceKHR‚Ìì¬
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkWaylandSurfaceCreateInfoKHR* pCreateInfo);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
			/// @brief XCB‚ðŽg‚Á‚½X11 windowŠÂ‹«‰º‚Å‚ÌVkSurfaceKHR‚Ìì¬
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkXcbSurfaceCreateInfoKHR* pCreateInfo);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
			/// @brief Xlib‚ðŽg‚Á‚½X11 windowŠÂ‹«‰º‚Å‚ÌVkSurfaceKHR‚Ìì¬
			/// @param[in] instance
			/// @param[in] pCreateInfo
			/// @exception HVKException
			void create(VkInstance instance, VkXlibSurfaceCreateInfoKHR* pCreateInfo);
#else
#error DefineKHRPlatform.h‚ÅŠÂ‹«‚ðŽw’è‚µ‚Ä‚­‚¾‚³‚¢
#endif

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