#pragma once
#ifndef __CLIENT_BOSS_SKELETON_STAGE_CIRCLE_H__
#define __CLIENT_BOSS_SKELETON_STAGE_CIRCLE_H__

#include "Effect.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CBossSkeletonStageCircle final:
	public CEffect
{
public:
	typedef struct tagBossSkeletonStageCircleDesc {
		OBJ_INFO	tObjInfo;
		LEVEL		eTextureLevel;
	} DESC;

private:
	explicit CBossSkeletonStageCircle(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CBossSkeletonStageCircle(const CBossSkeletonStageCircle& rhs);
	virtual ~CBossSkeletonStageCircle() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

private:
	//SetUp
	HRESULT SetUp_Component(void *pArg);

	//Render
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();

	//Tick

private: //Component
	class CRenderer* m_pRenderer = nullptr;
	class CVIBufferRectColor* m_pVIBuffer = nullptr;
	class CTransform* m_pTransform = nullptr;
	class CTexture* m_pTexture = nullptr;

private: //Variable
	_vec3			m_vScale = _vec3(1.f, 1.f, 1.f);

public:
	static CBossSkeletonStageCircle* Create(_pDevice pDevice);
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr);

};
END
#endif // !__CLIENT_PLAYER_H__
