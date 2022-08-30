#include "stdafx.h"
#include "..\public\Arrow.h"
#include "Player.h"
#include "Attack.h"
#include "Monster.h"


CArrow::CArrow(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CArrow::CArrow(const CArrow & rhs)
	: CGameObject(rhs)
{
}

HRESULT CArrow::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CNecromencerFire NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CArrow::NativeConstruct(void * pArg)
{
	if (pArg == nullptr)
		MSG_FAIL("CNecromencerFire::NativeConstruct - pArg == nullptr");

	DESC* pDesc = (DESC*)pArg;

	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CNecromencerFire NativeConstruct - __super::NativeConstruct");

	if (FAILED(SetUp_Component(pDesc->pObj)))
		MSG_FAIL("CNecromencerFire NativeConstruct - SetUp_Component");

	D3DXVec3Normalize(&m_vDir, &(pDesc->vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_POS)));
	
	_vec3 vRight = m_vDir;
	m_pTransformCom->SetUp_State(CTransform::STATE_RIGHT, vRight);

	_vec3 vUp(0.f, 1.f, 0.f);

	_vec3 vLook;
	D3DXVec3Cross(&vLook, &vRight, &vUp);
	m_pTransformCom->SetUp_State(CTransform::STATE_LOOK, vLook);

	D3DXVec3Cross(&vUp, &vLook, &vRight);
	m_pTransformCom->SetUp_State(CTransform::STATE_UP, vUp);

	return S_OK;
}

_int CArrow::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CMagicMissile Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	Move(fTimeDelta);

	return 0;
}

_int CArrow::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CNecromencerFire LateTick - __super");

	BillBoarding(m_pTransformCom);

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CNecromencerFire::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CArrow::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CNecromencerFire Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pTextureCom == nullptr ||
		m_pBoxColliderCom == nullptr ||
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CNecromencerFire::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CNecromencerFire::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CNecromencerFire::Render - m_pTextureComArr->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CNecromencerFire::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CNecromencerFire::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CNecromencerFire::Render - Release_RenderState");

	if (FAILED(m_pBoxColliderCom->Render()))
		MSG_FAIL("CNecromencerFire::Render - m_pBoxColliderCom->Render");

	return S_OK;
}

HRESULT CArrow::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	tObjInfo.dwColliderColor = 0xffffffff;
	tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 30.f;
	tTransformDesc.fDegree = 3.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CNecromencerFire::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CNecromencerFire::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CNecromencerFire::SetUp_Component - m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureArrow"), TEXT("ComTextureFire"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CNecromencerFire::SetUp_Component - ComTextureFire");

	m_pBoxColliderCom = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);

	return S_OK;
}

HRESULT CArrow::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNecromencerFire::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CNecromencerFire::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CNecromencerFire::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CNecromencerFire::Set_RenderState - m_pGraphicDevice->SetRenderState");

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CArrow::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNecromencerFire::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CNecromencerFire::Release_RenderState - m_pGraphicDevice->SetRenderState");

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CArrow::Move(_float fTimeDelta)
{
	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);

	_float fDist = fTimeDelta * 20.f;

	m_fDist += fDist;

	if (50.f <= m_fDist)
		m_bDead = true;

	vPos += *D3DXVec3Normalize(&m_vDir, &m_vDir) * fDist;
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos);

	return S_OK;
}

CArrow * CArrow::Create(_pDevice pDevice)
{
	CArrow* pInstance = new CArrow(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissile Create Failed");
	}

	return pInstance;
}

CGameObject * CArrow::Clone(void * pArg)
{
	CGameObject* pInstance = new CArrow(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissile Clone Failed");
	}

	return pInstance;
}

void CArrow::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}

void CArrow::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);

	if (pPlayer != nullptr)
	{
		CAttack::DESC tDesc;
		tDesc.iDmg = 10;
		tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
		tDesc.vScale = m_pTransformCom->Get_Scale();
		tDesc.bSingleAttack = true;
		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerMonsterAttack, &tDesc);

		Set_Dead();
	}

	RELEASE_INSTANCE(CGameInstance);
}

_vec3 CArrow::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxColliderCom->Get_MinPos();
	return vMinPos;
}

_vec3 CArrow::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxColliderCom->Get_MaxPos();
	return vMaxPos;
}

_vec3 CArrow::Get_WorldPos()
{
	return m_pBoxColliderCom->Get_ColliderWolrdPos();
}

_vec3 CArrow::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}

HRESULT CArrow::BillBoarding(CTransform * pTransform)
{
	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//_mat matCamWorld = pGameInstance->Get_CamWorld();
	//_vec3 vEffectScale = pTransform->Get_Scale();

	//pTransform->SetUp_State(CTransform::STATE_RIGHT, (*(_vec3*)(&matCamWorld.m[0][0])) * vEffectScale.x);
	//pTransform->SetUp_State(CTransform::STATE_UP, (*(_vec3*)(&matCamWorld.m[1][0])) * vEffectScale.y);
	//pTransform->SetUp_State(CTransform::STATE_LOOK, (*(_vec3*)(&matCamWorld.m[2][0])) * vEffectScale.z);

	//RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CArrow::Tick_Collider(_float fTimeDelta)
{
	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	return S_OK;
}
