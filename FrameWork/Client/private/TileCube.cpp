#include "stdafx.h"
#include "..\public\TileCube.h"


CTileCube::CTileCube(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{

}

CTileCube::CTileCube(const CTileCube & rhs)
	: CGameObject(rhs)
{

}

HRESULT CTileCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTileCube NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTileCube::NativeConstruct(void * pArg)
{
	if(FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTileCube NativeConstruct - __super");


	return S_OK;
}

_int CTileCube::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CTileCube Tick - super");

	if (m_bDead)
		return OBJ_DEAD;


	return _int();
}

_int CTileCube::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CTileCube LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CTileCube::LateTick - m_pRendererCom == nullptr");

	m_bCollision = false;
	if (m_pFrustum->Check_Culling(m_pTransform, m_fFrustumRadius))
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_NONALPHA, this)))
			MSG_MINUSONE("CTileCube LateTick - Add_RenderGroup");
		m_bCollision = true;
	}
	return _int();
}

HRESULT CTileCube::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CTileCube Render - __super");

	if (m_pRenderer == nullptr)
		MSG_FAIL("CTileCube Render - m_pRendererCom is null");

	if (m_pVIBuffer == nullptr)
		MSG_FAIL("CTileCube Render - m_pVIBufferCom is null");

	if (m_pTexture == nullptr)
		MSG_FAIL("CTileCube Render - m_pTextureCom is null");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CTileCube Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTexture->Bind_OnDevice(m_iTextureIndex)))
		MSG_FAIL("CTileCube Render - m_pTextureCom Bind_OnDevice");

	if(FAILED(SetUp_RenderState()))
		MSG_FAIL("CTileCube Render - SetUp_RenderState");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CTileCube Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CTileCube Render - Release_RenderState ");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CTileCube Render - m_pBoxCollider Render");


	return S_OK;

}

HRESULT CTileCube::SetUp_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CTileCube::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

_vec3 CTileCube::Compute_Height(CTransform * pObjectTransform)
{
	_vec3 vPos = pObjectTransform->Get_State(CTransform::STATE_POS);
	_mat matWorld = *m_pTransform->Get_WorldMatrixPtr();
	_mat matWorldInverse;

	D3DXMatrixInverse(&matWorldInverse, nullptr, &matWorld);
	D3DXVec3TransformCoord(&vPos, &vPos, &matWorldInverse);

	//_float fY = m_pVIBuffer->Compute_Height(vPos);
	//vPos.y = fY;
	//D3DXVec3TransformCoord(&vPos, &vPos, &matWorld);

	return vPos;
}

_vec3 CTileCube::Get_MinPos()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	_uint iCurrentCameraId = pInstance->Get_CurrentCameraID();
	RELEASE_INSTANCE(CGameInstance);
	_vec3 vMinPos;
	if (iCurrentCameraId == CAMERA_BACK)
		vMinPos = m_pBoxCollider->Get_MinPos();
	else if (iCurrentCameraId == CAMERA_2D)
		vMinPos = m_pBoxCollider->Get_MinPosProj();

	
	return vMinPos;
}

_vec3 CTileCube::Get_MaxPos()
{
	return m_pBoxCollider->Get_MaxPos();
}

_vec3 CTileCube::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

_vec3 CTileCube::Get_Length()
{
	return m_pTransform->Get_Scale();
}

_vec3 CTileCube::Get_MaxProjPos()
{
	return m_pBoxCollider->Get_MaxPosProj();
}

void CTileCube::Free()
{
	__super::Free();
	Safe_Release(m_pFrustum);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
}

HRESULT CTileCube::Tick_Collider(_float fTimeDelta)
{
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);
	m_plane[CBoxCollider::PLANE_FRONT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_FRONT);
	m_plane[CBoxCollider::PLANE_BACK] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BACK);
	m_plane[CBoxCollider::PLANE_LEFT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_LEFT);
	m_plane[CBoxCollider::PLANE_RIGHT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_RIGHT);
	m_plane[CBoxCollider::PLANE_TOP] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_TOP);
	m_plane[CBoxCollider::PLANE_BOTTOM] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BOTTOM);

	return S_OK;
}
