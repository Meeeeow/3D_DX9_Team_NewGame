#pragma once
#ifndef __CLIENT_TRUMP_SPADE_H__
#define __CLIENT_TRUMP_SPADE_H__

#include "TrumpCard.h"
BEGIN(Client)
class CTrumpCardSpade final : public CTrumpCard
{
public:
	explicit CTrumpCardSpade(_pDevice pGraphicDevice);
	explicit CTrumpCardSpade(const CTrumpCardSpade& rhs);
	virtual ~CTrumpCardSpade() DEFAULT;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Collision_Effect();
	HRESULT			Collision_NoPlayer();
	HRESULT			SetUp_Component(void* pArg);

public:
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	static CTrumpCardSpade* Create(_pDevice pGraphicDevice);
	virtual void			Free() override;
	virtual void			Collision(CGameObject* pColObj) override;
};
END
#endif // !__CLIENT_TRUMPSPADE_H__
