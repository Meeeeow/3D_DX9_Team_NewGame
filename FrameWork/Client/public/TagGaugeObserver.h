#pragma once
#ifndef __CLIENT_TAG_GAUGE_OBSERVER_H__
#define __CLIENT_TAG_GAUGE_OBSERVER_H__

#include "Observer.h"

BEGIN(Client)

class CTagGaugeObserver :
	public CObserver
{
public:
	explicit CTagGaugeObserver(_pDevice pDevice);
	explicit CTagGaugeObserver(const CTagGaugeObserver& rhs);
	virtual ~CTagGaugeObserver() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

private:
	class CTagGauge* m_pTagGauge = nullptr;

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
	virtual void Notify(void * pMessage = nullptr) override;
	static CTagGaugeObserver* Create(_pDevice pDevice); 
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__CLIENT_BOSS_SKELETON_STAGE_CUBE_OBSERVER_H__
