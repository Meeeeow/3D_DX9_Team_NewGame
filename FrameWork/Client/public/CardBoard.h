#pragma once
#ifndef __CLIENT_CARD_BOARD_H__
#define __CLIENT_CARD_BOARD_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferRect;
END


#include "GameObject.h"

BEGIN(Client)
class CCardBoard final :public CGameObject
{
public:
	explicit CCardBoard(_pDevice pGraphicDevice);
	explicit CCardBoard(const CCardBoard& rhs);
	virtual ~CCardBoard() DEFAULT;

public:
	virtual HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;
	virtual _int		Tick(_float fTimeDelta) override;
	virtual _int		LateTick(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

public: // Set
	void				SetUp_CardSelectPoision(_uint iCardCategory, _uint iCardNumber);

private:
	HRESULT				SetUp_Component();


private:
	class CTransform*		m_pTransform	= nullptr;
	class CTexture*			m_pTexture		= nullptr;
	class CRenderer*		m_pRenderer		= nullptr;
	class CVIBufferRect*	m_pVIBuffer		= nullptr;
	class CCardSelect*		m_pCardSelect	= nullptr;

	_mat					m_matProject;
	_float					m_fX = 0.f, m_fY = 0.f, m_fSizeX = 0.f, m_fSizeY = 0.f;

public:
	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
	static CCardBoard* Create(_pDevice pGraphicDevice);
};
END
#endif // !__CLIENT_CARD_BOARD_H__