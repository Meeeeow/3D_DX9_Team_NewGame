#include "..\public\GameObject.h"
#include "GameInstance.h"
#include "Conversion.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDevice)
	:m_pGraphicDevice(pGraphicDevice)
{
	Safe_AddRef(m_pGraphicDevice);
}

CGameObject::CGameObject(const CGameObject & rhs)
	:m_pGraphicDevice(rhs.m_pGraphicDevice)
{
	Safe_AddRef(m_pGraphicDevice);
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CGameObject::Tick(_float fTimeDelta)
{
	return 0;
}

_int CGameObject::LateTick(_float fTimeDelta)
{
	return 0;
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

_vec3 CGameObject::Get_MaxProjPos()
{
	return _vec3(0,0,0);
}

CComponent * CGameObject::Get_Component(const _tchar * szComponentTag)
{
	auto& iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CTagFinder(szComponentTag));

	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

_bool CGameObject::Check_Component(const _tchar* szComponentTag)
{
	auto& iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CTagFinder(szComponentTag));

	if (iter == m_mapComponent.end())
		return false;

	return true;
}

HRESULT CGameObject::Set_ObjInfo(void * pArg)
{
	return S_OK;
}

HRESULT CGameObject::Set_Pos(_vec3 vPos, CTransform* pTransform)
{
	if (pTransform == nullptr)
		MSG_FAIL("CGameObject::Set_Pos - pTransform == nullptr");

	pTransform->SetUp_State(CTransform::STATE_POS, vPos);

	return S_OK;
}

void CGameObject::Collision(CGameObject* pColObj)
{
	return;
}

HRESULT CGameObject::Tick_Collider(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * szComponentTag, CComponent ** ppOut, void * pArg)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	CComponent* pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (pComponent == nullptr)
		MSG_FAIL("CGameObject Add_Components - pComponent is null");

	auto& iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CTagFinder(szComponentTag));
	if (iter != m_mapComponent.end())
		MSG_FAIL("CGameObject Add_Components - m_mapComponent.end()");

	m_mapComponent.emplace(szComponentTag, pComponent);

	*ppOut = pComponent;
	Safe_AddRef(pComponent);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CGameObject::Free()
{
	Safe_Release(m_pGraphicDevice);

	if (m_mapComponent.size() > 0)
	{
		for (auto& pair : m_mapComponent)
		{
			Safe_Release(pair.second);
		}
		m_mapComponent.clear();
	}

}