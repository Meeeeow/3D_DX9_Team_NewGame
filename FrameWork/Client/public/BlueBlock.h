#pragma once
#ifndef __CLIENT_BLUE_BLOCK_H__
#define __CLIENT_BLUE_BLOCK_H__

#include "TileCube.h"
BEGIN(Client)
class CBlueBlock final : public CTileCube
{
public:
	explicit CBlueBlock(_pDevice pGraphicDevice);
	explicit CBlueBlock(const CBlueBlock& rhs);
	virtual ~CBlueBlock() DEFAULT;


public:
	virtual HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;

private:
	HRESULT				SetUp_Component();
	HRESULT				SetUp_Component(void* pArg);
	void				BlueBlock_Debuff(class CPlayer* pPlayerObject);
	_uint				m_iNumDebuffAcc = 0;


public:
	// CTileCube을(를) 통해 상속됨
	virtual CGameObject* Clone(void * pArg = nullptr) override;
	virtual void Collision(CGameObject * pColObj) override;
	static	CBlueBlock*	Create(_pDevice pGraphicDevice);
	virtual void Free() override;
};

END
#endif // !__CLIENT_BLUE_BLOCK_H__

