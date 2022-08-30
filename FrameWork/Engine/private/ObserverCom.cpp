#include "GameInstance.h"
#include "..\public\ObserverCom.h"


CObserverCom::CObserverCom(_pDevice pGraphicDevice)
	: CComponent(pGraphicDevice)
{
}

CObserverCom::CObserverCom(const CObserverCom & rhs)
	: CComponent(rhs)
{
}

HRESULT CObserverCom::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CObserverCom::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CObserverCom::Tick(_float fTimeDelta)
{
	return _int();
}

_int CObserverCom::LateTick(_float fTimeDelta)
{
	return _int();
}

HRESULT CObserverCom::Render()
{
	return S_OK;
}

void CObserverCom::Free()
{
	__super::Free();
}
