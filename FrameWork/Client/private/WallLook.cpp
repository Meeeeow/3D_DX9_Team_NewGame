#include "stdafx.h"
#include "..\public\WallLook.h"
#include "GameInstance.h"
#include "Player.h"
#include "Collision.h"

CWallLook::CWallLook(_pDevice pGraphicDevice)
	: CWallBase(pGraphicDevice)
{
}

CWallLook::CWallLook(const CWallLook & rhs)
	: CWallBase(rhs)
{
}

HRESULT CWallLook::NativeConstruct_Prototype()
{
	if (__super::NativeConstruct_Prototype())
		MSG_FAIL("CWallLook NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CWallLook::NativeConstruct(void * pArg)
{
	if (__super::NativeConstruct(pArg))
		MSG_FAIL("CWallLook NativeConstruct - __super");

	if (pArg == nullptr)
	{
		if (FAILED(SetUp_Component()))
			MSG_FAIL("CWallLook NativeConstruct - SetUp_Component");

		m_pTransform->SetUp_Scale(_vec3(3.f, 3.f, 3.f));
		m_pTransform->SetUp_State(CTransform::STATE_POS, _vec3(_float(rand() % 140), 3.f, _float(rand() % 140)));
	}
	else
	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CWallLook NativeConstruct - SetUp_Component");

	_vec3 vScale = m_pTransform->Get_Scale();
	m_fMaxScale = max(vScale.x, vScale.y);
	m_fMaxScale = max(m_fMaxScale, vScale.z);

	return S_OK;
}

_int CWallLook::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CWallLook Tick - __super");



	return _int();
}

_int CWallLook::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CExplosion LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CExplosion::LateTick - m_pRendererCom == nullptr");




	if (m_pFrustum->Check_Culling(m_pTransform, 1.5f * m_fMaxScale))
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_NONALPHA, this)))
			MSG_MINUSONE("CWallLook LateTick - Add_RenderGroup");
	}
	return _int();
}

HRESULT CWallLook::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CWallLook Render - __super");

	if (m_pRenderer == nullptr)
		MSG_FAIL("CWallLook Render - m_pRendererCom is null");

	if (m_pVIBuffer == nullptr)
		MSG_FAIL("CWallLook Render - m_pVIBufferCom is null");

	if (m_pTexture == nullptr)
		MSG_FAIL("CWallLook Render - m_pTextureCom is null");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CWallLook Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTexture->Bind_OnDevice(m_iTextureNum)))
		MSG_FAIL("CWallLook Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CWallLook::Render - Set_RenderState");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CWallLook Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CWallLook::Render - Release_RenderState");

	if(FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CWallLook Render - m_pBoxCollider Render");


	return S_OK;
}

HRESULT CWallLook::Tick_Collider(_float fTimeDelta)
{
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);
	m_planeCollision = m_pBoxCollider->Get_PlaneDir(m_ePlaneDir);

	return S_OK;
}

void CWallLook::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_uint iNumCurrentCamera = pGameInstance->Get_CurrentCameraID();
	RELEASE_INSTANCE(CGameInstance);
	if (iNumCurrentCamera == CAMERA_2D)
		return;

	
	CPlayer* pPlayerObject = dynamic_cast<CPlayer*>(pColObj);
	if (pPlayerObject != nullptr)
	{
		CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayerObject->Get_Component(pTagComTransform));

		_vec3 vDistance = CCollision::WallCollisionResult(
			Get_Plane(), Get_PlaneDir(), Get_WorldPos(), Get_Length()
			, pPlayerObject->Get_MinPos(), pPlayerObject->Get_MaxPos(), pPlayerObject->Get_WorldPos(), pPlayerObject->Get_Length());

		pPlayerTransform->SetUp_State(CTransform::STATE_POS, vDistance);
	}
}

HRESULT CWallLook::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform)))
		MSG_FAIL("CWallLook SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CWallLook SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustum)))
		MSG_FAIL("CWallLook SetUp_Component - Add_Component m_pFrustumCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRectXY"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CWallLook SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWall"), TEXT("ComTexture"), (CComponent**)&m_pTexture)))
		MSG_FAIL("CWallLook SetUp_Component - Add_Component m_pTextureCom");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice,_vec3(0.f,0.f,0.f),_vec3(1.f,1.f,1.f),D3DCOLOR_ARGB(255,255,255,255),this);

	if (m_pVIBuffer->Get_PlaneDir())
		m_ePlaneDir = CBoxCollider::PLANE_FRONT;
	else
		m_ePlaneDir = CBoxCollider::PLANE_BACK;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CWallLook::SetUp_Component(void * pArg)
{
	DESC tDesc = *(DESC*)pArg;
	m_iTextureNum = tDesc.tObjInfo.iTextureIndex;
	
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform)))
		MSG_FAIL("CWallLook SetUp_Component - Add_Component m_pTransformCom");
	m_pTransform->Set_Matrix(tDesc.tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CWallLook SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustum)))
		MSG_FAIL("CWallLook SetUp_Component - Add_Component m_pFrustumCom");

	m_pVIBuffer = CVIBufferRectXY::Create(m_pGraphicDevice, tDesc.tObjInfo.eWallLook, tDesc.tObjInfo.fSizeX,
		tDesc.tObjInfo.fSizeY, tDesc.tObjInfo.fU, tDesc.tObjInfo.fV, tDesc.tObjInfo.iNumVerticesX,
		tDesc.tObjInfo.iNumVerticesY);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWall"), TEXT("ComTexture"), (CComponent**)&m_pTexture)))
		MSG_FAIL("CWallLook SetUp_Component - Add_Component m_pTextureCom");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, tDesc.tObjInfo.vColliderCenter,
		tDesc.tObjInfo.vColliderSize, tDesc.tObjInfo.dwColliderColor, this);

	if (m_pVIBuffer->Get_PlaneDir())
		m_ePlaneDir = CBoxCollider::PLANE_FRONT;
	else
 		m_ePlaneDir = CBoxCollider::PLANE_BACK;

	return S_OK;
}

HRESULT CWallLook::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CWallLook::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

_vec3 CWallLook::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxCollider->Get_MinPos();
	
	return vMinPos;
}

_vec3 CWallLook::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxCollider->Get_MaxPos();
	
	return vMaxPos;
}

_vec3 CWallLook::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

_vec3 CWallLook::Get_Length()
{
	return m_pTransform->Get_Scale();
}

CGameObject * CWallLook::Clone(void * pArg)
{
	CWallLook* pInstance = new CWallLook(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CWallLook Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CWallLook * CWallLook::Create(_pDevice pGraphicDevice)
{
	CWallLook* pInstance = new CWallLook(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CWallLook Create");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWallLook::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pFrustum);
}

_plane CWallLook::Get_Plane()
{
	return m_planeCollision;
}

CBoxCollider::COLLIDER_PLANE CWallLook::Get_PlaneDir()
{
	return m_ePlaneDir;
}
