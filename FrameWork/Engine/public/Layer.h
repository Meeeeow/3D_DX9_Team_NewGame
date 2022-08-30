#pragma once
#ifndef __ENGINE_LAYER_H__
#define __ENGINE_LAYER_H__

#include "Base.h"
BEGIN(Engine)

class CLayer final :
	public CBase
{
	typedef list<class CGameObject*>	LISTOBJ;

private:
	explicit CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pObj);
	HRESULT NativeConstruct();
	_int Tick(_float fTimeDelta);
	_int LateTick(_float fTimeDelta);

public: //GET
	CGameObject*		Get_GameObjPtr(_int iIndex);
	LISTOBJ				Get_ObjectList() { return m_listObj; }
private:
	list<class CGameObject*>			m_listObj;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END
#endif // !__ENGINE_LAYER_H__
