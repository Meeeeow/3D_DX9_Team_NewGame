#pragma once
#ifndef __CLIENT_LEVEL_LOGO_H__
#define __CLIENT_LEVEL_LOGO_H__	 

#include "..\..\Reference\Headers\Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CLevel_Logo final:
	public CLevel
{
private:
	explicit CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Logo() = default;
public:
	virtual HRESULT NativeConstruct(_uint iLevelID);
	virtual _int Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Layer_Backgroud(const _tchar* pLayerTag);

public:
	static CLevel_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID);
	virtual void Free() override;

};

END

#endif // !__CLIENT_LEVEL_LOGO_H__
