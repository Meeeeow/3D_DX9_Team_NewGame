#pragma once

#ifndef __SHIELDCUBE_H__
#define __SHIELDCUBE_H__

#include "TileCube.h"

BEGIN(Client)
class CShieldCube final
	: public CTileCube
{
public:
	explicit CShieldCube(_pDevice pGraphicDevice);
	explicit CShieldCube(const CShieldCube& rhs);
	virtual ~CShieldCube() = default;

public:
	virtual HRESULT Render() override;

public:
	virtual HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;

private:
	HRESULT				SetUp_Component();

public:
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	static	CShieldCube*	Create(_pDevice pGraphicDevice);
	virtual void			Free() override;

public:
	// CTileCube을(를) 통해 상속됨
	virtual void Collision(CGameObject * pColObj) override;

};
END
#endif // !__SHIELDCUBE_H__
