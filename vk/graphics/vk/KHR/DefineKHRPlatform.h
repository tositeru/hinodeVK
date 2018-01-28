#pragma once

//
//	KHR�̊����w�肷��}�N�����`����w�b�_�[
//

#ifdef _WIN32
#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#elif defined(__ANDROID__)
#ifndef VK_USE_PLATFORM_ANDROID_KHR
#define VK_USE_PLATFORM_ANDROID_KHR
#endif

#else
//�������Ƒ���Linux�����ŃR���p�C������邱�ƂɂȂ��Ă���Ǝv����̂ŁA�J�����ɍ��킹���}�N����K���ɑI��ł�������

#define USED_MIR_PLATFORM 0
#define USED_WAYLAND_PLATFORM 1
#define USED_XCB_PLATFORM 2
#define USED_XLIB_PLATFORM 2
#define USED_PLATFORM USED_WAYLAND_PLATFORM

#if USED_PLATFORM == USED_MIR_PLATFORM
#ifndef VK_USE_PLATFORM_MIR_KHR
#define VK_USE_PLATFORM_MIR_KHR
#endif

#elif USED_PLATFORM == USED_WAYLAND_PLATFORM
// Wayland Display server
#ifndef VK_USE_PLATFORM_MIR_KHR
#define VK_USE_PLATFORM_MIR_KHR
#endif

#elif USED_PLATFORM == USED_XCB_PLATFORM
// X11 window, using the XCB client-side library
#ifndef VK_USE_PLATFORM_XCB_KHR
#define VK_USE_PLATFORM_XCB_KHR
#endif

#elif USED_PLATFORM == USED_XLIB_PLATFORM
// X11 window, using the Xlib client-side library
#ifndef VK_USE_PLATFORM_XLIB_KHR
#define VK_USE_PLATFORM_XLIB_KHR
#endif

#endif

#endif