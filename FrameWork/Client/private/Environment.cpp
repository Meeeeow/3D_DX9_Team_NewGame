#include "stdafx.h"
#include "..\public\Environment.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "InputDevice.h"
#include "PlayerTagMgr.h"

CEnvironment::CEnvironment(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CEnvironment::CEnvironment(const CEnvironment & rhs)
	:CGameObject(rhs)
{
}

HRESULT CEnvironment::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CEnvironment NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CEnvironment::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CEnvironment NativeConstruct - __super");

	return S_OK;
}

_int CEnvironment::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CEnvironment Tick - __super");

	return 0;
}

_int CEnvironment::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CEnvironment LateTick - __super");

	return _int();
}

HRESULT CEnvironment::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CEnvironment Render - __super");

	return S_OK;
}

void CEnvironment::Free()
{
	__super::Free();
}