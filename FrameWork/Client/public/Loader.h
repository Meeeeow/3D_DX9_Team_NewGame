#pragma once
#ifndef __CLIENT_LOADER_H__
#define __CLIENT_LOADER_H__

#include "Client_Defines.h"
#include "Base.h"
BEGIN(Client)
class CLoader final:
	public CBase
{
public:
	explicit CLoader(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CS() {
		return m_CS;
	}

	LEVEL Get_NextLevelID() const {
		return m_eNextLevel;
	}

	_bool Get_Finished() const {
		return m_isFinished;
	}

	const _tchar* Get_LoadingText() const {
		return m_szLoading;
	}

public:
	HRESULT NativeConstruct(LEVEL eLevel);
	HRESULT LoadingForGamePlay();
	HRESULT LoadingForTest();
	HRESULT LoadingForBossSkeleton();
	HRESULT LoadingForCircus();
	HRESULT LoadingForNecromencer();
private:
	HRESULT Ready_TextureGamePlayMonster(CGameInstance* pInstance);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDevice = nullptr;
	LEVEL					m_eNextLevel = LEVEL_END;
	HANDLE					m_hThread = 0;
	CRITICAL_SECTION		m_CS;

	_tchar					m_szLoading[MAX_PATH] = TEXT("");
	_bool					m_isFinished = false;

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual void Free() override;
};
END
#endif // !__CLIENT_LOADER_H__
