#include "stdafx.h"
#include "..\public\TombStoneParticles.h"
#include "GameInstance.h"


CTombStoneParticles::CTombStoneParticles(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CTombStoneParticles::CTombStoneParticles(const CTombStoneParticles & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTombStoneParticles::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTombStoneParticles NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTombStoneParticles::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTombStoneParticles NativeConstruct - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CTombStoneParticles NativeConstruct - SetUp_Component");

	Set_StartPos(pArg);

	return S_OK;
}

_int CTombStoneParticles::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return OBJ_DEAD;

	Move(fTimeDelta);

	return OBJ_NOEVENT;
}

_int CTombStoneParticles::LateTick(_float fTimeDelta)
{

	BillBoarding();

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_NONALPHA, this)))
		MSG_MINUSONE("CTombStoneParticles LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CTombStoneParticles::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CTombStoneParticles Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CTombStoneParticles Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CTombStoneParticles Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CTombStoneParticles Render - m_pTextureCom is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CTombStoneParticles Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice()))
		MSG_FAIL("CTombStoneParticles Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CTombStoneParticles Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CTombStoneParticles Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CTombStoneParticles Render - Release_RenderState");

	return S_OK;
}

HRESULT CTombStoneParticles::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CTombStoneParticles Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CTombStoneParticles::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CTombStoneParticles::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CTombStoneParticles::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CTombStoneParticles::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CTombStoneParticles Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CTombStoneParticles::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CTombStoneParticles::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CTombStoneParticles SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransformMove"), (CComponent**)&m_pMoveTransformCom)))
		MSG_FAIL("CTombStoneParticles SetUp_Component - Add_Component m_pMoveTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransformMiddle"), (CComponent**)&m_pMiddleTransformCom)))
		MSG_FAIL("CTombStoneParticles SetUp_Component - Add_Component m_pMiddleTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CTombStoneParticles SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRect"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CTombStoneParticles SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureTombStoneParticle"), TEXT("ComTexture"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CTombStoneParticles SetUp_Component - Add_Component m_pTextureCom");
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CTombStoneParticles::BillBoarding()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matCamWorld = pGameInstance->Get_CamWorld();
	_vec3 vEffectScale = m_pTransformCom->Get_Scale();

	m_pTransformCom->SetUp_State(CTransform::STATE_RIGHT, (*(_vec3*)(&matCamWorld.m[0][0])) * vEffectScale.x);
	m_pTransformCom->SetUp_State(CTransform::STATE_UP, (*(_vec3*)(&matCamWorld.m[1][0])) * vEffectScale.y);
	m_pTransformCom->SetUp_State(CTransform::STATE_LOOK, (*(_vec3*)(&matCamWorld.m[2][0])) * vEffectScale.z);

	RELEASE_INSTANCE(CGameInstance);
}

void CTombStoneParticles::Move(_float fTimeDelta)
{
	//// 현재 좌표
	//_vec3 vPos = m_pMoveTransformCom->Get_State(CTransform::STATE_POS);

	//// 이동
	////vPos += m_vDir * 2.5f * fTimeDelta;

	//// 타겟과의 거리 계산
	//_float fNowLength = D3DXVec3Length(&(vPos - m_vTargetPos));

	//// 작으면 소멸
	//if (fNowLength <= 0.2f)
	//	m_bDead = true;

	//// 이동할 트랜스폼의 좌표 세팅
	//m_pMoveTransformCom->SetUp_State(CTransform::STATE_POS, vPos);

	//// 회전
	//m_fAngle += 5.f;
	//m_pMoveTransformCom->SetUp_Rotation(m_pMoveTransformCom->Get_State(CTransform::STATE_LOOK), m_fAngle);

	//// 부모의 up 방향을 받아옴
	//_vec3 vUpDir = m_pMoveTransformCom->Get_State(CTransform::STATE_UP);
	//D3DXVec3Normalize(&vUpDir, &vUpDir);

	//// up방향으로 자식 행렬이동
	//vPos += vUpDir * 0.3f;
	//m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos);

	////// 부모 행렬과 곱
	////_mat MiddleMatrix = *m_pMiddleTransformCom->Get_WorldMatrixPtr();
	////_mat MoveMatrix = *m_pMoveTransformCom->Get_WorldMatrixPtr();
	////_mat RenderMatrix = MiddleMatrix * MoveMatrix;

	////// 랜더할 트랜스폼에 세팅
	////m_pTransformCom->Set_Matrix(MiddleMatrix);

	//m_pMoveTransformCom->SetUp_LookOrtho(m_vDir);

	//같은 방향 바라보게 룩벡터 세팅
	m_pMoveTransformCom->SetUp_State(CTransform::STATE_LOOK, m_vDir);
	//직교할 수 있게 나머지 방향도 세팅 필요함
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &_vec3(0.f, 0.f, -1.f), &m_vDir);
	m_pMoveTransformCom->SetUp_State(CTransform::STATE_RIGHT, vRight);

	_vec3 vUp;
	D3DXVec3Cross(&vUp, &m_vDir, &vRight);
	m_pMoveTransformCom->SetUp_State(CTransform::STATE_UP, vUp);

	// 현재 좌표
	_vec3 vPos = m_pMoveTransformCom->Get_State(CTransform::STATE_POS);
	// 이동
	vPos += m_vDir * 2.5f * fTimeDelta;
	// 타겟과의 거리 계산
	_float fNowLength = D3DXVec3Length(&(vPos - m_vTargetPos));
	// 작으면 소멸
	if (fNowLength <= 0.2f)
		m_bDead = true;
	// 이동할 트랜스폼의 좌표 세팅
	m_pMoveTransformCom->SetUp_State(CTransform::STATE_POS, vPos);
	// 회전
	m_fAngle += 5.f;

	m_pMiddleTransformCom->Set_Matrix(*m_pMoveTransformCom->Get_WorldMatrixPtr());

	m_pMiddleTransformCom->SetUp_Scale(_vec3(0.2f, 0.2f, 0.2f));

	m_pMiddleTransformCom->SetUp_Rotation(m_pMiddleTransformCom->Get_State(CTransform::STATE_LOOK), m_fAngle);

	m_pMiddleTransformCom->SetUp_State(CTransform::STATE_POS, m_pMiddleTransformCom->Get_State(CTransform::STATE_POS) + m_pMiddleTransformCom->Get_State(CTransform::STATE_RIGHT) * 2.f );

	m_pTransformCom->Set_Matrix(*m_pMiddleTransformCom->Get_WorldMatrixPtr());
}

void CTombStoneParticles::Set_StartPos(void * pArg)
{
	DESC* pDESC = (DESC*)pArg;

	_vec3 vMyPos = pDESC->vStartPos;
	_vec3 vTargetPos = pDESC->vTargetPos;

	// 거리설정
	m_fMaxLength = D3DXVec3Length(&(vTargetPos - vMyPos));

	m_vTargetPos = vTargetPos;

	// 회전
	_vec3 vDirToTarget = vTargetPos - vMyPos;
	D3DXVec3Normalize(&vDirToTarget, &vDirToTarget);

	m_vDir = vDirToTarget;

	_vec3 vDirNomarl = { 0.f,0.f,1.f };
	D3DXVec3Normalize(&vDirNomarl, &vDirNomarl);

	_float fCosTheta = D3DXVec3Dot(&vDirToTarget, &vDirNomarl);

	_float fAngle = acosf(fCosTheta);

	fAngle = D3DXToDegree(fAngle);

	if (vMyPos.x > vTargetPos.x)
		fAngle *= -1.f;

	m_pMoveTransformCom->SetUp_State(CTransform::STATE_POS, pDESC->vStartPos);

	m_pMoveTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), fAngle);

	m_pMoveTransformCom->SetUp_Rotation(m_pMoveTransformCom->Get_State(CTransform::STATE_RIGHT), 90.f);

	m_pTransformCom->SetUp_Scale(pDESC->vScale);
}

void CTombStoneParticles::Free()
{
	__super::Free();
	Safe_Release(m_pMoveTransformCom);
	Safe_Release(m_pMiddleTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CTombStoneParticles::Clone(void * pArg)
{
	CGameObject* pInstance = new CTombStoneParticles(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CTombStoneParticles Clone Failed");
	}

	return pInstance;
}

CTombStoneParticles * CTombStoneParticles::Create(_pDevice pGraphicDevice)
{
	CTombStoneParticles* pInstance = new CTombStoneParticles(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CTombStoneParticles Create Failed");
	}

	return pInstance;
}

_vec3 CTombStoneParticles::Get_MinPos()
{
	return _vec3();
}

_vec3 CTombStoneParticles::Get_MaxPos()
{
	return _vec3();
}

_vec3 CTombStoneParticles::Get_WorldPos()
{
	return _vec3();
}

_vec3 CTombStoneParticles::Get_Length()
{
	return _vec3();
}
