#include "stdafx.h"
#include "..\public\BossSkeletonStageCubeObserver.h"
#include "EnvironmentMgr.h"
#include "BossSkeletonStageCube.h"

CBossSkeletonStageCubeObserver::CBossSkeletonStageCubeObserver(_pDevice pDevice)
	:CObserver(pDevice)
{
}

CBossSkeletonStageCubeObserver::CBossSkeletonStageCubeObserver(const CBossSkeletonStageCubeObserver & rhs)
	:CObserver(rhs)
{
}

HRESULT CBossSkeletonStageCubeObserver::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBossSkeletonStageCubeObserver::NativeConstruct_Prototype - __super::NativeConstruct_Prototype");

	return S_OK;
}

HRESULT CBossSkeletonStageCubeObserver::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(nullptr)))
		MSG_FAIL("CBossSkeletonStageCubeObserver::NativeConstruct_Prototype - __super::NativeConstruct_Prototype");

	//무조건 큐브 받아와야함
	if (pArg == nullptr)
		MSG_FAIL("CBossSkeletonStageCubeObserver::NativeConstruct_Prototype - pArg == nullptr");

	m_pCube = static_cast<CBossSkeletonStageCube*>(pArg);

	CEnvironmentMgr::Get_Instance()->SubScribe_Observer(this);

	return S_OK;
}

_vec3 CBossSkeletonStageCubeObserver::Get_MinPos()
{
	return _vec3();
}

_vec3 CBossSkeletonStageCubeObserver::Get_MaxPos()
{
	return _vec3();
}

_vec3 CBossSkeletonStageCubeObserver::Get_WorldPos()
{
	return _vec3();
}

_vec3 CBossSkeletonStageCubeObserver::Get_Length()
{
	return _vec3();
}

void CBossSkeletonStageCubeObserver::Notify(void * pMessage /*= nullptr*/)
{
	if (pMessage == nullptr)
		return;

	_ulong dwLong = *(_ulong*)pMessage;

	if (dwLong & ENV_RAIN)
		m_pCube->Set_Env(CEnvironmentMgr::TYPE_RAIN);

	if (dwLong & ENV_MAGMA)
		m_pCube->Set_Env(CEnvironmentMgr::TYPE_MAGMA);
}

CBossSkeletonStageCubeObserver * CBossSkeletonStageCubeObserver::Create(_pDevice pDevice)
{
	CBossSkeletonStageCubeObserver* pInstance = new CBossSkeletonStageCubeObserver(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCubeObserver Create Failed");
	}

	return pInstance;
}

CGameObject * CBossSkeletonStageCubeObserver::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CBossSkeletonStageCubeObserver(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCubeObserver Clone Failed");
	}

	return pInstance;
}

void CBossSkeletonStageCubeObserver::Free()
{
	__super::Free();
}
