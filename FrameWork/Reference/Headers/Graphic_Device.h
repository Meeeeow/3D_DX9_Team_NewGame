#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;
public:
	LPDIRECT3DDEVICE9 Get_Device()
	{
		return m_pDevice;
	}
	LPD3DXSPRITE Get_Sprite() { return m_pSprite; }
	LPD3DXFONT Get_Font() { return m_pFont; }
	LPD3DXLINE Get_Line() { return m_pLine; }
public:
	HRESULT Ready_Graphic_Device(HWND hWnd, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppOut);

	void Render_Begin();
	void Render_End(HWND hWND = nullptr);
	void Draw_Font(const _tchar * pText, D3DXCOLOR FontColor, _vec3* vPos = nullptr, _vec3 vScale = _vec3(0.5f, 1.f, 1.f));

private:
	// com 객체 
	LPDIRECT3D9 m_pSDK; // 장치의 수준을 조사하고 장치를 제어할 컴객체를 만드는 역할
	LPDIRECT3DDEVICE9 m_pDevice; //장치를 제어할 컴객체. 
	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT m_pFont;
	LPD3DXLINE m_pLine;

public:
	virtual void Free() override;
};

END