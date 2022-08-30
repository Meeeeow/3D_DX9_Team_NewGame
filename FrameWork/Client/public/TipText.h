#pragma once
#ifndef __CLIENT_TIP_TEXT_H__
#define __CLIENT_TIP_TEXT_H__


BEGIN(Engine)
END

#include "TipObject.h"

BEGIN(Client)
class CTipText :
	public CTipObject
{
public:
	typedef struct tagTipTextDesc {
		tagTipTextDesc()
		{
			ZeroMemory(this, sizeof(tagTipTextDesc));
		}

		CTipObject::DESC tSuperDesc;
		const _tchar * pText;
		D3DXCOLOR FontColor;

	}DESC;

public:
	explicit CTipText(_pDevice pDevice);
	explicit CTipText(const CTipText& rhs);
	virtual ~CTipText() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	virtual void Add_Pos(_vec3 vPos) override;

public: //GET

private:
	const _tchar*	m_pText = nullptr;
	D3DXCOLOR		m_dwFontColor;

public:
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	static CTipText* Create(_pDevice pDevice, DESC* tDesc);
	virtual void Free() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
