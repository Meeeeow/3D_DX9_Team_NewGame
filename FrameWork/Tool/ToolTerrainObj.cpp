#include "stdafx.h"
#include "ToolTerrainObj.h"
#include "GameInstance.h"

void CToolTerrainObj::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CToolTerrainObj * CToolTerrainObj::Create(_pDevice pDevice)
{
	CToolTerrainObj* pInstance = new CToolTerrainObj(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CToolTerrain Create Failed");
	}

	return pInstance;
}

_vec3 CToolTerrainObj::Get_MinPos()
{
	return _vec3();
}

_vec3 CToolTerrainObj::Get_MaxPos()
{
	return _vec3();
}

_vec3 CToolTerrainObj::Get_WorldPos()
{
	return _vec3();
}

_vec3 CToolTerrainObj::Get_Length()
{
	return _vec3();
}

CToolTerrainObj::CToolTerrainObj(_pDevice pDevice)
	:CToolObj(pDevice)
{
}

HRESULT CToolTerrainObj::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CToolTerrain NativeConstruct_Prototype - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CToolTerrain NativeConstruct - SetUp_Component");

	m_pTransformCom->SetUp_State(CTransform::STATE::STATE_POS, _vec3(0.f, 0.f, 0.f));
	m_pTransformCom->SetUp_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

HRESULT CToolTerrainObj::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CToolTerrain NativeConstruct - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CToolTerrain NativeConstruct - SetUp_Component");

	m_pTransformCom->SetUp_State(CTransform::STATE::STATE_POS, _vec3(0.f, 0.f, 0.f));
	m_pTransformCom->SetUp_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

_int CToolTerrainObj::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CToolTerrain Tick - __super");

	return _int();
}

_int CToolTerrainObj::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CToolTerrain LateTick - __super");

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_NONALPHA, this)))
		MSG_MINUSONE("CToolTerrain LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CToolTerrainObj::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CToolTerrain Render - __super");

	if (m_pTransformCom == nullptr)
		MSG_FAIL("CToolTerrain Render - m_pTransformCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CToolTerrain Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CToolTerrain Render - m_pTextureCom is null");

	m_pTransformCom->Bind_OnDevice();
	m_pTextureCom->Bind_OnDevice(m_iIndex);

	if (m_bRenderWireFrame)
		m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pVIBufferCom->Render();

	if (m_bRenderWireFrame)
		m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CToolTerrainObj::Set_TerrainBuffer(OBJ_INFO* pObjInfo)
{
	if (m_pVIBufferCom != nullptr)
		Safe_Release(m_pVIBufferCom);

	Set_ObjInfo(pObjInfo);

	m_pTransformCom->Set_Matrix(pObjInfo->matWorld);

	//하이트맵 없을때
	if (0 == lstrcmp(pObjInfo->szHeightMap, L"None"))
		m_pVIBufferCom = CVIBufferTerrain::Create(m_pGraphicDevice, pObjInfo->iNumVerticesX, pObjInfo->iNumVerticesZ
		,pObjInfo->fSizeX, pObjInfo->fSizeZ, pObjInfo->fU, pObjInfo->fV);
	else
	{
		//하이트맵 패스로 바꿔야함
		CString wstrHeightMapPath = L"../bin/Resource/Texture/Terrain/";
		wstrHeightMapPath += pObjInfo->szHeightMap;
		wstrHeightMapPath += L".bmp";
		m_pVIBufferCom = CVIBufferTerrain::Create(m_pGraphicDevice, wstrHeightMapPath.GetString());
	}

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CToolTerrainObj Set_TerrainBuffer - m_pVIBufferCom");

	return S_OK;
}

HRESULT CToolTerrainObj::Set_Matrix(_mat matWorld)
{
	__super::Set_Matrix(matWorld);

	m_pTransformCom->Set_Matrix(matWorld);
	return S_OK;
}

_vec3 CToolTerrainObj::Compute_Height(CTransform * pTransform)
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

HRESULT CToolTerrainObj::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CToolTerrain SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CToolTerrain SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeTextureTerrain", TEXT("ComTextureTerrain"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CToolTerrain SetUp_Component - Add_Component m_pTextureCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
