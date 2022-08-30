#pragma once
#ifndef __CLIENT_TRUMP_CLOVER_H__
#define __CLIENT_TRUMP_CLOVER_H__
#include "TrumpCard.h"


BEGIN(Client)
class CTrumpCardClover final : public CTrumpCard
{
public:

public:
	explicit CTrumpCardClover(_pDevice pGraphicDevice);
	explicit CTrumpCardClover(const CTrumpCardClover& rhs);
	virtual ~CTrumpCardClover() DEFAULT;

public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_bool			m_bDead = false;

private:
	HRESULT			SetUp_Component(void* pArg);
	virtual void	Collision(CGameObject* pCollisionObject) override;
	HRESULT			Collision_Effect(class CPlayer* pPlayerObject);
	HRESULT			Collision_NoPlayer();

public:
	virtual CGameObject*		Clone(void * pArg = nullptr) override;
	static	CTrumpCardClover*	Create(_pDevice pGraphicDevice);
	virtual void				Free() override;
};

END
#endif // !__CLIENT_TRUMP_CLOVER_H__
