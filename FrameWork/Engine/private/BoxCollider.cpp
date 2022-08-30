#include "..\public\BoxCollider.h"
#include "GameObject.h"
#include "InputDevice.h"
#include "ColliderRenderManager.h"

CBoxCollider::CBoxCollider(_pDevice pGraphicDevice)
	: CComponent(pGraphicDevice)
{

}

CBoxCollider::CBoxCollider(const CBoxCollider & rhs)
	: CComponent(rhs), m_pIB(rhs.m_pIB), m_pVB(rhs.m_pVB)
{

}

HRESULT CBoxCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - __super");

	m_iNumVertices = 8;

	m_iStride = sizeof(VTXCUBECOL);
	m_dwFVF = D3DFVF_XYZ;

	if (FAILED(CreateVertexBuffer()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - CreateVertexBuffer");

	VTXCUBECOL*			pVertices = nullptr;

	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	pVertices[0].vPos = _vec3(-0.5f, 0.5f, -0.5f);
	pVertices[1].vPos = _vec3(0.5f, 0.5f, -0.5f);
	pVertices[2].vPos = _vec3(0.5f, -0.5f, -0.5f);
	pVertices[3].vPos = _vec3(-0.5f, -0.5f, -0.5f);
	pVertices[4].vPos = _vec3(-0.5f, 0.5f, 0.5f);
	pVertices[5].vPos = _vec3(0.5f, 0.5f, 0.5f);
	pVertices[6].vPos = _vec3(0.5f, -0.5f, 0.5f);
	pVertices[7].vPos = _vec3(-0.5f, -0.5f, 0.5f);

	m_vMinWorldPos = pVertices[3].vPos;
	m_vMaxWorldPos = pVertices[5].vPos;
	for (_uint i = 0; i < m_iNumVertices; ++i)
		pVertices[i].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_pVB->Unlock();

	m_iNumPrimitive = 12;
	m_iIndicesSize = sizeof(INDICES_LINE);
	m_eFormat = D3DFMT_INDEX16;
	m_ePrimitiveType = D3DPT_LINELIST;

	if (FAILED(CreateIndexBuffer()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - CreateIndexBuffer");

	INDICES_LINE*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0; pIndices[0]._2 = 1;
	pIndices[1]._1 = 1; pIndices[1]._2 = 2;
	pIndices[2]._1 = 2; pIndices[2]._2 = 3;
	pIndices[3]._1 = 3; pIndices[3]._2 = 0;
	pIndices[4]._1 = 4; pIndices[4]._2 = 5;
	pIndices[5]._1 = 5; pIndices[5]._2 = 6;
	pIndices[6]._1 = 6; pIndices[6]._2 = 7;
	pIndices[7]._1 = 7; pIndices[7]._2 = 4;
	pIndices[8]._1 = 0; pIndices[8]._2 = 4;
	pIndices[9]._1 = 3; pIndices[9]._2 = 7;
	pIndices[10]._1 = 1; pIndices[10]._2 = 5;
	pIndices[11]._1 = 2; pIndices[11]._2 = 6;

	m_pIB->Unlock();

	if (FAILED(Create_VertexPosArr()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - Create_VertexPosArr");

	return S_OK;
}

HRESULT CBoxCollider::NativeConstruct_Prototype(void * pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - __super");

	m_iNumVertices = 8;

	m_iStride = sizeof(VTXCUBECOL);
	m_dwFVF = D3DFVF_XYZ;

	if (FAILED(CreateVertexBuffer()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - CreateVertexBuffer");

	VTXCUBECOL*			pVertices = nullptr;

	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);


	_vec3 vSize;
	vSize = *(_vec3*)pArg;
	_float fSizeX = vSize.x * 0.5f;
	_float fSizeY = vSize.y * 0.5f;
	_float fSizeZ = vSize.z * 0.5f;

	pVertices[0].vPos = _vec3(-fSizeX, fSizeY, -fSizeZ);
	pVertices[1].vPos = _vec3(fSizeX, fSizeY, -fSizeZ);
	pVertices[2].vPos = _vec3(fSizeX, -fSizeY, -fSizeZ);
	pVertices[3].vPos = _vec3(-fSizeX, -fSizeY, -fSizeZ);
	pVertices[4].vPos = _vec3(-fSizeX, fSizeY, fSizeZ);
	pVertices[5].vPos = _vec3(fSizeX, fSizeY, fSizeZ);
	pVertices[6].vPos = _vec3(fSizeX, -fSizeY, fSizeZ);
	pVertices[7].vPos = _vec3(-fSizeX, -fSizeY, fSizeZ);

	m_vMinWorldPos = pVertices[3].vPos;
	m_vMaxWorldPos = pVertices[5].vPos;
	for (_uint i = 0; i < m_iNumVertices; ++i)
		pVertices[i].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_pVB->Unlock();

	m_iNumPrimitive = 12;
	m_iIndicesSize = sizeof(INDICES_LINE);
	m_eFormat = D3DFMT_INDEX16;
	m_ePrimitiveType = D3DPT_LINELIST;

	if (FAILED(CreateIndexBuffer()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - CreateIndexBuffer");

	INDICES_LINE*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0; pIndices[0]._2 = 1;
	pIndices[1]._1 = 1; pIndices[1]._2 = 2;
	pIndices[2]._1 = 2; pIndices[2]._2 = 3;
	pIndices[3]._1 = 3; pIndices[3]._2 = 0;
	pIndices[4]._1 = 4; pIndices[4]._2 = 5;
	pIndices[5]._1 = 5; pIndices[5]._2 = 6;
	pIndices[6]._1 = 6; pIndices[6]._2 = 7;
	pIndices[7]._1 = 7; pIndices[7]._2 = 4;
	pIndices[8]._1 = 0; pIndices[8]._2 = 4;
	pIndices[9]._1 = 3; pIndices[9]._2 = 7;
	pIndices[10]._1 = 1; pIndices[10]._2 = 5;
	pIndices[11]._1 = 2; pIndices[11]._2 = 6;

	m_pIB->Unlock();

	if (FAILED(Create_VertexPosArr()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - Create_VertexPosArr");

	return S_OK;
}

HRESULT CBoxCollider::NativeConstruct_Prototype(_vec3 vColliderCenter, _vec3 vColliderSize, DWORD dwColors)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - __super");

	m_vColliderCenter = vColliderCenter;

	m_iNumVertices = 8;

	m_iStride = sizeof(VTXCUBECOL);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	if (FAILED(CreateVertexBuffer()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - CreateVertexBuffer");

	VTXCUBECOL*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	_vec3 vSize = {};
	vSize.x = vColliderSize.x / 2.f;
	vSize.y = vColliderSize.y / 2.f;
	vSize.z = vColliderSize.z / 2.f;


	pVertices[0].vPos = _vec3(vColliderCenter.x + (-vSize.x),vColliderCenter.y + (vSize.y) ,vColliderCenter.z + (-vSize.z));
	pVertices[1].vPos = _vec3(vColliderCenter.x + (vSize.x) ,vColliderCenter.y + (vSize.y) ,vColliderCenter.z + (-vSize.z));
	pVertices[2].vPos = _vec3(vColliderCenter.x + (vSize.x) ,vColliderCenter.y + (-vSize.y),vColliderCenter.z + (-vSize.z));
	pVertices[3].vPos = _vec3(vColliderCenter.x + (-vSize.x),vColliderCenter.y + (-vSize.y),vColliderCenter.z + (-vSize.z));
	pVertices[4].vPos = _vec3(vColliderCenter.x + (-vSize.x),vColliderCenter.y + (vSize.y) ,vColliderCenter.z + (vSize.z) );
	pVertices[5].vPos = _vec3(vColliderCenter.x + (vSize.x) ,vColliderCenter.y + (vSize.y) ,vColliderCenter.z + (vSize.z) );
	pVertices[6].vPos = _vec3(vColliderCenter.x + (vSize.x) ,vColliderCenter.y + (-vSize.y),vColliderCenter.z + (vSize.z) );
	pVertices[7].vPos = _vec3(vColliderCenter.x + (-vSize.x),vColliderCenter.y + (-vSize.y),vColliderCenter.z + (vSize.z) );
	for (_uint i = 0; i < m_iNumVertices; ++i)
		pVertices[i].dwColor = dwColors;

	m_vMinWorldPos = pVertices[3].vPos;
	m_vMaxWorldPos = pVertices[5].vPos;
	m_vMinProjPos = pVertices[3].vPos;
	m_vMaxProjPos = pVertices[1].vPos;

	m_pVB->Unlock();

	m_iNumPrimitive = 12;
	m_iIndicesSize = sizeof(INDICES_LINE);
	m_eFormat = D3DFMT_INDEX16;
	m_ePrimitiveType = D3DPT_LINELIST;

	if (FAILED(CreateIndexBuffer()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - CreateIndexBuffer");

	INDICES_LINE*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0; pIndices[0]._2 = 1;
	pIndices[1]._1 = 1; pIndices[1]._2 = 2;
	pIndices[2]._1 = 2; pIndices[2]._2 = 3;
	pIndices[3]._1 = 3; pIndices[3]._2 = 0;
	pIndices[4]._1 = 4; pIndices[4]._2 = 5;
	pIndices[5]._1 = 5; pIndices[5]._2 = 6;
	pIndices[6]._1 = 6; pIndices[6]._2 = 7;
	pIndices[7]._1 = 7; pIndices[7]._2 = 4;
	pIndices[8]._1 = 0; pIndices[8]._2 = 4;
	pIndices[9]._1 = 3; pIndices[9]._2 = 7;
	pIndices[10]._1 = 1; pIndices[10]._2 = 5;
	pIndices[11]._1 = 2; pIndices[11]._2 = 6;

	m_pIB->Unlock();

	if (FAILED(Create_VertexPosArr()))
		MSG_FAIL("CBoxCollider NativeConstruct_Prototype - Create_VertexPosArr");

	return S_OK;
}

HRESULT CBoxCollider::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBoxCollider NativeConstruct - __super");

	return S_OK;
}

_int CBoxCollider::Tick(_float fTimeDelta)
{
	// 받아온 월드의 행렬을 받아와 자신의 행렬에 재정의
	Set_DefaultRotation();
	// 새로 저장한 월드의 행렬과 VB의 좌표를 기반으로 m_vWorldPosArr 배열에 월드 좌표를 갱신해줌 
	Update_VerticesArrToWorld();
	// Update_VerticesArrToWorld를 기반으로 새로운 D3DXPLANE을
	// FRONT / BACK / LEFT / RIGHT 순서로 생성
	Create_Plane();
	return _int();
}

_int CBoxCollider::LateTick(_float fTimeDelta)
{
	return _int();
}

HRESULT CBoxCollider::Render()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBoxCollider Render - m_pGraphicDevice is null");
	m_pGraphicDevice->SetTransform(D3DTS_WORLD, &m_matColliderWorld);
	if (CColliderRenderManager::Get_Instance()->Get_ColliderRenderState())
	{
		m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pGraphicDevice->SetTexture(0, nullptr);
		m_pGraphicDevice->SetStreamSource(0, m_pVB, 0, m_iStride);
		m_pGraphicDevice->SetFVF(m_dwFVF);
		m_pGraphicDevice->SetIndices(m_pIB);
		m_pGraphicDevice->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);
		m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	return S_OK;
}

HRESULT CBoxCollider::Tick_Mat(_mat matWorld)
{
	memcpy(&m_matColliderWorld, matWorld, sizeof(_mat));
	return S_OK;
}

HRESULT CBoxCollider::Set_DefaultRotation()
{

	_vec3 vRight = { 1.f , 0.f , 0.f };
	_vec3 vUp	 = { 0.f , 1.f , 0.f };
	_vec3 vLook	 = { 0.f , 0.f , 1.f };

	vRight	*= D3DXVec3Length((_vec3*)(&m_matColliderWorld.m[0][0]));
	vUp		*= D3DXVec3Length((_vec3*)(&m_matColliderWorld.m[1][0]));
	vLook	*= D3DXVec3Length((_vec3*)(&m_matColliderWorld.m[2][0]));

	memcpy(&m_matColliderWorld.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&m_matColliderWorld.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&m_matColliderWorld.m[2][0], &vLook, sizeof(_vec3));

	return S_OK;
}

void CBoxCollider::Update_VerticesArrToWorld()
{
	for (int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vWorldPosArr[i], &m_vVerticesPosArr[i], &m_matColliderWorld);

	m_vMinWorldPos = m_vWorldPosArr[3];
	m_vMaxWorldPos = m_vWorldPosArr[5];
	m_vMinProjPos = m_vWorldPosArr[3];
	m_vMaxProjPos = m_vWorldPosArr[1];

}

HRESULT CBoxCollider::CreateVertexBuffer()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBoxCollider CreateVertexBuffer - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->CreateVertexBuffer(m_iNumVertices * m_iStride, 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED
		, &m_pVB, nullptr)))
		MSG_FAIL("CBoxCollider CreateVertexBuffer - CreateVertexBuffer");

	return S_OK;
}

HRESULT CBoxCollider::CreateIndexBuffer()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBoxCollider CreateIndexBuffer - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->CreateIndexBuffer(m_iNumPrimitive * m_iIndicesSize, 0, m_eFormat, D3DPOOL_MANAGED
		, &m_pIB, nullptr)))
		MSG_FAIL("CBoxCollider CreateVertexBuffer - CreateVertexBuffer");

	return S_OK;
}

HRESULT CBoxCollider::Create_VertexPosArr()
{
	void* pVertices = nullptr;
	m_pVB->Lock(0, 0, &pVertices, 0);

	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_vVerticesPosArr[i] = *(_vec3*)((_byte*)pVertices + i * m_iStride);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CBoxCollider::Create_Plane()
{
	D3DXPlaneFromPoints(&m_planeDir[PLANE_FRONT] , &m_vWorldPosArr[0], &m_vWorldPosArr[1], &m_vWorldPosArr[2]);
	D3DXPlaneFromPoints(&m_planeDir[PLANE_BACK]  , &m_vWorldPosArr[5], &m_vWorldPosArr[4], &m_vWorldPosArr[7]);
	D3DXPlaneFromPoints(&m_planeDir[PLANE_LEFT]  , &m_vWorldPosArr[4], &m_vWorldPosArr[0], &m_vWorldPosArr[3]);
	D3DXPlaneFromPoints(&m_planeDir[PLANE_RIGHT] , &m_vWorldPosArr[1], &m_vWorldPosArr[5], &m_vWorldPosArr[6]);
	D3DXPlaneFromPoints(&m_planeDir[PLANE_TOP]   , &m_vWorldPosArr[4], &m_vWorldPosArr[5], &m_vWorldPosArr[1]);
	D3DXPlaneFromPoints(&m_planeDir[PLANE_BOTTOM], &m_vWorldPosArr[3], &m_vWorldPosArr[2], &m_vWorldPosArr[6]);

	return S_OK;
}

CBoxCollider * CBoxCollider::Create(_pDevice pGraphicDevice, void * pArg)
{
	CBoxCollider* pInstance = new CBoxCollider(pGraphicDevice);

	if (pArg == nullptr)
	{
		if (FAILED(pInstance->NativeConstruct_Prototype()))
		{
			MSGBOX("CBoxCollider Create - NativeConstruct_Prototype");
			Safe_Release(pInstance);
		}
	}
	else
	{
		if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
		{
			MSGBOX("CBoxCollider Create - NativeConstruct_Prototype(pArg)");
			Safe_Release(pInstance);
		}
	}

	return pInstance;
}

CBoxCollider * CBoxCollider::Create_Tool(_pDevice pGraphicDevice, _vec3 vColliderCetner, _vec3 vColliderSize, DWORD dwColor, CGameObject* pObj)
{
	CBoxCollider* pInstance = new CBoxCollider(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(vColliderCetner, vColliderSize,dwColor)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBoxCollider Create - NativeConstruct_Prototype( ColliderInfo )");
	}
	pObj->Set_Collision(true);

	return pInstance;
}

void CBoxCollider::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	__super::Free();
}

CComponent * CBoxCollider::Clone(void * pArg)
{
	CComponent* pInstance = new CBoxCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBoxCollider Clone Failed");
	}

	return pInstance;
}
