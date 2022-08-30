#pragma once
#ifndef __CLIENT_TAG_GAUGE_H__
#define __CLIENT_TAG_GAUGE_H__

#include "Effect.h"
#include "Player.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)

class CTagGauge final :
	public CEffect
{
public:
	typedef struct tagTagGaugeDesc {
		_vec3 vPos;
		_vec3 vScale;
		CPlayer::TYPE eType;
	} DESC;
public:
	explicit CTagGauge(_pDevice pDevice);
	explicit CTagGauge(const CTagGauge& rhs);
	virtual ~CTagGauge() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	void Change_Player(CPlayer::TYPE eType);

public: //GET

private:
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();
	HRESULT SetUp_Component(void* pArg);

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferRect*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

private:
	_mat						m_matProject;

	_bool						m_bRender = false;
	CPlayer*					m_pPlayer = nullptr;
	CPlayer::TYPE				m_eType = CPlayer::TYPE_END;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CTagGauge* Create(_pDevice pDevice);

	// CEffect을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};

END
#endif // !__CLIENT_TERRAIN_H__
