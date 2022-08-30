#pragma once
#ifndef __CLIENT_THORN_OBJECT_H__
#define __CLIENT_THORN_OBJECT_H__

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferThorn;
class CTransform;
class CTexture;
class CFrustum;
class CBoxCollider;
END

BEGIN(Client)
class CThornObject abstract :
	public CGameObject
{
public:
	typedef struct tagThornObjectDesc {
		OBJ_INFO					tObjInfo;
		CTransform::TRANSFORM_DESC	tTransformDesc;
		LEVEL						eTextureLevel;
	}DESC;
public:
	explicit CThornObject(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CThornObject(const CThornObject& rhs);
	virtual ~CThornObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	_float	Go_Up(_float fTimeDelta);
	_float	Go_Down(_float fTimeDelta);
	HRESULT SetUp_Speed(_float fSpeed);

public: //GET
	_vec3 Get_TransformState(CTransform::STATE eState);
	_vec3 Get_TransformScale();
	_bool Check_YIsZero();
public:
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;

protected:
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Release_RenderState();

private:
	HRESULT			SetUp_Component(void * pArg);

private:
	CRenderer*		m_pRenderer = nullptr;
	CVIBufferThorn* m_pVIBuffer = nullptr;
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CFrustum*		m_pFrustum = nullptr;
	CBoxCollider*	m_pBoxCollider = nullptr;

private:
	FRAME			m_tFrame;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr)PURE;
	virtual void Collision(CGameObject* pColObj) override;

	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};
END
#endif // !__CLIENT_PLAYER_H__
