#include "VKWindow.h"

#include <iostream>
#include <chrono>

//using namespace hinode::graphics;

VKWindow::VKWindow()
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

VKWindow::~VKWindow()
{
	this->clear();
}

void VKWindow::clear()
{
}

void VKWindow::create(const InitParam& initParam)
{
	Window::create(initParam);

	this->show();
}

LRESULT VKWindow::onResize(WPARAM wParam, UINT width, UINT height)
{
	return 0;
}

LRESULT VKWindow::onKeyDown(UINT8 key, const KeyInfo& keyInfo)
{
	if (static_cast<UINT8>(key) == VK_ESCAPE) {
		SendMessage(this->HWnd(), WM_DESTROY, 0, 0);
	}
	return 0;
}

LRESULT VKWindow::onKeyUp(UINT8 key, const KeyInfo& keyInfo)
{
	return 0;
}

void VKWindow::onPaint()
{
	//----------------------------------------------------------------------
	//	Ç±Ç±Ç…ï`âÊèàóùÇèëÇ¢ÇƒÇ≠ÇæÇ≥Ç¢
	//

	//----------------------------------------------------------------------
}

//-------------------------------------------------------------------------------------------------------------------
//
//	VKWindow::InitParam
//
//-------------------------------------------------------------------------------------------------------------------


VKWindow::InitParam::InitParam()
	: hinode::winapi::Window::InitParam()
	, frameCount(2)
{}

VKWindow::InitParam::InitParam(LONG width, LONG height, const TCHAR* pTitle)noexcept
	: hinode::winapi::Window::InitParam(width, height, pTitle)
	, frameCount(2)
{}
