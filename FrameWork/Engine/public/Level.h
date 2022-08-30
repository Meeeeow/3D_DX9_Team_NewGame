#pragma once
#ifndef __ENGINE_LEVEL_H__
#define __ENGINE_LEVEL_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract:
	public CBase
{
protected:
	explicit CLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel() = default;

public:
	_uint Get_LevelID() const {	return m_iLevelID; }

public:
	virtual HRESULT NativeConstruct(_uint iLevelID);
	virtual _int Tick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	_uint						m_iLevelID = 0;
public:
	virtual void Free() override;
};

END
#endif // !__ENGINE_LEVEL_H__
