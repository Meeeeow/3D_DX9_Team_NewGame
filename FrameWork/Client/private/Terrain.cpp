#include "stdafx.h"
#include "..\public\Terrain.h"
#include "GameInstance.h"

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CTerrain::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CTerrain Clone Failed");
	}

	return pInstance;
}

CTerrain * CTerrain::Create(_pDevice pDevice)
{
	CTerrain* pInstance = new CTerrain(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CTerrain Create Failed");
	}

	return pInstance;
}

_vec3 CTerrain::Get_MinPos()
{
	return _vec3();
}

_vec3 CTerrain::Get_MaxPos()
{
	return _vec3();
}

_vec3 CTerrain::Get_WorldPos()
{
	return _vec3();
}

_vec3 CTerrain::Get_Length()
{
	return _vec3();
}


CTerrain::CTerrain(_pDevice pDevice)
	:CGameObject(pDevice)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	:CGameObject(rhs)
{
}

HRESULT CTerrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTerrain NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTerrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTerrain NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CTerrain NativeConstruct - SetUp_Component");

	if (pArg == nullptr)
	{
		//게임 플레이도 로딩으로 바뀌면 삭제
		m_pTransformCom->SetUp_State(CTransform::STATE::STATE_POS, _vec3(0.f, 0.f, 0.f));
		m_pTransformCom->SetUp_Scale(1.f, 1.f, 1.f);
	}

	return S_OK;
}

_int CTerrain::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CTerrain Tick - __super");

	return _int();
}

_int CTerrain::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CTerrain LateTick - __super");

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_NONALPHA, this)))
		MSG_MINUSONE("CTerrain LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CTerrain::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CTerrain Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CTerrain Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CTerrain Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CTerrain Render - m_pTextureCom is null");

	//m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTransformCom->Bind_OnDevice();
	m_pTextureCom->Bind_OnDevice(3);

	m_pVIBufferCom->Render();

	//m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_FILLMODE, D3DFILL_SOLID);


	return S_OK;
}

_vec3 CTerrain::Compute_Height(CTransform * pTransform)
{
	_vec3 vPos = pTransform->Get_State(CTransform::STATE_POS);
	_mat matWorld = *m_pTransformCom->Get_WorldMatrixPtr();
	_mat matWorldInverse;

	D3DXMatrixInverse(&matWorldInverse, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vPos, &vPos, &matWorldInverse);

	_float fY = m_pVIBufferCom->Compute_Height(vPos);
	vPos.y = fY;
	D3DXVec3TransformCoord(&vPos, &vPos, &matWorld);

	return vPos;
}

HRESULT CTerrain::SetUp_Component(void* pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	OBJ_INFO tObjInfo;
	if (pArg != nullptr)
		tObjInfo = *(OBJ_INFO*)pArg;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CTerrain SetUp_Component - Add_Component m_pTransformCom");
	//임시 조건문
	if (pArg != nullptr)
		m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CTerrain SetUp_Component - Add_Component m_pRendererCom");

	if (pArg == nullptr)
	{
		if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("PrototypeVIBufferTerrain"), pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
			MSG_FAIL("CTerrain SetUp_Component - Add_Component PrototypeVIBufferTerrain");
	}
	else
		m_pVIBufferCom = CVIBufferTerrain::Create(m_pGraphicDevice, tObjInfo.iNumVerticesX, tObjInfo.iNumVerticesZ, tObjInfo.fSizeX, tObjInfo.fSizeZ, tObjInfo.fU, tObjInfo.fV);
			
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureTerrain"), TEXT("ComTextureTerrain"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CTerrain SetUp_Component - Add_Component m_pTextureCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
