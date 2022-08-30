#include "stdafx.h"
#include "..\public\Item.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "InputDevice.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "PoisonCubeHit.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CItem::CItem(const CItem & rhs)
	: CGameObject(rhs)
{
	
}

HRESULT CItem::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CItem NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CItem::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CItem NativeConstruct - __super");

	return S_OK;
}

_int CItem::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CItem Tick - __super");

	return 0;
}

_int CItem::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CItem LateTick - __super");

	return _int();
}

HRESULT CItem::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CItem Render - __super");

	return S_OK;
}

HRESULT CItem::SetUp_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CItem SetUp_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CItem::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CItem Release_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

void CItem::Collision_Terrain(_float fTimeDelta, CGameInstance* pInstance, CTransform* pTransform)
{
	//터레인 여러개 되면 어떻게 할 것인가
	CTerrain* pTerrain = (CTerrain*)pInstance->Get_GameObjPtr(pInstance->Get_CurrentLevel(), L"LayerTerrain", 0);

	if (pTerrain == nullptr)
		return;

	_vec3 vTerrainPos = pTerrain->Compute_Height(pTransform);
	pTransform->SetUp_State(CTransform::STATE_POS, vTerrainPos);
}

HRESULT CItem::BillBoarding(CTransform * pTransform)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	_mat matCamWorld = pGameInstance->Get_CamWorld();
	_vec3 vEffectScale = pTransform->Get_Scale();

	pTransform->SetUp_State(CTransform::STATE_RIGHT, (*(_vec3*)(&matCamWorld.m[0][0])) * vEffectScale.x);
	pTransform->SetUp_State(CTransform::STATE_LOOK, (*(_vec3*)(&matCamWorld.m[2][0])) * vEffectScale.z);
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CItem::Free()
{
	__super::Free();
}

_vec3 CItem::Get_MinPos()
{
	return _vec3();
}

_vec3 CItem::Get_MaxPos()
{
	return _vec3();
}

_vec3 CItem::Get_WorldPos()
{
	return _vec3();
}

_vec3 CItem::Get_Length()
{
	return _vec3();
}
