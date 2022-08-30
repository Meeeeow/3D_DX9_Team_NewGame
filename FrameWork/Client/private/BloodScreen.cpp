#include "stdafx.h"
#include "..\public\OrthoObj.h"
#include "..\public\BloodScreen.h"

CBloodScreen::CBloodScreen(_pDevice pDevice)
	:COrthoObj(pDevice)
{
}

CBloodScreen::CBloodScreen(const CBloodScreen & rhs)
	: COrthoObj(rhs)
{
}

HRESULT CBloodScreen::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBloodScreen::NativeConstruct_Prototype - __super::NativeConstruct_Prototype");

	return S_OK;
}

HRESULT CBloodScreen::NativeConstruct(void * pArg)
{
	COrthoObj::DESC tSuperDesc = ((DESC*)(pArg))->tSuperDesc;

	if (FAILED(__super::NativeConstruct(&tSuperDesc)))
		MSG_FAIL("CBloodScreen::NativeConstruct - __super::NativeConstruct");

	Set_AlphaTest(false);
	Set_FrameValue(11);

	return S_OK;
}

_int CBloodScreen::Tick(_float fTimeDelta)
{
	if (FAILED(__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBloodScreen::Tick - __super::Tick");

	if (Tick_Frame(fTimeDelta))
	{
		Set_Dead();
	}


	if (m_bDead)
		return OBJ_DEAD;



	return _int();
}

_int CBloodScreen::LateTick(_float fTimeDelta)
{
	if (FAILED(__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBloodScreen::LateTick - __super::LateTick");

	return _int();
}

HRESULT CBloodScreen::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBloodScreen::Render - __super::Render");

	return S_OK;
}

_vec3 CBloodScreen::Get_MinPos()
{
	return _vec3();
}

_vec3 CBloodScreen::Get_MaxPos()
{
	return _vec3();
}

_vec3 CBloodScreen::Get_WorldPos()
{
	return _vec3();
}

_vec3 CBloodScreen::Get_Length()
{
	return _vec3();
}

void CBloodScreen::Free()
{
	__super::Free();
}

CBloodScreen * CBloodScreen::Create(_pDevice pDevice)
{
	CBloodScreen* pInstance = new CBloodScreen(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBloodScreen Create Failed");
	}

	return pInstance;
}

CGameObject * CBloodScreen::Clone(void * pArg)
{
	CGameObject* pInstance = new CBloodScreen(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBloodScreen Clone Failed");
	}

	return pInstance;
}
