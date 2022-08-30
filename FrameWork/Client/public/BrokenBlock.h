#pragma once
#ifndef __CLIENT_BROKEN_BLOCK_H__
#define __CLIENT_BROKEN_BLOCK_H__

#include "TileCube.h"

BEGIN(Client)
class CBrokenBlock final : public CTileCube
{
public:
	explicit CBrokenBlock(_pDevice pGraphicDevice);
	explicit CBrokenBlock(const CBrokenBlock& rhs);
	virtual ~CBrokenBlock() DEFAULT;

public:
	virtual HRESULT NativeConstruct_Prototype()override;
	virtual HRESULT NativeConstruct(void* pArg)override;

private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_Component(void* pArg);
public:
	// CTileCube을(를) 통해 상속됨
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	virtual void			Collision(CGameObject * pColObj) override;
	virtual void			Free() override;
	static	CBrokenBlock*	Create(_pDevice pGraphicDevice);

private:
	_int			m_iDurabillity = 3;
};
END
#endif // !__CLIENT_BROKEN_BLOCK_H__
