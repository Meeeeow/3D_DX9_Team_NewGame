#pragma once
#ifndef __ENGINE_INPUT_DEVICE_H__
#define __ENGINE_INPUT_DEVICE_H__

#include "Base.h"
BEGIN(Engine)
class CInputDevice final:
	public CBase
{
	DECLARE_SINGLETON(CInputDevice)

private:
	explicit CInputDevice();
	virtual ~CInputDevice() = default;

public:
	enum DIMB { DIMB_LBUTTON, DIMB_RBUTTON, DIMB_WHEEL, DIMB_X, DIMB_END };
	enum DIMM { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };

public:
	_byte Get_DIKeyState(_ubyte byKeyID) {
		return m_byKeyStateArr[byKeyID];
	}

	_byte Get_DIMouseButtonState(DIMB eMouseButton) {
		return m_MouseState.rgbButtons[eMouseButton];
	}

	_long Get_DIMouseMoveState(DIMM eMouseMove) {
		return ((_long*)&m_MouseState)[eMouseMove];
	}

	_bool Key_Pressing(_ubyte byKeyID);
	_bool Key_Up(_ubyte byKeyID);
	_bool Key_Down(_ubyte byKeyID);
	_bool Mouse_Pressing(DIMB eMouseButton);
	_bool Mouse_Up(DIMB eMouseButton);
	_bool Mouse_Down(DIMB eMouseButton);

public:
	HRESULT NativeConstruct(HINSTANCE hInstance, HWND hWnd);
	HRESULT Update_InputDeviceState();

public:
	LPDIRECTINPUT8				m_pInput = nullptr;
	LPDIRECTINPUTDEVICE8		m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;

private:
	_byte						m_byKeyStateArr[256];
	_bool						m_bKeyUpArr[256];
	_bool						m_bKeyDownArr[256];

	DIMOUSESTATE				m_MouseState;
	_bool						m_bMouseUpArr[4];
	_bool						m_bMouseDownArr[4];

public:
	virtual void Free() override;



};
END
#endif // !__ENGINE_INPUT_DEVICE_H__
