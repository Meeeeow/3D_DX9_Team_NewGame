#pragma once
#ifndef __CLIENT_TELEKINESIS_MANAGER_H__
#define __CLIENT_TELEKINESIS_MANAGER_H__

#include "Base.h"
#include "Monster.h"

class CTelekinesisManager final : public CBase
{
public:
	DECLARE_SINGLETON(CTelekinesisManager)

public:
	CTelekinesisManager();
	virtual ~CTelekinesisManager() DEFAULT;


public:
	void	Set_MonsterTransform(CTransform* pTransform);
	void	Telekinesis(_float fTimeDelta, CTransform* pPlayerTransform);

private:
	CTransform* m_pTargetTransform = nullptr;
	_float		m_fTelekinesis = 5.f;
	_bool		m_bTelekinesis = false;

	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};


#endif // !__CLIENT_TELEKINESIS_MANAGER_H__
