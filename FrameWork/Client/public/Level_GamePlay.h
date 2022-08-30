#pragma once
#ifndef __CLIENT_LEVEL_GAMEPLAY_H__
#define __CLIENT_LEVEL_GAMEPLAY_H__	 

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CVIBufferTerrain;
END
BEGIN(Client)

class CLevel_GamePlay final:
	public CLevel
{
private:
	explicit CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;
public:
	virtual HRESULT NativeConstruct(_uint iLevelID) override;
	virtual _int Tick(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT Ready_CollisionTag();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Crosshair(const _tchar* pLayerTag);
	HRESULT Ready_UI(list<class CPlayer*>& plist);

public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID);
	virtual void Free() override;
};

END

#endif // !__CLIENT_LEVEL_LOGO_H__
