#pragma once
#ifndef __CLIENT_CARD_LOST_H__
#define __CLIENT_CARD_LOST_H__


BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferRect;
END

#include "GameObject.h"
BEGIN(Client)
class CCardLost final : public CGameObject
{
public:
	typedef struct tagCardLostDesc
	{
		_uint iCardObjectNum = 0;
		_uint iCardTextureNum = 0;
	}DESC;

public:
	explicit CCardLost(_pDevice pGraphicDevice);
	explicit CCardLost(const CCardLost& rhs);
	virtual ~CCardLost() DEFAULT;

public:
	virtual HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;
	virtual _int		Tick(_float fTimeDelta) override;
	virtual _int		LateTick(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	HRESULT				SetUp_Component(void* pArg);

private:
	class CTransform*		m_pTransform	= nullptr;
	class CTexture*			m_pTexture		= nullptr;
	class CRenderer*		m_pRenderer		= nullptr;
	class CVIBufferRect*	m_pVIBuffer		= nullptr;

	_mat					m_matProject;
	_float					m_fX = 0.f, m_fY = 0.f, m_fSizeX = 0.f, m_fSizeY = 0.f;
	DESC					m_tDesc;

public:
	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CCardLost* Create(_pDevice pGraphicDevice);
	virtual void Free() override;

};
END
#endif // !__CLIENT_CARD_LOST_H__
