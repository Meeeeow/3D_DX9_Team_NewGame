#include "stdafx.h"
#include "..\public\NonControlObserver.h"
#include "EnvironmentMgr.h"
#include "PlayerTagMgr.h"
#include "TagGauge.h"
#include "Player.h"
#include "NonControl.h"

CNonControlObserver::CNonControlObserver(_pDevice pDevice)
	:CObserver(pDevice)
{
}

CNonControlObserver::CNonControlObserver(const CNonControlObserver & rhs)
	:CObserver(rhs)
{
}

HRESULT CNonControlObserver::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CNonControlObserver::NativeConstruct_Prototype - __super::NativeConstruct_Prototype");

	return S_OK;
}

HRESULT CNonControlObserver::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(nullptr)))
		MSG_FAIL("CNonControlObserver::NativeConstruct_Prototype - __super::NativeConstruct_Prototype");

	//무조건 게이지 받아와야함
	if (pArg == nullptr)
		MSG_FAIL("CNonControlObserver::NativeConstruct_Prototype - pArg == nullptr");

	m_pNonControl = static_cast<CNonControl*>(pArg);

	return S_OK;
}

_vec3 CNonControlObserver::Get_MinPos()
{
	return _vec3();
}

_vec3 CNonControlObserver::Get_MaxPos()
{
	return _vec3();
}

_vec3 CNonControlObserver::Get_WorldPos()
{
	return _vec3();
}

_vec3 CNonControlObserver::Get_Length()
{
	return _vec3();
}

void CNonControlObserver::Notify(void * pMessage /*= nullptr*/)
{
	if (pMessage == nullptr)
		return;

	CPlayer::TYPE eType = *(CPlayer::TYPE*)(pMessage);

	m_pNonControl->Change_Player(eType);
}

CNonControlObserver * CNonControlObserver::Create(_pDevice pDevice)
{
	CNonControlObserver* pInstance = new CNonControlObserver(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CNonControlObserver Create Failed");
	}

	return pInstance;
}

CGameObject * CNonControlObserver::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CNonControlObserver(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CNonControlObserver Clone Failed");
	}

	return pInstance;
}

void CNonControlObserver::Free()
{
	__super::Free();
}
