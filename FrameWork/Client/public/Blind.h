#pragma once
#ifndef __CLIENT_BLIND_H__
#define __CLIENT_BLIND_H__

#include "Effect.h"

class CBlind final : public CEffect
{
public:
	explicit CBlind(_pDevice pGraphicDevice);
	explicit CBlind(const CBlind& rhs);
	virtual ~CBlind() DEFAULT;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			SetUp_Component();

private:
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CVIBufferRect*	m_pVIBuffer = nullptr;

	_float		m_fTime = 1.5f;
	_mat		m_matProject;
	_float		m_fX = 0.f, m_fY = 0.f, m_fSizeX = 0.f, m_fSizeY = 0.f;
public:
	static	CBlind*			Create(_pDevice pGraphicDevice);
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	virtual void			Free() override;
};


#endif // !__CLIENT_BLIND_H__
