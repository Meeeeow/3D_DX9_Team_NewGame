#pragma once
#ifndef __CLIENT_SKYBOX_H__
#define __CLIENT_SKYBOX_H__

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferCube;
class CTransform;
class CTexture;
END

BEGIN(Client)

class CSkyBox final:
	public CGameObject
{
public:
	typedef struct tagSkyBoxDesc{
		_uint iNumTexture = 2;
	}DESC;
public:
	explicit CSkyBox(_pDevice pDevice);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() = default;

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
	HRESULT SetUp_Component();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferCube*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

private:
	DESC						m_tDesc;
public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CSkyBox* Create(_pDevice pDevice);



	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual Engine::_vec3 Get_Length();

};

END
#endif // !__CLIENT_TERRAIN_H__
