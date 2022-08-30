#pragma once

#ifndef __TOMBSTONEMANAGER_H__
#define __TOMBSTONEMANAGER_H__

BEGIN(Engine)
END

#include "Observer.h"
#include "Shield.h"

BEGIN(Client)
class CTombStoneManager final
	: CBase
{
	DECLARE_SINGLETON(CTombStoneManager)
private:
	explicit CTombStoneManager();
	virtual ~CTombStoneManager() = default;

public:
	HRESULT Add_Shield(CShield* pShield);
	HRESULT Sub_Shield();
	HRESULT Add_TombStone(CObserver* pObserver);
	HRESULT Sub_TombStone(CObserver* pObserver);

public:
	void Broken();
	void Repair();

public:
	_vec3 Get_ShieldPos();

private:
	list<CObserver*>		m_listObserver;
	CShield*				m_pShield = nullptr;

	_int					m_iIdleTombStone = 0;

public:
	virtual void Free() override;

};
END
#endif // !__TOMBSTONEMANAGER_H__
