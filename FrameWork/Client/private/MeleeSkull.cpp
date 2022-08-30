#include "stdafx.h"
#include "..\public\MeleeSkull.h"
#include "ResurrectionManager.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "DmgFont.h"
#include "MonsterColliderManager.h"


CMeleeSkull::CMeleeSkull(_pDevice pGraphicDevice)
	: CMonster(pGraphicDevice)
{
	ZeroMemory(m_pTextureComArr, sizeof(m_pTextureComArr));
}

CMeleeSkull::CMeleeSkull(const CMeleeSkull & rhs)
	: CMonster(rhs)
{
}

HRESULT CMeleeSkull::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CMeleeSkull NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CMeleeSkull::NativeConstruct(void * pArg)
{
	CMonster::MONSTER_STAT tMonsterStat;
	tMonsterStat.iMaxHp = 30;
	tMonsterStat.iHp = tMonsterStat.iMaxHp;
	tMonsterStat.iDmg = 5;

	if (FAILED(__super::NativeConstruct(&tMonsterStat)))
		MSG_FAIL("CMonsterNormalMelee NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CMonsterNormalMelee NativeConstruct - SetUp_Component");

	Check_Frame();

	m_bClone = true;

	m_tRetreatCount.fDelay = 2.f;

	CResurrectionManager::Get_Instance()->Add_Monster(this);

	return S_OK;
}

_int CMeleeSkull::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CMonsterNormalMelee Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if(m_bRetreat == true)
		m_tRetreatCount.Tick_Time(fTimeDelta);

	Retreat(fTimeDelta);

	AI();
	
	Move(fTimeDelta);

	Go_Necromencer(fTimeDelta);

	Check_Frame();

	if(m_ePreState != STATE_DIE)
		Collision_Terrain(fTimeDelta, pGameInstance, m_pTransformCom);

	BillBoarding(m_pTransformCom);

	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CMeleeSkull::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CMonsterNormalMelee LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CMonsterNormalMelee::LateTick - m_pRendererCom == nullptr");

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CMonsterNormalMelee::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CMeleeSkull::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CMonsterNormalMelee Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pTextureComArr == nullptr ||
		m_pBoxColliderCom == nullptr ||
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CMonsterNormalMelee::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CMonsterNormalMelee::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureComArr[m_ePreState]->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CMonsterNormalMelee::Render - m_pTextureComArr[m_eCurState]->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CMonsterNormalMelee::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CMonsterNormalMelee::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CMonsterNormalMelee::Render - Release_RenderState");

	if (FAILED(m_pBoxColliderCom->Render()))
		MSG_FAIL("CMonsterNormalMelee::Render - m_pBoxColliderCom->Render");

	return S_OK;
}

_bool CMeleeSkull::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, CTransform * pOut)
{
	_vec3 vPos;
	if (m_pVIBufferCom->Picking(vMouseRay, vMousePivot, &vPos))
	{
		pOut = dynamic_cast<CTransform*>(Get_Component(pTagComTransform));
		return true;
	}
	return false;
}

HRESULT CMeleeSkull::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 10.f;
	tTransformDesc.fDegree = 3.f;
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC,TEXT("MeleeSkullBuffer"), pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - m_pVIBufferCom");

	// Attack1
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullAttack1"), L"ComTextureAttack1", (CComponent**)&m_pTextureComArr[STATE_ATTACK])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - TEXTURE_ATTACK1");

	// Death
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullDeath"), L"ComTextureDeath", (CComponent**)&m_pTextureComArr[STATE_DEATH])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - TEXTURE_DEATH");

	// Die
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullDie"), L"ComTextureDie", (CComponent**)&m_pTextureComArr[STATE_DIE])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - TEXTURE_DIE");

	// Hit
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullHit"), L"ComTextureHit", (CComponent**)&m_pTextureComArr[STATE_HIT])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - TEXTURE_HIT");

	// Idle
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullIdle"), L"ComTextureIdle", (CComponent**)&m_pTextureComArr[STATE_IDLE])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - TEXTURE_IDLE");

	// Move
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullMove"), L"ComTextureMove", (CComponent**)&m_pTextureComArr[STATE_MOVE])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - TEXTURE_MOVE");

	// Spawn
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullSpawn"), L"ComTextureSpawn", (CComponent**)&m_pTextureComArr[STATE_SPAWN])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - TEXTURE_SPAWN");

	OBJ_INFO* pColDat = CMonsterColliderManager::Get_Instance()->Get_ColliderDat(CMonsterColliderManager::COLLIDER_MELEE_SKULL);

	m_pBoxColliderCom = CBoxCollider::Create_Tool(m_pGraphicDevice, pColDat->vColliderCenter, pColDat->vColliderSize, pColDat->dwColliderColor, this);

	return S_OK;
}

HRESULT CMeleeSkull::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CMonsterNormalMelee::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CMonsterNormalMelee::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CMonsterNormalMelee::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CMonsterNormalMelee::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CMeleeSkull::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CMonsterNormalMelee::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CMonsterNormalMelee::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

void CMeleeSkull::Set_State(STATE eState)
{
	if (m_bStateLock)
		return;

	m_eCurState = eState;
}

void CMeleeSkull::Set_Idle()
{
	m_bStateLock = false;
	Set_State(STATE_IDLE);
	Check_Frame();
}

void CMeleeSkull::Check_Frame()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CMeleeSkull::STATE_IDLE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_IDLE]->Get_ImageCount() - 1);
			break;
		case Client::CMeleeSkull::STATE_MOVE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_MOVE]->Get_ImageCount() - 1);
			break;
		case Client::CMeleeSkull::STATE_ATTACK:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_ATTACK]->Get_ImageCount() - 1);
			break;
		case Client::CMeleeSkull::STATE_HIT:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_HIT]->Get_ImageCount() - 1);
			break;
		case Client::CMeleeSkull::STATE_DEATH:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_DEATH]->Get_ImageCount() - 1);
			break;
		case Client::CMeleeSkull::STATE_DIE:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_DIE]->Get_ImageCount() - 1);
			break;
		case Client::CMeleeSkull::STATE_SPAWN:
			m_tFrame.Set_Value(m_pTextureComArr[STATE_SPAWN]->Get_ImageCount() - 1);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CMeleeSkull::Tick_Frame(_float fTimeDelta)
{
	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fTime >= m_tFrame.fDelay)
	{
		m_tFrame.fTime = 0.f;
		m_tFrame.iStart++;

		if (m_ePreState == STATE_ATTACK && m_tFrame.iStart == 4)
			Attack();

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;

			switch (m_ePreState)
			{
			case Client::CMeleeSkull::STATE_ATTACK:
				m_bRetreat = true;
				Set_RetreatPos();
				break;
			case Client::CMeleeSkull::STATE_HIT:
				m_eCurState = STATE::STATE_IDLE;
				m_bStateLock = false;
				break;
			case Client::CMeleeSkull::STATE_DEATH:
				m_eCurState = STATE::STATE_DIE;
				break;
			case Client::CMeleeSkull::STATE_SPAWN:
				m_eCurState = STATE::STATE_IDLE;
				m_bStateLock = false;
				break;
			case Client::CMeleeSkull::STATE_DIE:
				break;
			}
		}
	}
}

void CMeleeSkull::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	if (pAttack != nullptr && m_eCurState != STATE_DIE && m_eCurState != STATE_DEATH)
	{
		_uint iDmg = pAttack->Get_Dmg();
		//공격이 단일 공격인데 이미 맞은 오브젝트가 잇는 경우
		if (pAttack->Get_SingleAttack() && pAttack->Get_Dead())
			return;

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

HRESULT CMeleeSkull::Sub_Hp(_uint iDmg)
{
	__super::Sub_Hp(iDmg);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
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

void CMeleeSkull::AI()
{
	m_bMove = false;

	if (m_bStateLock == false)
	{

	m_pPlayerTransformCom = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
	if (m_pPlayerTransformCom == nullptr)
		MSG_VOID("CMonsterNormalMelee::AI - m_pTransformComPlayer == nullptr");

		_float fDistance = m_pTransformCom->Get_DistanceTarget(m_pPlayerTransformCom);

		if (fDistance <= 15.f)
		{
			Set_State(STATE::STATE_MOVE);
			m_bMove = true;
		}
		else
			Set_State(STATE::STATE_IDLE);

		if (m_eCurState == STATE::STATE_MOVE)
		{
			if (fDistance <= 2.f)
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
		m_bDie = true;
		m_bStateLock = true;
	}
}

void CMeleeSkull::Move(_float fTimeDelta) // 이동
{
	if (m_bMove == false)
		return;

	if (m_pPlayerTransformCom == nullptr ||
		m_pTransformCom == nullptr)
		MSG_VOID("CMonsterNormalMelee::Move - null");

	m_pTransformCom->Chase_Target(m_pPlayerTransformCom, fTimeDelta * 2.5f, 0.5f);
}

void CMeleeSkull::Attack() // 공격
{
	if (m_ePreState != STATE::STATE_ATTACK)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CAttack::DESC tDesc;
	tDesc.bDestroy = false;
	tDesc.iDmg = 1;//m_tStat.iDmg;
	tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	tDesc.vScale = _vec3(4.f, 2.f, 4.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerMonsterAttack, &tDesc)))
		MSG_VOID("CPlayer::Tick_FramingWarriorSkill1 - pGameInstance->Add_GameObjectToLayer pTagPrototypeAttack");

	RELEASE_INSTANCE(CGameInstance);
}

void CMeleeSkull::Go_Necromencer(_float fTimeDelta)
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

void CMeleeSkull::Retreat(_float fTimeDelta)
{
	if (m_bRetreat == false)
		return;

	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	m_eCurState = STATE_MOVE;
	vPos += m_vRetreatDir * fTimeDelta * 2.5f;
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos);

	if (m_tRetreatCount.Check_Time())
	{
		m_bStateLock = false;
		m_bRetreat = false;
		m_tRetreatCount.fTime = 0.f;
	}
}

void CMeleeSkull::Set_RetreatPos()
{
	_mat matMyWorld = *m_pTransformCom->Get_WorldMatrixPtr();
	_mat matPlayerWorld = *m_pPlayerTransformCom->Get_WorldMatrixPtr();

	_vec3 vPlayerPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POS);
	_vec3 vMyPos = m_pTransformCom->Get_State(CTransform::STATE_POS);

	_vec3 vDirToPlayer = vPlayerPos - vMyPos;
	D3DXVec3Normalize(&vDirToPlayer, &vDirToPlayer);
	_vec3 vDirNomarl = { 0.f,0.f,1.f };
	D3DXVec3Normalize(&vDirNomarl, &vDirNomarl);

	_float fCosTheta = D3DXVec3Dot(&vDirToPlayer, &vDirNomarl);

	_float fAngle = acosf(fCosTheta);

	fAngle = D3DXToDegree(fAngle);

	if (vMyPos.x > vPlayerPos.x)
		fAngle *= -1.f;

	fAngle += 90 + _float(rand() % 180);

	OBJ_INFO	tObj;
	_vec3		vLook;

	tObj.matWorld = matMyWorld;

	tObj.SetUp_Rotation(tObj.Get_State(OBJ_INFO::STATE_UP), fAngle);

	vLook = tObj.Get_State(OBJ_INFO::STATE_LOOK);

	m_vRetreatDir = *D3DXVec3Normalize(&vLook, &vLook);
}

void CMeleeSkull::Hit(_float fTimeDelta) // 피격
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

void CMeleeSkull::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& pTexture : m_pTextureComArr)
		Safe_Release(pTexture);

	if (m_bClone == true)
		CResurrectionManager::Get_Instance()->Sub_Monster(this);
}

CMeleeSkull * CMeleeSkull::Create(_pDevice pDevice)
{
	CMeleeSkull* pInstance = new CMeleeSkull(pDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMeleeSkull Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeleeSkull::Clone(void * pArg)
{
	CMeleeSkull* pInstance = new CMeleeSkull(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMeleeSkull Create - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_vec3 CMeleeSkull::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxColliderCom->Get_MinPos();
	return vMinPos;
}

_vec3 CMeleeSkull::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxColliderCom->Get_MaxPos();
	return vMaxPos;
}

_vec3 CMeleeSkull::Get_WorldPos()
{
	return m_pBoxColliderCom->Get_ColliderWolrdPos();
}

Engine::_vec3 CMeleeSkull::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}

void CMeleeSkull::Notify(void * pMessage)
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

HRESULT CMeleeSkull::Tick_Collider(_float fTimeDelta)
{
	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	return S_OK;
}

void CMeleeSkull::Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc)
{
	memcpy(&pDesc->tObjInfo.matWorld, m_pTransformCom->Get_WorldMatrixPtr(), sizeof(_mat));
}

const _vec3 & CMeleeSkull::Get_BoxColliderCenter() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pBoxColliderCom->Get_ColliderCenter();
}
