#pragma once
#ifndef __CLIENT_DIALOG_H__
#define __CLIENT_DIALOG_H__

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
END

BEGIN(Client)
class CDialog final :
	public CGameObject
{
public:
	typedef struct tagDialogDesc {
		tagDialogDesc()
		{
			ZeroMemory(this, sizeof(tagDialogDesc));
			vScale = _vec3(0.5f, 1.f, 1.f);
		}

		const _tchar * pText;
		D3DXCOLOR FontColor;
		_vec3 vPos;
		_vec3 vScale;
	} DESC;

public:
	explicit CDialog(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CDialog(const CDialog& rhs);
	virtual ~CDialog() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET


public: //GET


private:
	virtual HRESULT SetUp_Component();

private:
	DESC m_tDesc;

	CRenderer* m_pRenderer = nullptr;

public:
	virtual void Free() override;
	static CDialog* Create(_pDevice pDevice);
	virtual CGameObject * Clone(void *pArg = (void *)nullptr);

	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;

	virtual _vec3 Get_MaxPos() override;

	virtual _vec3 Get_WorldPos() override;

	virtual _vec3 Get_Length() override;

};
END
#endif // !__CLIENT_PLAYER_H__
