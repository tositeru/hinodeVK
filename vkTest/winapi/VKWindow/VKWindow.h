#pragma once

#include <graphics\vk\utility\winapi\Window.h>

class VKWindow : public hinode::winapi::Window
{
public:
	struct InitParam : public hinode::winapi::Window::InitParam
	{
		UINT frameCount;

		InitParam();
		InitParam(LONG width, LONG height, const TCHAR* pTitle)noexcept;
	};

public:
	VKWindow();

	virtual ~VKWindow();

	void clear();
	void create(const InitParam& initParam);

	virtual LRESULT onResize(WPARAM wParam, UINT width, UINT height)override;

	virtual LRESULT onKeyDown(UINT8 key, const KeyInfo& keyInfo)override;

	virtual LRESULT onKeyUp(UINT8 key, const KeyInfo& keyInfo)override;

	virtual void onPaint()override;

private:

};

