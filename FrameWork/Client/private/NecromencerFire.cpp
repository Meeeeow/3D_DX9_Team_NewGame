#include "stdafx.h"
#include "..\public\NecromencerFire.h"
#include "Player.h"
#include "Attack.h"
#include "Monster.h"


CNecromencerFire::CNecromencerFire(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CNecromencerFire::CNecromencerFire(const CNecromencerFire & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNecromencerFire::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CNecromencerFire NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CNecromencerFire::NativeConstruct(void * pArg)
{
	if (pArg == nullptr)
		MSG_FAIL("CNecromencerFire::NativeConstruct - pArg == nullptr");

	DESC* pDesc = (DESC*)pArg;

	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CNecromencerFire NativeConstruct - __super::NativeConstruct");

	if(FAILED(SetUp_Component(pDesc->pObj)))
		MSG_FAIL("CNecromencerFire NativeConstruct - SetUp_Component");

	//방향 구하기
	_vec3 vRight;
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
	m_vLook = pDesc->vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_POS);
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	return S_OK;
}

_int CNecromencerFire::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CMagicMissile Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	Tick_Frame(fTimeDelta);

	Move(fTimeDelta);

	return 0;
}

_int CNecromencerFire::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CNecromencerFire LateTick - __super");

	BillBoarding(m_pTransformCom);

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CNecromencerFire::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CNecromencerFire::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CNecromencerFire Render - __super");

	if (m_pTransformCom		== nullptr ||
		m_pTextureCom		== nullptr ||
		m_pBoxColliderCom	== nullptr ||
		m_pVIBufferCom		== nullptr)
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

HRESULT CNecromencerFire::SetUp_Component(void * pArg)
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

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureFire"), TEXT("ComTextureFire"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CNecromencerFire::SetUp_Component - ComTextureFire");

	m_tFrame.Set_Value(m_pTextureCom->Get_ImageCount() - 1);

	m_pBoxColliderCom = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);

	return S_OK;
}

HRESULT CNecromencerFire::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNecromencerFire::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CNecromencerFire::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CNecromencerFire::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CNecromencerFire::Set_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

HRESULT CNecromencerFire::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNecromencerFire::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CNecromencerFire::Release_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

void CNecromencerFire::Tick_Frame(_float fTimeDelta)
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

HRESULT CNecromencerFire::Move(_float fTimeDelta)
{
	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	_float fDist = fTimeDelta * 20.f;

	m_fDist += fDist;

	if (50.f <= m_fDist)
		m_bDead = true;

	vPos += *D3DXVec3Normalize(&m_vLook, &m_vLook) * fDist;
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos);

	return S_OK;
}

CNecromencerFire * CNecromencerFire::Create(_pDevice pDevice)
{
	CNecromencerFire* pInstance = new CNecromencerFire(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissile Create Failed");
	}

	return pInstance;
}

CGameObject * CNecromencerFire::Clone(void * pArg)
{
	CNecromencerFire* pInstance = new CNecromencerFire(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissile Clone Failed");
	}

	return pInstance;
}

void CNecromencerFire::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}

void CNecromencerFire::Collision(CGameObject * pColObj)
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

_vec3 CNecromencerFire::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxColliderCom->Get_MinPos();
	return vMinPos;
}

_vec3 CNecromencerFire::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxColliderCom->Get_MaxPos();
	return vMaxPos;
}

_vec3 CNecromencerFire::Get_WorldPos()
{
	return m_pBoxColliderCom->Get_ColliderWolrdPos();
}

Engine::_vec3 CNecromencerFire::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}

HRESULT CNecromencerFire::BillBoarding(class CTransform* pTransform)
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

HRESULT CNecromencerFire::Tick_Collider(_float fTimeDelta)
{
	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	return S_OK;
}