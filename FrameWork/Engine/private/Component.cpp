#include "..\public\Component.h"

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDevice)
	:m_pGraphicDevice(pGraphicDevice), m_bClone(false)
{
	Safe_AddRef(m_pGraphicDevice);
}

CComponent::CComponent(const CComponent & rhs)
	:m_pGraphicDevice(rhs.m_pGraphicDevice), m_bClone(true)
{
	Safe_AddRef(m_pGraphicDevice);
}

HRESULT CComponent::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CComponent::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	Safe_Release(m_pGraphicDevice);
}
