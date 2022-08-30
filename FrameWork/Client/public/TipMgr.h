#pragma once
#ifndef __CLIENT_TIP_MGR_H__
#define __CLIENT_TIP_MGR_H__

#include "Base.h"
BEGIN(Engine)
class CRenderer;
class CGameObject;
END

BEGIN(Client)
class CTipMgr :
	public CBase
{
	DECLARE_SINGLETON(CTipMgr)

private:
	explicit CTipMgr();
	virtual ~CTipMgr();

public:
	void SetUp_Mgr();
	void Tick(_float fTimeDelta);


public:
	void Add_Obj(class CTipObject* pObj);
	void Set_Out();

private:
	void Tick_In(_float fTimeDelta);
	void Tick_Out(_float fTimeDelta);
	void Tick_End(_float fTimeDelta);

	void Clear_Container();

private:
	list<class CTipObject*>		m_listObj;
	CRenderer*				m_pRenderer = nullptr;

	_bool					m_bTick = false;

	_bool					m_bIn = false;
	_bool					m_bFirstIn = false;
	_bool					m_bFirstBounce = false;
	const _float			m_fStartY = _float(g_iWinCY);
	_float					m_fY = _float(g_iWinCY);
	_float					m_fPreY = _float(g_iWinCY);
	_float					m_fFallTime = 0.f;

	_bool					m_bOut = false;
	_bool					m_bDown = false;
	_bool					m_bUp = false;
	_float					m_fOutFallTime = 0.f;
	const _float			m_fOutStartY = 0.f;
	_float					m_fOutY = 0.f;
	_float					m_fOutPreY = 0.f;

public:
	virtual void Free() override;
};
END
#endif // !__CLIENT_TIP_MGR_H__
