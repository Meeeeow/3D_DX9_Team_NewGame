#pragma once
#ifndef __CLIENT_DMG_FONT_H__
#define __CLIENT_DMG_FONT_H__

#include "Effect.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)

class CDmgFont final:
	public CEffect
{
public:
	typedef struct tagDesc {

		tagDesc() {
			ZeroMemory(this, sizeof(tagDesc));
			vScale = _vec3(1.f, 1.f, 1.f);
		}
		_vec3 vScale;
		_vec3 vPos;
		_uint iDmg;
		_bool bGuard;
	}DESC;

public:
	explicit CDmgFont(_pDevice pDevice);
	explicit CDmgFont(const CDmgFont& rhs);
	virtual ~CDmgFont() = default;

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

	void SetUp_ListDmg(DESC tDesc);

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferRect*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

private:
	TIME						m_tTimeDie;
	_vec3						m_vOriginalPos;
	list<int>					m_listDmg;

	_float						m_fScaleX = 3.f;
	_float						m_fScaleY = 3.f;

	_vec3						m_vScaleChange = _vec3(1.f, 1.f, 1.f);

	_bool						m_bGuard = false;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CDmgFont* Create(_pDevice pDevice);

	// CEffect을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};

END
#endif // !__CLIENT_TERRAIN_H__
