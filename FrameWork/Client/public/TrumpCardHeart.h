#pragma once
#ifndef __CLIENT_TRUMP_HEART_H__
#define __CLIENT_TRUMP_HEART_H__


#include "TrumpCard.h"
BEGIN(Client)
class CTrumpCardHeart final : public CTrumpCard
{
public:
	explicit CTrumpCardHeart(_pDevice pGraphicDevice);
	explicit CTrumpCardHeart(const CTrumpCardHeart& rhs);
	virtual ~CTrumpCardHeart() DEFAULT;

public:
	virtual HRESULT			NativeConstruct_Prototype() override;
	virtual HRESULT			NativeConstruct(void* pArg) override;
	virtual _int			Tick(_float fTimeDelta);
	virtual _int			LateTick(_float fTimeDelta);
	virtual HRESULT			Render() override;

private:
	HRESULT					Collision_Effect(class CPlayer* pPlayerObject);
	HRESULT					Collision_NoPlayer();
	HRESULT					SetUp_Component(void* pArg);

public:
	// CTrumpCard을(를) 통해 상속됨
	static CTrumpCardHeart*	Create(_pDevice pGraphicDevice);
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	virtual void			Free() override;
	virtual void			Collision(CGameObject* pColObj) override;
	
};
END
#endif // !__CLIENT_TRUMP_HEART_H__
