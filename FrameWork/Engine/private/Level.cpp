#include "..\public\Level.h"

CLevel::CLevel(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLevel::NativeConstruct(_uint iLevelID)
{
	m_iLevelID = iLevelID;
	return S_OK;
}

_int CLevel::Tick(_float fTimeDelta)
{
	return _int();
}

HRESULT CLevel::Render()
{
	return S_OK;
}

void CLevel::Free()
{
	Safe_Release(m_pGraphic_Device);
}
