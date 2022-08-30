#pragma once

#ifndef __MONSTERCOLLIDERMANAGER_H__
#define __MONSTERCOLLIDERMANAGER_H__

BEGIN(Engine)
END

BEGIN(Client)
class CMonsterColliderManager final
	: CBase
{
	DECLARE_SINGLETON(CMonsterColliderManager)
public:
	enum COLLIDER { COLLIDER_MELEE_SKULL, COLLIDER_RANGE_SKULL, COLLIDER_NECROMENCER, COLLIDER_CHIMERA, COLLIDER_END };
private:
	explicit CMonsterColliderManager();
	virtual ~CMonsterColliderManager() = default;

public:
	OBJ_INFO* Get_ColliderDat(COLLIDER eDat);
	void Add_ColliderDat(COLLIDER eDat, OBJ_INFO* pObj);

private:
	OBJ_INFO*	m_pColliderDat[COLLIDER_END];

public:
	virtual void Free() override;

};
END
#endif // !__MONSTERCOLLIDERMANAGER_H__
