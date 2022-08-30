#pragma once
#ifndef __CLIENT_PLAYER_SHIELD_H__
#define __CLIENT_PLAYER_SHIELD_H__

#include "CubeObject.h"

BEGIN(Client)
class CPlayerShield final
	: public CCubeObject
{
public:
	typedef struct tagPlayerShieldDesc {

		tagPlayerShieldDesc() {
			ZeroMemory(this, sizeof(tagPlayerShieldDesc));
		}

		OBJ_INFO tObjInfo;

	}DESC;

public:
	explicit CPlayerShield(_pDevice pGraphicDevice);
	explicit CPlayerShield(const CPlayerShield& rhs);
	virtual ~CPlayerShield() = default;

public:
	virtual HRESULT Render() override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;

public:
	virtual HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;

	void SetUp_RenderState();
	void Release_RenderState();

public:
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	static	CPlayerShield*	Create(_pDevice pGraphicDevice);
	virtual void			Free() override;

};
END
#endif // !__SHIELDCUBE_H__
