#pragma once
#ifndef __CLIENT_TERRAIN_H__
#define __CLIENT_TERRAIN_H__

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferTerrain;
class CTransform;
class CTexture;
END

BEGIN(Client)

class CTerrain final:
	public CGameObject
{
public:
	explicit CTerrain(_pDevice pDevice);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET
	_vec3 Compute_Height(CTransform* pTransform);

private:
	HRESULT SetUp_Component(void* pArg);

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferTerrain*			m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

private:
	_uint						m_iTexNum = 0;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CTerrain* Create(_pDevice pDevice);

	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};

END
#endif // !__CLIENT_TERRAIN_H__
