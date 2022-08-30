#pragma once

#ifndef __RESURRECTIONMANAGER_H__
#define __RESURRECTIONMANAGER_H__

BEGIN(Engine)
END

#include "Observer.h"

BEGIN(Client)
class CResurrectionManager final 
	: CBase
{
	DECLARE_SINGLETON(CResurrectionManager)
private:
	explicit CResurrectionManager();
	virtual ~CResurrectionManager() = default;

public:
	HRESULT Add_Monster(CObserver* pObserver);
	HRESULT Sub_Monster(CObserver* pObserver);

public:
	void Resurrection();
	void Chimera(OBJ_INFO* pObj);

private:
	list<CObserver*>		m_listObserver; 

	_uint					m_iSpell = 0;

public:
	virtual void Free() override;
};
END
#endif // !__RESURRECTIONMANAGER_H__
