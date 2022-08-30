#pragma once
#ifndef __CLIENT_ORTHO_OBJ_H__
#define __CLIENT_ORTHO_OBJ_H__

#include "GameObject.h"
BEGIN(Engine)
class CTransform;
class CRenderer;
class CVIBufferRect;
class CTexture;
END

class COrthoObj abstract:
	public CGameObject
{
public:

	typedef struct tagOrthoObjDesc {
		tagOrthoObjDesc() {
			ZeroMemory(this, sizeof(tagOrthoObjDesc));
		}

		OBJ_INFO	tObjInfo;
		//텍스쳐 등록된 레벨
		LEVEL		eTextureLevel;
		
	}DESC;

public:
	explicit COrthoObj(_pDevice pDevice);
	explicit COrthoObj(const COrthoObj& rhs);
	virtual ~COrthoObj() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int	Tick(_float fTimeDelta);
	virtual _int	LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public: //SET
	void Set_AlphaRef(_uint iAlphaRef) { m_iAlphaRef = iAlphaRef; }
	void Set_OrthoMatrix();
	//false 시 알파블랜딩
	void Set_AlphaTest(_bool bAlphaTest) { m_bAlphaTest = bAlphaTest; }
	void Set_FrameValue(int iEnd, int iStart = 0, float fDelay = 0.1f, float fTime = 0.f);

	//마지막 프레임 재생끝날때 리턴 true
	_bool Tick_Frame(_float fTimeDelta);

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CTexture*		m_pTexture = nullptr;
	CVIBufferRect*	m_pVIBuffer = nullptr;

private:
	_mat			m_matProject;
	_mat			m_matIdentity;
	_uint			m_iAlphaRef = 150;
	_bool			m_bAlphaTest = true;
	Engine::FRAME	m_tFrame;

private:
	HRESULT SetUp_Component(void* pArg);
	virtual void	SetUp_RenderState();
	virtual void	Release_RenderState();

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr)PURE;
};

#endif // !__CLIENT_ORTHO_OBJ_H__
