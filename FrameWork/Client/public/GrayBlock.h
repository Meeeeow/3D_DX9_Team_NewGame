#pragma once
#ifndef __CLIENT_GRAY_BLOCK_H__
#define __CLIENT_GRAY_BLOCK_H__

#include "TileCube.h"

BEGIN(Client)
class CGrayBlock final : public CTileCube
{
public:
	explicit CGrayBlock(_pDevice pGraphicDevice);
	explicit CGrayBlock(const CGrayBlock& rhs);
	virtual ~CGrayBlock() DEFAULT;


	virtual HRESULT NativeConstruct_Prototype()override;
	virtual HRESULT NativeConstruct(void* pArg)override;
	
private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_Component(void* pArg);

public:
	// CTileCube을(를) 통해 상속됨
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Collision(CGameObject * pColObj) override;
	virtual void Free() override;
	static	CGrayBlock*	Create(_pDevice pGraphicDevice);

private:
};
END
#endif // !__CLIENT_GRAY_BLOCK_H__
