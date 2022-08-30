#pragma once
#ifndef __CLIENT_TEMPLE_CUBE_DESTROYABLE_H__
#define __CLIENT_TEMPLE_CUBE_DESTROYABLE_H__



#include "TileCube.h"

BEGIN(Client)
class CTempleCubeDestroyable final : public CTileCube
{
public:
	explicit CTempleCubeDestroyable(_pDevice pGraphicDevice);
	explicit CTempleCubeDestroyable(const CTempleCubeDestroyable& rhs);
	virtual ~CTempleCubeDestroyable() DEFAULT;

public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
private:
	HRESULT			SetUp_Component(void* pArg);


private:
	_int			m_iDestroyCount = 0;

public:
	// CTileCube을(를) 통해 상속됨
	static CTempleCubeDestroyable* Create(_pDevice pGraphicDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Collision(CGameObject * pColObj) override;
	virtual void Free() override;
};
END
#endif // !__CLIENT_TEMPLE_CUBE_DESTROYABLE_H__
