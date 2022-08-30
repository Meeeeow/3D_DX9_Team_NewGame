#pragma once
#ifndef __CLIENT_BACK_GROUND_H__
#define __CLIENT_BACK_GROUND_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)

class CBackGround final : public CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Component();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferRect*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

private:
	_mat						m_matProject;
	_float						m_fX = 0.f, m_fY = 0.f, m_fSizeX = 0.f, m_fSizeY = 0.f;

public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual void Free() override;	
	virtual CGameObject * Clone(void *pArg = (void *)nullptr);

	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};

END
#endif // !__CLIENT_BACK_GROUND_H__
