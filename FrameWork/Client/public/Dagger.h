#pragma once
#ifndef __CLIENT_DAGGER_H__
#define __CLIENT_DAGGER_H__

#include "CollisionRect.h"

BEGIN(Client)
class CDagger final:
	public CCollisionRect
{
public:
	typedef struct tagDaggerDesc {
		tagDaggerDesc()
		{
			ZeroMemory(this, sizeof(tagDaggerDesc));
		}
		CCollisionRect::DESC tSuperDesc;
		_vec3 vTargetPos;
	} DESC;

private:
	explicit CDagger(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CDagger(const CDagger& rhs);
	virtual ~CDagger() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision(CGameObject* pColObj) override;

public: //SET

public: //GET

private:

	//Tick

private: //Variable
	const _float	m_fMaxDist = 80.f;
	_float			m_fDist = 0.f;

	_vec3			m_vMoveDir = _vec3(0.f, 0.f, 0.f);
public:
	static CDagger* Create(_pDevice pDevice);
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr);
};
END
#endif // !__CLIENT_PLAYER_H__
