#pragma once
#ifndef __ENGINE_LEVEL_MANAGER_H__
#define __ENGINE_LEVEL_MANAGER_H__

#include "Base.h"
#include "Level.h"

BEGIN(Engine)

class CLevel_Manager final:
	public CBase

{
	DECLARE_SINGLETON(CLevel_Manager)

private:
	explicit CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Change_Level(CLevel* pLevel);
	_int Tick(_float fTimeDelta);
	HRESULT Render();
	_int	Get_CurrentLevel() { return m_pCurLevel->Get_LevelID(); }
private:
	CLevel* m_pCurLevel = nullptr;

public:
	virtual void Free() override;
};

END

#endif // !__ENGINE_LEVEL_MANAGER_H__
