#pragma once
#ifndef __CLIENT_LEVEL_BOSS_SKELETON_H__
#define __CLIENT_LEVEL_BOSS_SKELETON_H__	 

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CVIBufferTerrain;
END
BEGIN(Client)

class CLevel_BossSkeleton final:
	public CLevel
{
private:
	explicit CLevel_BossSkeleton(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_BossSkeleton() = default;
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

public:
	static CLevel_BossSkeleton* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID);
	virtual void Free() override;
};

END

#endif // !__CLIENT_LEVEL_LOGO_H__
