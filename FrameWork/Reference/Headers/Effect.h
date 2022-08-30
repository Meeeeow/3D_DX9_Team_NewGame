#pragma once
#ifndef __ENGINE_EFFECT_H__
#define __ENGINE_EFFECT_H__

#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CEffect abstract:
	public CGameObject
{
public:
	enum BILL_BOARDING { BB_WITHOUTUP, BB_ALL, BB_END};
public:
	explicit CEffect(_pDevice pDevice);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta, _bool bPlayOnce = false);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public: //SET
	virtual HRESULT Compute_Distance(class CTransform* pTransform);
	virtual HRESULT BillBoarding(class CTransform* pTransform, BILL_BOARDING eBillBoardType = BB_WITHOUTUP);
	void Set_FrameValue(_int iEnd, _int iStart = 0, _float fDelay = 0.1f, _float fTime = 0.f);


public: //GET
	const _float& Get_CamDistance() const { return m_fCamDistance; }
	_int Get_Frame() { return m_tFrame.iStart; }

private:
	void Tick_Frame(_float fTimeDelta);
	void Tick_FrameOnce(_float fTimeDelta);

private:
	_float	m_fCamDistance = 0.f;
	FRAME	m_tFrame;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr)PURE;

	virtual _vec3	Get_MinPos();
	virtual _vec3	Get_MaxPos();
	virtual _vec3	Get_WorldPos();
	virtual _vec3	Get_Length();
};
END
#endif // !__ENGINE_EFFECT_H__
