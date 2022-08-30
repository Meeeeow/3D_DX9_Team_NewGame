#pragma once
#ifndef __CLIENT_BOSS_SKELETON_MAGIC_CIRCLE_IN_H__
#define __CLIENT_BOSS_SKELETON_MAGIC_CIRCLE_IN_H__

#include "Effect.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRectColor;
class CTransform;
class CTexture;
END

BEGIN(Client)

class CBossSkeletonMagicCircleIn final:
	public CEffect
{
public:
	enum TYPE { TYPE_RED, TYPE_GREEN, TYPE_BLUE, TYPE_END };
	typedef struct tagDesc {
		_vec3 vPos;
		_vec3 vScale;
		TYPE eType;
	}DESC;

public:
	explicit CBossSkeletonMagicCircleIn(_pDevice pDevice);
	explicit CBossSkeletonMagicCircleIn(const CBossSkeletonMagicCircleIn& rhs);
	virtual ~CBossSkeletonMagicCircleIn() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

private:
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();
	HRESULT SetUp_Component(void* pArg);

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferRectColor*			m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

private:
	D3DXCOLOR					m_dwColor;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CBossSkeletonMagicCircleIn* Create(_pDevice pDevice);

	// CEffect을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};

END
#endif // !__CLIENT_TERRAIN_H__
