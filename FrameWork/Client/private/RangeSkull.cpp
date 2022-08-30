#include "stdafx.h"
#include "..\public\RangeSkull.h"
#include "ResurrectionManager.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "MagicMissile.h"
#include "Attack.h"
#include "DmgFont.h"
#include "MonsterColliderManager.h"
#include "Arrow.h"


CRangeSkull::CRangeSkull(_pDevice pGraphicDevice)
	: CMonster(pGraphicDevice)
{
	ZeroMemory(m_pTextureComArr, sizeof(m_pTextureComArr));
}

CRangeSkull::CRangeSkull(const CRangeSkull & rhs)
	: CMonster(rhs)
{
}

HRESULT CRangeSkull::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CRangeSkull NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CRangeSkull::NativeConstruct(void * pArg)
{
	CMonster::MONSTER_STAT tMonsterStat;
	tMonsterStat.iMaxHp = 15;
	tMonsterStat.iHp = tMonsterStat.iMaxHp;
	tMonsterStat.iDmg = 20;

	if (FAILED(__super::NativeConstruct(&tMonsterStat)))
		MSG_FAIL("CMonsterNormalRange NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CMonsterNormalRange NativeConstruct - SetUp_Component");

	Check_Frame();

	m_bStateLock = true;

	m_bClone = true;

	CResurrectionManager::Get_Instance()->Add_Monster(this);

	return S_OK;
}

_int CRangeSkull::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CMonsterNormalRange Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	AI();
	
	Move(fTimeDelta);

	Go_Necromencer(fTimeDelta);

	Check_Frame();

	if (m_ePreState != STATE_DIE)
		Collision_Terrain(fTimeDelta, pGameInstance, m_pTransformCom);

	BillBoarding(m_pTransformCom);

	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CRangeSkull::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CMonsterNormalRange LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CMonsterNormalRange::LateTick - m_pRendererCom == nullptr");

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CMonsterNormalRange::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CRangeSkull::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CMonsterNormalRange Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pTextureComArr == nullptr ||
		m_pBoxColliderCom == nullptr ||
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CMonsterNormalRange::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CMonsterNormalRange::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureComArr[m_ePreState]->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CMonsterNormalRange::Render - m_pTextureComArr[m_eCurState]->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CMonsterNormalRange::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CMonsterNormalRange::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CMonsterNormalRange::Render - Release_RenderState");

	if (FAILED(m_pBoxColliderCom->Render()))
		MSG_FAIL("CMonsterNormalRange::Render - m_pBoxColliderCom->Render");

	return S_OK;
}

_bool CRangeSkull::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, CTransform * pOut)
{
	_vec3 vPos;
	if (m_pVIBufferCom->Picking(vMouseRay, vMousePivot, &vPos))
	{
		pOut = dynamic_cast<CTransform*>(Get_Component(pTagComTransform));
		return true;
	}
	return false;
}

HRESULT CRangeSkull::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 10.f;
	tTransformDesc.fDegree = 3.f;
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("RangeSkullBuffer"), pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - m_pVIBufferCom");

	// Attack
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullAttack"), L"ComTextureAttack1", (CComponent**)&m_pTextureComArr[STATE_ATTACK])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - TEXTURE_ATTACK");

	// Death
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullDeath"), L"ComTextureDeath", (CComponent**)&m_pTextureComArr[STATE_DEATH])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - TEXTURE_DEATH");

	// Die
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullDie"), L"ComTextureDie", (CComponent**)&m_pTextureComArr[STATE_DIE])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - TEXTURE_DIE");

	// Hit
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullHit"), L"ComTextureHit", (CComponent**)&m_pTextureComArr[STATE_HIT])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - TEXTURE_HIT");

	// Idle
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullIdle"), L"ComTextureIdle", (CComponent**)&m_pTextureComArr[STATE_IDLE])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - TEXTURE_IDLE");

	// Move
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullMove"), L"ComTextureMove", (CComponent**)&m_pTextureComArr[STATE_MOVE])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - TEXTURE_MOVE");

	// Spawn
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullSpawn"), L"ComTextureSpawn", (CComponent**)&m_pTextureComArr[STATE_SPAWN])))
		MSG_FAIL("CMonsterNormalRange::SetUp_Component - TEXTURE_SPAWN");

	OBJ_INFO* pColDat = CMonsterColliderManager::Get_Instance()->Get_ColliderDat(CMonsterColliderManager::COLLIDER_RANGE_SKULL);

	m_pBoxColliderCom = CBoxCollider::Create_Tool(m_pGraphicDevice, pColDat->vColliderCenter, pColDat->vColliderSize, pColDat->dwColliderColor, this);

	return S_OK;
}

HRESULT CRangeSkull::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CRangeSkull::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CMonsterNormalRange::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

void CRangeSkull::Set_State(STATE eState)
{
	if (m_bStateLock)
		return;

	m_eCurState = eState;
}

void CRangeSkull::Set_Idle()
{
	m_bStateLock = false;
	Set_State(STATE_IDLE);
	Check_Frame();
}

void CRangeSkull::Check_Frame()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CRangeSkull::STATE_IDLE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_IDLE]->Get_ImageCount() - 1);
			break;
		case Client::CRangeSkull::STATE_MOVE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_MOVE]->Get_ImageCount() - 1);
			break;
		case Client::CRangeSkull::STATE_ATTACK:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_ATTACK]->Get_ImageCount() - 1, 0, 0.05f);
			break;
		case Client::CRangeSkull::STATE_HIT:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_HIT]->Get_ImageCount() - 1);
			break;
		case Client::CRangeSkull::STATE_DEATH:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_DEATH]->Get_ImageCount() - 1);
			break;
		case Client::CRangeSkull::STATE_DIE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_DIE]->Get_ImageCount() - 1);
			break;
		case Client::CRangeSkull::STATE_SPAWN:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_SPAWN]->Get_ImageCount() - 1);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CRangeSkull::Tick_Frame(_float fTimeDelta)
{
	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fTime >= m_tFrame.fDelay)
	{
		m_tFrame.fTime = 0.f;
		m_tFrame.iStart++;

		if (m_ePreState == STATE_ATTACK && m_tFrame.iStart == 7)
			Attack();

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;

			switch (m_ePreState)
			{
			case Client::CRangeSkull::STATE_ATTACK:
				m_eCurState = STATE::STATE_IDLE;
				m_bStateLock = false;
				break;
			case Client::CRangeSkull::STATE_HIT:
				m_eCurState = STATE::STATE_IDLE;
				m_bStateLock = false;
				break;
			case Client::CRangeSkull::STATE_DEATH:
				m_eCurState = STATE::STATE_DIE;
				break;
			case Client::CRangeSkull::STATE_SPAWN:
				m_eCurState = STATE::STATE_IDLE;
				m_bStateLock = false;
				break;
			case Client::CRangeSkull::STATE_DIE:
				break;
			}
		}
	}
}

void CRangeSkull::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	if (pAttack != nullptr && m_eCurState != STATE_DIE && m_eCurState != STATE_DEATH)
	{
		_uint iDmg = pAttack->Get_Dmg();
		//공격이 단일 공격인데 이미 맞은 오브젝트가 잇는 경우
		if (pAttack->Get_SingleAttack() && pAttack->Get_Dead())
			return;

		////빙결 상태이상
		//if (pAttack->Get_Freeze())
		//	;

		////독 상태이상
		//if (pAttack->Get_Poision())
		//	;

		if (iDmg > 0)
		{
			Sub_Hp(iDmg);
		}

		//공격 당한 모션
		m_bStateLock = false;
		Set_State(STATE_HIT);
		m_tTimeHit.fTime = 0.f;
		m_tTimeHit.fDelay = 0.5f;
		m_tFrame.iStart = 0;
		m_bStateLock = true;

		pAttack->Set_Dead();
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CRangeSkull::Sub_Hp(_uint iDmg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(__super::Sub_Hp(iDmg)))
		MSG_FAIL("CRangeSkull::Sub_Hp - __super::Sub_Hp");

	CDmgFont::DESC tDesc;
	tDesc.iDmg = iDmg;
	tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	tDesc.vPos.y += 2.5f;
	tDesc.vScale = m_pTransformCom->Get_Scale();
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDmgFont, pTagLayerDmgFont, &tDesc)))
		MSG_FAIL("pGameInstance->Add_GameObjectToLayer - pTagPrototypeDmgFont");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CRangeSkull::AI()
{
	m_pPlayerTransformCom = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
	if (m_pPlayerTransformCom == nullptr)
		MSG_VOID("CMonsterNormalRange::AI - m_pTransformComPlayer == nullptr");

	if (m_bStateLock == false)
	{
		_float fDistance = m_pTransformCom->Get_DistanceTarget(m_pPlayerTransformCom);

		if (fDistance <= 20.f)
			Set_State(STATE::STATE_MOVE);
		else
			Set_State(STATE::STATE_IDLE);

		if (m_eCurState == STATE::STATE_MOVE)
		{
			if (fDistance <= 10.f)
			{
				Set_State(STATE::STATE_ATTACK);
				m_bStateLock = true;
			}
		}
	}

	if (Check_Dead() && m_ePreState != STATE::STATE_DIE && m_ePreState != STATE::STATE_DEATH)
	{
		m_bStateLock = false;
		Set_State(STATE::STATE_DEATH);
		m_bStateLock = true;
		m_bDie = true;
	}
}

void CRangeSkull::Move(_float fTimeDelta) // 이동
{
	if (m_pPlayerTransformCom == nullptr ||
		m_pTransformCom == nullptr)
		MSG_VOID("CMonsterNormalRange::Move - null");

	if (m_eCurState != STATE::STATE_MOVE)
		return;

	m_pTransformCom->Chase_Target(m_pPlayerTransformCom, fTimeDelta, 0.5f);
}

void CRangeSkull::Attack() // 공격
{
	if (m_ePreState != STATE::STATE_ATTACK)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CArrow::DESC tDesc;
	OBJ_INFO tObj;
	_vec3 vPos;
	vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	vPos.y += 1.f;
	tObj.SetUp_Scale(0.7f, 0.7f, 0.7f);
	tObj.SetUp_State(OBJ_INFO::STATE_POS, vPos);

	vPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POS);
	vPos.y += 0.5f;
	tDesc.vTargetPos = vPos;
	tDesc.pObj = &tObj;

	pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeArrow"), pTagLayerMonsterAttack, &tDesc);

	RELEASE_INSTANCE(CGameInstance);
}

void CRangeSkull::Go_Necromencer(_float fTimeDelta)
{
	if (false == m_bChimera)
		return;

	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	_vec3 vDir = *(_vec3*)&m_matTarget.m[3][0] - vPos;

	_float fDist = D3DXVec3Length(&vDir);

	if (0.5f >= fDist)
	{
		m_bDead = true;
		return;
	}

	D3DXVec3Normalize(&vDir, &vDir);

	vPos += vDir * 10.f * fTimeDelta;

	m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos);
}

void CRangeSkull::Hit(_float fTimeDelta) // 피격
{
	if (m_eCurState != STATE_HIT)
		return;

	m_tTimeHit.Tick_Time(fTimeDelta);
	if (m_tTimeHit.Check_Time())
	{
		Set_Idle();
		m_tTimeHit.fTime = 0.f;
	}
}

void CRangeSkull::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& pTexture : m_pTextureComArr)
		Safe_Release(pTexture);

	if(m_bClone == true)
		CResurrectionManager::Get_Instance()->Sub_Monster(this);
}

CRangeSkull * CRangeSkull::Create(_pDevice pDevice)
{
	CRangeSkull* pInstance = new CRangeSkull(pDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CRangeSkull Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRangeSkull::Clone(void * pArg)
{
	CRangeSkull* pInstance = new CRangeSkull(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CRangeSkull Create - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_vec3 CRangeSkull::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxColliderCom->Get_MinPos();
	return vMinPos;
}

_vec3 CRangeSkull::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxColliderCom->Get_MaxPos();
	return vMaxPos;
}

_vec3 CRangeSkull::Get_WorldPos()
{
	return m_pBoxColliderCom->Get_ColliderWolrdPos();
}

Engine::_vec3 CRangeSkull::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}

void CRangeSkull::Notify(void * pMessage)
{
	if (nullptr == pMessage && m_bDie == true && m_bOnce == false)
	{
		// 리저렉션
		m_bResurrection = true;

		m_bStateLock = true;

		m_eCurState = STATE_SPAWN;

		m_tStat.iHp = m_tStat.iMaxHp / 2;

		m_bOnce = true;
	}
	else if (nullptr != pMessage)
	{
		// 키메라
		OBJ_INFO* pObj = (OBJ_INFO*)pMessage;

		m_matTarget = pObj->matWorld;

		m_eCurState = STATE_DIE;

		m_bStateLock = true;

		m_bChimera = true;
	}
}

HRESULT CRangeSkull::Tick_Collider(_float fTimeDelta)
{
	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	return S_OK;
}

void CRangeSkull::Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc)
{
	memcpy(&pDesc->tObjInfo.matWorld, m_pTransformCom->Get_WorldMatrixPtr(), sizeof(_mat));
}

const _vec3 & CRangeSkull::Get_BoxColliderCenter() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pBoxColliderCom->Get_ColliderCenter();
}
