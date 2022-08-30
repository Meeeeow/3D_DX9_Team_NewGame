#include "stdafx.h"
#include "..\public\Dialog.h"
#include "GameInstance.h"
#include "DialogMgr.h"

CDialog::CDialog(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CDialog::CDialog(const CDialog & rhs)
	: CGameObject(rhs)
{
	
}

HRESULT CDialog::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CDialog NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CDialog::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CDialog NativeConstruct - __super");

	m_tDesc = *(DESC*)(pArg);

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CDialog::NativeConstruct - SetUp_Component");
	
	return S_OK;
}

_int CDialog::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CDialog Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	return 0;
}

_int CDialog::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CDialog LateTick - __super");

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::GROUP_UI_LATE, this)))
		MSG_MINUSONE("CDialog::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CDialog::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CDialog Render - __super");

	CDialogMgr::Get_Instance()->Draw_Font(m_tDesc.pText,
		m_tDesc.FontColor, &m_tDesc.vPos, m_tDesc.vScale);

	return S_OK;
}

HRESULT CDialog::SetUp_Component()
{
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CDialog::SetUp_Component - pTagPrototypeRenderer");

	return S_OK;
}

void CDialog::Free()
{
	Safe_Release(m_pRenderer);
	__super::Free();
}

CDialog * CDialog::Create(_pDevice pDevice)
{
	CDialog* pInstance = new CDialog(pDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed To CDialog Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDialog::Clone(void * pArg)
{
	CGameObject* pInstance = new CDialog(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed To CDialog Create - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_vec3 CDialog::Get_MinPos()
{
	return _vec3();
}

_vec3 CDialog::Get_MaxPos()
{
	return _vec3();
}

_vec3 CDialog::Get_WorldPos()
{
	return _vec3();
}

_vec3 CDialog::Get_Length()
{
	return _vec3();

}
