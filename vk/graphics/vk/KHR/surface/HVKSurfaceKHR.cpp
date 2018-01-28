#include "HVKSurfaceKHR.h"

#include <utility> //for std::move()

#include "../../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKSurfaceKHR::HVKSurfaceKHR()
			: mSurface(nullptr)
			, mParentInstance(nullptr)
		{ }

		HVKSurfaceKHR::HVKSurfaceKHR(HVKSurfaceKHR&& right)noexcept
			: mSurface(right.mSurface)
			, mParentInstance(right.mParentInstance)
		{
			right.mSurface = nullptr;
			right.mParentInstance = nullptr;
		}

		HVKSurfaceKHR& HVKSurfaceKHR::operator=(HVKSurfaceKHR&& right)noexcept
		{
			this->mSurface = right.mSurface;
			this->mParentInstance = right.mParentInstance;
			right.mSurface = nullptr;
			right.mParentInstance = nullptr;
			return *this;
		}

		HVKSurfaceKHR::~HVKSurfaceKHR()
		{
			this->release();
		}

		void HVKSurfaceKHR::release()noexcept
		{
			if (this->mSurface != nullptr) {
				vkDestroySurfaceKHR(this->mParentInstance, this->mSurface, this->allocationCallbacksPointer());
				this->mSurface = nullptr;
				this->mParentInstance = nullptr;
			}
		}

#ifdef VK_USE_PLATFORM_WIN32_KHR
		void HVKSurfaceKHR::create(VkInstance instance, VkWin32SurfaceCreateInfoKHR* pCreateInfo)
		{
			pCreateInfo->sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			pCreateInfo->pNext = nullptr;
			pCreateInfo->flags = 0;
			auto ret = vkCreateWin32SurfaceKHR(instance, pCreateInfo, this->allocationCallbacksPointer(), &this->mSurface);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSurfaceKHR, create, ret);
			}
			this->mParentInstance = instance;
		}

#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
		void HVKSurfaceKHR::create(VkInstance instance, VkAndroidSurfaceCreateInfoKHR* pCreateInfo)
		{
			pCreateInfo->sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
			pCreateInfo->pNext = nullptr;
			pCreateInfo->flags = 0;
			auto ret = vkCreateAndroidSurfaceKHR(instance, pCreateInfo, this->allocationCallbacksPointer(), &this->mSurface);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSurfaceKHR, create, ret);
	}
			this->mParentInstance = instance;
		}
#elif defined(VK_USE_PLATFORM_MIR_KHR)
		void HVKSurfaceKHR::create(VkInstance instance, VkMirSurfaceCreateInfoKHR* pCreateInfo)
		{
			pCreateInfo->sType = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR;
			pCreateInfo->pNext = nullptr;
			pCreateInfo->flags = 0;
			auto ret = VkMirSurfaceCreateInfoKHR(instance, pCreateInfo, this->allocationCallbacksPointer(), &this->mSurface);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSurfaceKHR, create, ret);
			}
			this->mParentInstance = instance;
		}

#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
		void HVKSurfaceKHR::create(VkInstance instance, VkWaylandSurfaceCreateInfoKHR* pCreateInfo)
		{
			pCreateInfo->sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
			pCreateInfo->pNext = nullptr;
			pCreateInfo->flags = 0;
			auto ret = vkCreateWaylandSurfaceKHR(instance, pCreateInfo, this->allocationCallbacksPointer(), &this->mSurface);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSurfaceKHR, create, ret);
			}
			this->mParentInstance = instance;
		}

#elif defined(VK_USE_PLATFORM_XCB_KHR)
		void HVKSurfaceKHR::create(VkInstance instance, VkXcbSurfaceCreateInfoKHR* pCreateInfo)
		{
			pCreateInfo->sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
			pCreateInfo->pNext = nullptr;
			pCreateInfo->flags = 0;
			auto ret = vkCreateXcbSurfaceKHR(instance, pCreateInfo, this->allocationCallbacksPointer(), &this->mSurface);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSurfaceKHR, create, ret);
			}
			this->mParentInstance = instance;
		}

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		void create(VkInstance instance, VkXlibSurfaceCreateInfoKHR* pCreateInfo)
		{
			pCreateInfo->sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
			pCreateInfo->pNext = nullptr;
			pCreateInfo->flags = 0;
			auto ret = vkCreateXlibSurfaceKHR(instance, pCreateInfo, this->allocationCallbacksPointer(), &this->mSurface);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKSurfaceKHR, create, ret);
			}
			this->mParentInstance = instance;
		}

#else
#error DefineKHRPlatform.h‚ÅŠÂ‹«‚ðŽw’è‚µ‚Ä‚­‚¾‚³‚¢
#endif

		bool HVKSurfaceKHR::isGood()const noexcept
		{
			return this->mSurface != nullptr;
		}

		VkSurfaceKHR HVKSurfaceKHR::surface()noexcept
		{
			assert(this->isGood());
			return this->mSurface;
		}
	}
}