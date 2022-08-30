#pragma once
#ifndef __CLIENT_RED_BLOCK_H__
#define __CLIENT_RED_BLOCK_H__


#include "TileCube.h"

BEGIN(Client)
class CRedBlock final : public CTileCube
{
public:
	explicit CRedBlock(_pDevice pGraphicDevice);
	explicit CRedBlock(const CRedBlock& rhs);
	virtual ~CRedBlock() DEFAULT;


public:
	virtual HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;

private:
	HRESULT				SetUp_Component();
	HRESULT				SetUp_Component(void* pArg);
	void				RedBlock_Debuff(class CPlayer* pPlayerObject);
	_uint				m_iNumDebuffAcc = 0;

public:
	// CTileCube을(를) 통해 상속됨
	virtual CGameObject* Clone(void * pArg = nullptr) override;
	virtual void Collision(CGameObject * pColObj) override;
	static	CRedBlock*	Create(_pDevice pGraphicDevice);
	virtual void Free() override;
};
END
#endif // !__CLIENT_RED_BLOCK_H__