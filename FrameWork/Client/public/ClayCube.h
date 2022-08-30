#pragma once
#ifndef __CLIENT_CLAY_CUBE_H__
#define __CLIENT_CLAY_CUBE_H__

#include "TileCube.h"

BEGIN(Client)
class CClayCube final : public CTileCube
{
public:
	explicit CClayCube(_pDevice pGraphicDevice);
	explicit CClayCube(const CClayCube& rhs);
	virtual ~CClayCube() DEFAULT;


public:
	virtual HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;

private:
	HRESULT				SetUp_Component();
	HRESULT				SetUp_Component(void* pArg);

public:
	// CTileCube을(를) 통해 상속됨
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	static	CClayCube*		Create(_pDevice pGraphicDevice);
	virtual void			Free() override;

	// CTileCube을(를) 통해 상속됨
	virtual void Collision(CGameObject * pColObj) override;
};

END
#endif // !__CLIENT_CLAY_CUBE_H__
