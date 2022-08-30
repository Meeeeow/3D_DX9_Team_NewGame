#pragma once
#ifndef __CLIENT_BOX_COLLIDER_H__
#define __CLIENT_BOX_COLLIDER_H__

#include "RectObject.h"

BEGIN(Engine)
class CBoxCollider;
END

BEGIN(Client)
class CCollisionRect abstract:
	public CRectObject
{
public:
	typedef struct tagBossSkeletonStageCircleDesc {
		tagBossSkeletonStageCircleDesc()
		{
			ZeroMemory(this, sizeof(tagBossSkeletonStageCircleDesc));
		}

		CRectObject::DESC tSuperDesc;
	} DESC;

public:
	explicit CCollisionRect(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CCollisionRect(const CCollisionRect& rhs);
	virtual ~CCollisionRect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

public:
	virtual HRESULT Tick_Collider(_float fTimeDelta) override; 

private:
	CBoxCollider* m_pBoxCollider = nullptr;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr)PURE;


	// CRectObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;

};
END
#endif // !__CLIENT_PLAYER_H__
