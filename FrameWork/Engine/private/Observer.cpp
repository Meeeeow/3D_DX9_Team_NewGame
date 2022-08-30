#include "..\public\Observer.h"
#include "GameInstance.h"


CObserver::CObserver(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CObserver::CObserver(const CObserver & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObserver::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CObserver::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CObserver::Tick(_float fTimeDelta)
{
	return _int();
}

_int CObserver::LateTick(_float fTimeDelta)
{
	return _int();
}

HRESULT CObserver::Render()
{
	return S_OK;
}

void CObserver::Free()
{
	__super::Free();
}
