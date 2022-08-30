#pragma once
#ifndef __CLIENT_LEVEL_TEST_H__
#define __CLIENT_LEVEL_TEST_H__	 

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CVIBufferTerrain;
END
BEGIN(Client)

class CLevelTest final:
	public CLevel
{
private:
	explicit CLevelTest(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevelTest() = default;
public:
	virtual HRESULT NativeConstruct(_uint iLevelID) override;
	virtual _int Tick(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Cube(const _tchar* pLayerTag);

public:
	static CLevelTest* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID);
	virtual void Free() override;
};

END

#endif // !__CLIENT_LEVEL_LOGO_H__
