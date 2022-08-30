#include "Graphic_Device.h"
#include <Windows.h>
#include <filesystem>
#include <io.h>

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
{

}

HRESULT CGraphic_Device::Ready_Graphic_Device(HWND hWnd, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppOut)
{
	D3DCAPS9 d3dcaps;
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps)))
		goto ERR;

	DWORD vp = 0;

	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = iWinCX;
	d3dpp.BackBufferHeight = iWinCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = TRUE;// TRUE -창모드,FALSE - 전체화면모드  
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pDevice)))
		goto ERR;

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
		goto ERR;

	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
		goto ERR;





	_finddata_t fd;

	__int64 handle = _findfirst("../bin/Resource/Font/*.ttf", &fd);

	int iResult = 0;

	char szCurPath[128] = "../bin/Resource/Font/";
	char szFullPath[128] = "";

	strcpy_s(szFullPath, szCurPath);
	strcat_s(szFullPath, fd.name);
	size_t iCount = 0;
	wchar_t szTag[128] = L"";
	int nLen = MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szTag, nLen);

	_findclose(handle);
	
	_int i = AddFontResource(szTag);

	D3DXFONT_DESCW font;
	ZeroMemory(&font, sizeof(D3DXFONT_DESCW));
	font.Height = 35;
	font.Width = 35;
	font.Weight = FW_EXTRABOLD;
	font.CharSet = HANGUL_CHARSET;
	lstrcpy(font.FaceName, L"닉스곤체 B 2.0");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &font, &m_pFont)))
		goto ERR;

	*ppOut = m_pDevice;
	Safe_AddRef(m_pDevice);

	return S_OK;
ERR:
	MSGBOX("Ready_Graphic_Device Failed");
	return E_FAIL;
}


void CGraphic_Device::Render_Begin()
{
	CGraphic_Device::Get_Instance()->Get_Device()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	CGraphic_Device::Get_Instance()->Get_Device()->BeginScene();
}

void CGraphic_Device::Render_End(HWND hWND /*= nullptr*/)
{
	CGraphic_Device::Get_Instance()->Get_Device()->EndScene();
	CGraphic_Device::Get_Instance()->Get_Device()->Present(nullptr, nullptr, hWND, nullptr);
}

void CGraphic_Device::Draw_Font(const _tchar * pText, D3DXCOLOR FontColor, _vec3* vPos, _vec3 vScale)
{
	if (nullptr == m_pFont)
		return;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	if (nullptr != vPos)
	{
		_mat matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixTranslation(&matTrans, vPos->x, vPos->y, 0.f);
		matWorld = matScale * matTrans;
		m_pSprite->SetTransform(&matWorld);
	}
	
	m_pFont->DrawTextW(m_pSprite, pText, lstrlen(pText), nullptr, DT_NOCLIP, FontColor);

	m_pSprite->End();
}

void CGraphic_Device::Free()
{
	if (m_pLine)
		m_pLine->Release();

	if (m_pFont)
		m_pFont->Release();

	if (m_pSprite)
		m_pSprite->Release();

	if (m_pDevice)
		m_pDevice->Release();

	if (m_pSDK)
		m_pSDK->Release();
}

