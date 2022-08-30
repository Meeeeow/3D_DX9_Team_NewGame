#pragma once
#ifndef __CLIENT_NON_CONTROL_OBSERVER_H__
#define __CLIENT_NON_CONTROL_OBSERVER_H__

#include "Observer.h"

BEGIN(Client)

class CNonControlObserver :
	public CObserver
{
public:
	explicit CNonControlObserver(_pDevice pDevice);
	explicit CNonControlObserver(const CNonControlObserver& rhs);
	virtual ~CNonControlObserver() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

private:
	class CNonControl* m_pNonControl = nullptr;

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
	virtual void Notify(void * pMessage = nullptr) override;
	static CNonControlObserver* Create(_pDevice pDevice); 
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__CLIENT_BOSS_SKELETON_STAGE_CUBE_OBSERVER_H__
