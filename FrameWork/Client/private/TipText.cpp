#include "stdafx.h"
#include "..\public\TipText.h"
#include "DialogMgr.h"

CTipText::CTipText(_pDevice pDevice)
	: CTipObject(pDevice)
{
}

CTipText::CTipText(const CTipText & rhs)
	: CTipObject(rhs)
{
}

HRESULT CTipText::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTipText NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTipText::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);
	if (FAILED(__super::NativeConstruct(&tDesc.tSuperDesc)))
		MSG_FAIL("CTipText NativeConstruct - __super");

	_vec3 vOriPos = Get_TransformState(CTransform::STATE_POS);
	Set_TransformState(CTransform::STATE::STATE_POS, _vec3(vOriPos.x + ((float)g_iWinCX * 0.5f), vOriPos.y + ((float)g_iWinCY * 0.5f), 0.f));

	m_dwFontColor = tDesc.FontColor;
	m_pText = tDesc.pText;

	return S_OK;
}

_int CTipText::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CTipText Tick - __super");

	return 0;
}

HRESULT CTipText::Render()
{
	CDialogMgr::Get_Instance()->Draw_Font(m_pText,
		m_dwFontColor, &Get_TransformState(CTransform::STATE_POS), Get_TransformScale());

	return S_OK;
}

void CTipText::Add_Pos(_vec3 vPos)
{
	CTipObject::Sub_Pos(vPos);
}

CGameObject * CTipText::Clone(void * pArg)
{
	return nullptr;
}

CTipText * CTipText::Create(_pDevice pDevice, DESC * tDesc)
{
	CTipText* pInstance = new CTipText(pDevice);

	if (FAILED(pInstance->NativeConstruct(tDesc)))
	{
		Safe_Release(pInstance);
		MSGBOX("CTipText Create Failed");
	}

	return pInstance;
}

void CTipText::Free()
{
	__super::Free();
}