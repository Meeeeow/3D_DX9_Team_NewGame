#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
class CFrustum;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT NativeConstruct();
	_uint Tick(_float fTimeDelta);
	HRESULT Render();

private:
	HRESULT Open_Level(LEVEL eLevel);
	HRESULT Ready_Prototype_Component_ForStatic();
	HRESULT Ready_Texture(CGameInstance* pGameInstance);
	HRESULT Ready_TextureGamePlayPlayer(CGameInstance* pGameInstance);
	HRESULT Ready_Prototype_GameObject_ForStatic();
	HRESULT SetUp_SamplerState();

private:
	HRESULT Ready_PlayerBuffer();

private:
	CGameInstance*			m_pGameInstance = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDevice = nullptr;
	CRenderer*				m_pRenderer = nullptr;
	CFrustum*				m_pFrustum = nullptr;

#ifdef _DEBUG
private:
	_float			m_fTimerAcc = 0.f;
	_tchar			m_szFPS[MAX_PATH] = TEXT("");
	_uint			m_iNumDraw = 0;
#endif

public:
	static CMainApp* Create();
	virtual void Free();
};
	
END





