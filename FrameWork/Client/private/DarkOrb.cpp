#include "stdafx.h"
#include "..\public\DarkOrb.h"
#include "Player.h"
#include "Attack.h"
#include "Monster.h"
#include "PlayerTagMgr.h"


CDarkOrb::CDarkOrb(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CDarkOrb::CDarkOrb(const CDarkOrb & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDarkOrb::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CDarkOrb NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CDarkOrb::NativeConstruct(void * pArg)
{
	if (pArg == nullptr)
		MSG_FAIL("CDarkOrb::NativeConstruct - pArg == nullptr");

	DESC* pDesc = (DESC*)pArg;

	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CDarkOrb NativeConstruct - __super::NativeConstruct");

	if (FAILED(SetUp_Component(pDesc->pObj)))
		MSG_FAIL("CDarkOrb NativeConstruct - SetUp_Component");

	//방향 구하기
	_vec3 vRight;
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
	m_vLook = pDesc->vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_POS);
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_vReadyPos = pDesc->vTargetPos;

	m_tTime.fDelay = 1.f;

	return S_OK;
}

_int CDarkOrb::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CMagicMissile Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	if(m_bReady == true)
		m_tTime.Tick_Time(fTimeDelta);

	Tick_Frame(fTimeDelta);

	MoveFirePos(fTimeDelta);

	Move(fTimeDelta);

	return 0;
}

_int CDarkOrb::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CDarkOrb LateTick - __super");

	BillBoarding(m_pTransformCom);

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CDarkOrb::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CDarkOrb::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CDarkOrb Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pTextureCom == nullptr ||
		m_pBoxColliderCom == nullptr ||
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CDarkOrb::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CDarkOrb::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CDarkOrb::Render - m_pTextureComArr->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CDarkOrb::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CDarkOrb::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CDarkOrb::Render - Release_RenderState");

	if (FAILED(m_pBoxColliderCom->Render()))
		MSG_FAIL("CDarkOrb::Render - m_pBoxColliderCom->Render");

	return S_OK;
}

HRESULT CDarkOrb::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	tObjInfo.dwColliderColor = 0xffffffff;
	tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tObjInfo.vColliderSize = _vec3(0.5f, 0.5f, 0.5f);

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 30.f;
	tTransformDesc.fDegree = 3.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CDarkOrb::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CDarkOrb::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CDarkOrb::SetUp_Component - m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureDarkOrb"), TEXT("ComTextureDarkOrb"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CDarkOrb::SetUp_Component - ComTextureDarkOrb");

	m_tFrame.Set_Value(m_pTextureCom->Get_ImageCount() - 1);

	m_pBoxColliderCom = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);

	return S_OK;
}

HRESULT CDarkOrb::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CDarkOrb::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CDarkOrb::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CDarkOrb::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CDarkOrb::Set_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

HRESULT CDarkOrb::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CDarkOrb::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CDarkOrb::Release_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

void CDarkOrb::Tick_Frame(_float fTimeDelta)
{
	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fTime >= m_tFrame.fDelay)
	{
		m_tFrame.fTime = 0.f;
		m_tFrame.iStart++;

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;
		}
	}
}

HRESULT CDarkOrb::MoveFirePos(_float fTimeDelta)
{
	if (true == m_bReady)
		return S_OK;

	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	_float fDist = fTimeDelta * 2.f;
	vPos += *D3DXVec3Normalize(&m_vLook, &m_vLook) * fDist;
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos);

	_vec3 vLength;
	vLength = vPos - m_vReadyPos;
	_float fLength = D3DXVec3Length(&vLength);

	if (0.1f >= fLength)
		m_bReady = true;

	return S_OK;
}

HRESULT CDarkOrb::Move(_float fTimeDelta)
{
	if (false == m_bReady)
		return S_OK;

	if (m_tTime.Check_Time())
	{

		_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
		_float fDist = fTimeDelta * 40.f;
		m_fDist += fDist;

		if (50.f <= m_fDist)
			m_bDead = true;

		vPos += *D3DXVec3Normalize(&m_vLook, &m_vLook) * fDist;
		m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos);
	}
	else
	{
		_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);

		m_pPlayerTransformCom = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();

		_vec3 pTargetPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POS);

		pTargetPos.y += 0.5f;

		m_vLook = pTargetPos - vPos;
	}

	return S_OK;
}

CDarkOrb * CDarkOrb::Create(_pDevice pDevice)
{
	CDarkOrb* pInstance = new CDarkOrb(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissile Create Failed");
	}

	return pInstance;
}

CGameObject * CDarkOrb::Clone(void * pArg)
{
	CGameObject* pInstance = new CDarkOrb(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissile Clone Failed");
	}

	return pInstance;
}

void CDarkOrb::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}

void CDarkOrb::Collision(CGameObject * pColObj)
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

_vec3 CDarkOrb::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxColliderCom->Get_MinPos();
	return vMinPos;
}

_vec3 CDarkOrb::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxColliderCom->Get_MaxPos();
	return vMaxPos;
}

_vec3 CDarkOrb::Get_WorldPos()
{
	return m_pBoxColliderCom->Get_ColliderWolrdPos();
}

_vec3 CDarkOrb::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}

HRESULT CDarkOrb::BillBoarding(CTransform * pTransform)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matCamWorld = pGameInstance->Get_CamWorld();
	_vec3 vEffectScale = pTransform->Get_Scale();

	pTransform->SetUp_State(CTransform::STATE_RIGHT, (*(_vec3*)(&matCamWorld.m[0][0])) * vEffectScale.x);
	pTransform->SetUp_State(CTransform::STATE_UP, (*(_vec3*)(&matCamWorld.m[1][0])) * vEffectScale.y);
	pTransform->SetUp_State(CTransform::STATE_LOOK, (*(_vec3*)(&matCamWorld.m[2][0])) * vEffectScale.z);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CDarkOrb::Tick_Collider(_float fTimeDelta)
{
	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	return S_OK;
}
