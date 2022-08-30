#include "..\public\InputDevice.h"

IMPLEMENT_SINGLETON(CInputDevice)

CInputDevice::CInputDevice()
{
	ZeroMemory(m_bKeyDownArr, sizeof(m_bKeyDownArr));
	ZeroMemory(m_bKeyUpArr, sizeof(m_bKeyUpArr));
	ZeroMemory(m_byKeyStateArr, sizeof(m_byKeyStateArr));
	ZeroMemory(m_bMouseDownArr, sizeof(m_bMouseDownArr));
	ZeroMemory(m_bMouseUpArr, sizeof(m_bMouseUpArr));
}

_bool CInputDevice::Key_Pressing(_ubyte byKeyID)
{
	if (m_byKeyStateArr[byKeyID] & 0x80)
		return true;
	else
		return false;
}

_bool CInputDevice::Key_Up(_ubyte byKeyID)
{
	if (Key_Pressing(byKeyID))
	{
		m_bKeyUpArr[byKeyID] = true;
		return false;
	}
	else if (m_bKeyUpArr[byKeyID])
	{
		m_bKeyUpArr[byKeyID] = false;
		return true;
	}

	return false;
}

_bool CInputDevice::Key_Down(_ubyte byKeyID)
{
	if (Key_Pressing(byKeyID) && !m_bKeyDownArr[byKeyID])
		return m_bKeyDownArr[byKeyID] = true;

	if (!Key_Pressing(byKeyID) && m_bKeyDownArr[byKeyID])
		return m_bKeyDownArr[byKeyID] = false;

	return false;
}

_bool CInputDevice::Mouse_Pressing(DIMB eMouseButton)
{
	if (Get_DIMouseButtonState(eMouseButton) & 0x80)
		return true;

	return false;
}

_bool CInputDevice::Mouse_Up(DIMB eMouseButton)
{
	if (Mouse_Pressing(eMouseButton))
	{
		m_bMouseUpArr[eMouseButton] = true;
		return false;
	}
	else if (m_bMouseUpArr[eMouseButton])
	{
		m_bMouseUpArr[eMouseButton] = false;
		return true;
	}

	return false;
}

_bool CInputDevice::Mouse_Down(DIMB eMouseButton)
{
	if (Mouse_Pressing(eMouseButton) && !m_bMouseDownArr[eMouseButton])
		return m_bMouseDownArr[eMouseButton] = true;

	if (!Mouse_Pressing(eMouseButton) && m_bMouseDownArr[eMouseButton])
		return m_bMouseDownArr[eMouseButton] = false;

	return false;
}

HRESULT CInputDevice::NativeConstruct(HINSTANCE hInstance, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		MSG_FAIL("CInputDevice NativeConstruct - DirectInput8Create");

	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		MSG_FAIL("CInputDevice NativeConstruct - CreateDevice");

	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		MSG_FAIL("CInputDevice NativeConstruct - SetDataFormat");

	if (FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		MSG_FAIL("CInputDevice NativeConstruct - SetCooperativeLevel");

	if (FAILED(m_pKeyBoard->Acquire()))
		MSG_FAIL("CInputDevice NativeConstruct - Acquire");


	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		MSG_FAIL("CInputDevice NativeConstruct - CreateDevice");

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		MSG_FAIL("CInputDevice NativeConstruct - SetDataFormat");

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		MSG_FAIL("CInputDevice NativeConstruct - SetCooperativeLevel");

	if (FAILED(m_pMouse->Acquire()))
		MSG_FAIL("CInputDevice NativeConstruct - Acquire");

	return S_OK;
}

HRESULT CInputDevice::Update_InputDeviceState()
{
	if (nullptr == m_pKeyBoard ||
		nullptr == m_pMouse)
		MSG_FAIL("CInputDevice Update_InputDeviceState - m_pKeyBoard is null or m_pMouse is null");

	if (FAILED(m_pKeyBoard->GetDeviceState(256, m_byKeyStateArr)))
		MSG_FAIL("CInputDevice Update_InputDeviceState - GetDeviceState m_pKeyBoard");

	if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
		MSG_FAIL("CInputDevice Update_InputDeviceState - GetDeviceState m_pMouse");

	return S_OK;
}

void CInputDevice::Free()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pInput);
}
