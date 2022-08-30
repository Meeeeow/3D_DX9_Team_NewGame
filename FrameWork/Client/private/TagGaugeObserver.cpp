#include "stdafx.h"
#include "..\public\TagGaugeObserver.h"
#include "EnvironmentMgr.h"
#include "PlayerTagMgr.h"
#include "TagGauge.h"
#include "Player.h"

CTagGaugeObserver::CTagGaugeObserver(_pDevice pDevice)
	:CObserver(pDevice)
{
}

CTagGaugeObserver::CTagGaugeObserver(const CTagGaugeObserver & rhs)
	:CObserver(rhs)
{
}

HRESULT CTagGaugeObserver::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTagGaugeObserver::NativeConstruct_Prototype - __super::NativeConstruct_Prototype");

	return S_OK;
}

HRESULT CTagGaugeObserver::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(nullptr)))
		MSG_FAIL("CTagGaugeObserver::NativeConstruct_Prototype - __super::NativeConstruct_Prototype");

	//무조건 게이지 받아와야함
	if (pArg == nullptr)
		MSG_FAIL("CTagGaugeObserver::NativeConstruct_Prototype - pArg == nullptr");

	m_pTagGauge = static_cast<CTagGauge*>(pArg);

	return S_OK;
}

_vec3 CTagGaugeObserver::Get_MinPos()
{
	return _vec3();
}

_vec3 CTagGaugeObserver::Get_MaxPos()
{
	return _vec3();
}

_vec3 CTagGaugeObserver::Get_WorldPos()
{
	return _vec3();
}

_vec3 CTagGaugeObserver::Get_Length()
{
	return _vec3();
}

void CTagGaugeObserver::Notify(void * pMessage /*= nullptr*/)
{
	if (pMessage == nullptr)
		return;

	CPlayer::TYPE eType = *(CPlayer::TYPE*)(pMessage);
	m_pTagGauge->Change_Player(eType);
}

CTagGaugeObserver * CTagGaugeObserver::Create(_pDevice pDevice)
{
	CTagGaugeObserver* pInstance = new CTagGaugeObserver(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CTagGaugeObserver Create Failed");
	}

	return pInstance;
}

CGameObject * CTagGaugeObserver::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CTagGaugeObserver(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CTagGaugeObserver Clone Failed");
	}

	return pInstance;
}

void CTagGaugeObserver::Free()
{
	__super::Free();
}
