#include "stdafx.h"
#include "..\public\MonsterNormalMelee.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "DmgFont.h"


CMonsterNormalMelee::CMonsterNormalMelee(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CMonster(pGraphicDevice)
{
	ZeroMemory(m_pTextureArr, sizeof(m_pTextureArr));
}

CMonsterNormalMelee::CMonsterNormalMelee(const CMonsterNormalMelee & rhs)
	:CMonster(rhs)
{
}

HRESULT CMonsterNormalMelee::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CMonsterNormalMelee NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CMonsterNormalMelee::NativeConstruct(void * pArg)
{
	CMonster::MONSTER_STAT tMonsterStat;
	tMonsterStat.iHp = 30;
	tMonsterStat.iMaxHp = 30;
	tMonsterStat.iDmg = 10;

	if (FAILED(__super::NativeConstruct(&tMonsterStat)))
		MSG_FAIL("CMonsterNormalMelee NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CMonsterNormalMelee NativeConstruct - SetUp_Component");

	Check_Frame();

	m_tRetreatCount.fDelay = 1.f;

	return S_OK;
}

_int CMonsterNormalMelee::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CMonsterNormalMelee Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_bRetreat == true)
		m_tRetreatCount.Tick_Time(fTimeDelta);

	Retreat(fTimeDelta);

	AI();
	Check_Frame();
	Move(fTimeDelta);
	Hit(fTimeDelta);

	//지형 타기
	Collision_Terrain(fTimeDelta, pGameInstance, m_pTransform);
	//빌보딩
	BillBoarding(m_pTransform);
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);
	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

_int CMonsterNormalMelee::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CMonsterNormalMelee LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CMonsterNormalMelee::LateTick - m_pRenderer == nullptr");

	if (m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CMonsterNormalMelee::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CMonsterNormalMelee::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CMonsterNormalMelee Render - __super");

	if (m_pTransform == nullptr ||
		m_pTextureArr == nullptr ||
		m_pBoxCollider == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("CMonsterNormalMelee::Render - Com is null");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CMonsterNormalMelee::Render - m_pTransform Bind_OnDevice");

	if (FAILED(m_pTextureArr[m_eCurState]->Bind_OnDevice(Get_FrameIndex())))
		MSG_FAIL("CMonsterNormalMelee::Render - m_pTextureArr[m_eCurState]->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CMonsterNormalMelee::Render - Set_RenderState");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CMonsterNormalMelee::Render - m_pVIBuffer->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CMonsterNormalMelee::Render - Release_RenderState");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CMonsterNormalMelee::Render - m_pBoxCollider->Render");

	return S_OK;
}

void CMonsterNormalMelee::Set_State(MONSTER_FSM eState)
{
	if (m_bStateLock)
		return;

	m_eCurState = eState;
}

void CMonsterNormalMelee::Set_Idle()
{
	m_bStateLock = false;
	Set_State(FSM_IDLE);
	Check_Frame();
}

HRESULT CMonsterNormalMelee::Sub_Hp(_uint iDmg)
{
	__super::Sub_Hp(iDmg);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CDmgFont::DESC tDesc;
	tDesc.bGuard = true;
	tDesc.iDmg = iDmg;
	tDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS) + m_pTransform->Get_State(CTransform::STATE_UP);
	tDesc.vScale = m_pTransform->Get_Scale();
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDmgFont, pTagLayerDmgFont, &tDesc)))
		MSG_FAIL("pGameInstance->Add_GameObjectToLayer - pTagPrototypeDmgFont");
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CMonsterNormalMelee::Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc)
{
	memcpy(&pDesc->tObjInfo.matWorld, m_pTransform->Get_WorldMatrixPtr(), sizeof(_mat));
}

HRESULT CMonsterNormalMelee::SetUp_Component(void *pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 10.f;
	tTransformDesc.fDegree = 3.f;
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform, &tTransformDesc)))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - m_pTransform");
	m_pTransform->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - m_pRenderer");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PlayerBuffer"), pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pVIBufferCom");

	wstring wstrTextureName;

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Idle";
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTextureIdle", (CComponent**)&m_pTextureArr[FSM_IDLE])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - FSM_IDLE");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Attack";
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTextureAttack", (CComponent**)&m_pTextureArr[FSM_ATTACK])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - FSM_ATTACK");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Die";
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTextureDie", (CComponent**)&m_pTextureArr[FSM_DIE])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - FSM_DIE");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Hit";
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTextureHit", (CComponent**)&m_pTextureArr[FSM_HIT])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - FSM_HIT");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Move";
	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTextureMove", (CComponent**)&m_pTextureArr[FSM_MOVE])))
		MSG_FAIL("CMonsterNormalMelee::SetUp_Component - FSM_MOVE");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);

	return S_OK;
}

HRESULT CMonsterNormalMelee::Set_RenderState()
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

HRESULT CMonsterNormalMelee::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CMonsterNormalMelee::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CMonsterNormalMelee::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

void CMonsterNormalMelee::AI()
{
	m_pTransformPlayer = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
	if (m_pTransformPlayer == nullptr)
		MSG_VOID("CMonsterNormalMelee::AI - m_pTransformPlayer == nullptr");

	_float fDistance = m_pTransform->Get_DistanceTarget(m_pTransformPlayer);

	if (fDistance <= 10.f)
		Set_State(MONSTER_FSM::FSM_MOVE);
	else
		Set_State(MONSTER_FSM::FSM_IDLE);

	if (m_eCurState == MONSTER_FSM::FSM_MOVE)
	{
		if (fDistance <= 1.f)
		{
			Set_State(MONSTER_FSM::FSM_ATTACK);
			m_bStateLock = true;
		}
	}

	if (Check_Dead())
	{
		m_bStateLock = false;
		Set_State(MONSTER_FSM::FSM_DIE);
		m_bStateLock = true;
	}
}

void CMonsterNormalMelee::Check_Frame()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CMonsterNormalMelee::FSM_IDLE:
			m_tFrame.Set_Value(1);
			break;
		case Client::CMonsterNormalMelee::FSM_MOVE:
			m_tFrame.Set_Value(4);
			break;
		case Client::CMonsterNormalMelee::FSM_ATTACK:
			m_tFrame.Set_Value(7);
			break;
		case Client::CMonsterNormalMelee::FSM_DIE:
			m_tFrame.Set_Value(3);
			break;
		case Client::CMonsterNormalMelee::FSM_HIT:
			m_tFrame.Set_Value(2);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CMonsterNormalMelee::Move(_float fTimeDelta)
{
	if (m_pTransformPlayer == nullptr ||
		m_pTransform == nullptr)
		MSG_VOID("CMonsterNormalMelee::Move - null");

	if (m_eCurState != MONSTER_FSM::FSM_MOVE)
		return;

	m_pTransform->Chase_Target(m_pTransformPlayer, fTimeDelta, 0.5f);
}

void CMonsterNormalMelee::Hit(_float fTimeDelta)
{
	if (m_eCurState != FSM_HIT)
		return;

	m_tTimeHit.Tick_Time(fTimeDelta);
	if (m_tTimeHit.Check_Time())
	{
		Set_Idle();
		m_tTimeHit.fTime = 0.f;
	}
}

void CMonsterNormalMelee::Tick_Frame(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fTime >= m_tFrame.fDelay)
	{
		m_tFrame.fTime = 0.f;
		m_tFrame.iStart++;

		if (m_eCurState == FSM_ATTACK && m_tFrame.iStart == 4)
		{
			CAttack::DESC tDesc;
			tDesc.iDmg = 10;
			_vec3 vPlayerPos = m_pTransformPlayer->Get_State(CTransform::STATE_POS);
			_vec3 vMyPos = m_pTransform->Get_State(CTransform::STATE_POS);
			_vec3 vDir = vPlayerPos - vMyPos;
			D3DXVec3Normalize(&vDir, &vDir);
			tDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS) + vDir * 1.f;
			tDesc.vScale = _vec3(1.f, 1.f, 1.f);
			pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerMonsterAttack, &tDesc);
		}

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;

			if (m_eCurState == FSM_ATTACK)
			{
				m_bRetreat = true;
				Set_RetreatPos();
			}

			if (m_eCurState == FSM_DIE)
			{
				m_tFrame.iStart = m_tFrame.iEnd - 1;
				Set_Dead();
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CMonsterNormalMelee::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	if (pAttack != nullptr && m_eCurState != FSM_DIE)
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
		Set_State(FSM_HIT);
		m_tTimeHit.fTime = 0.f;
		m_tTimeHit.fDelay = 0.5f;
		m_tFrame.iStart = 0;
		m_bStateLock = true;

		pAttack->Set_Dead();
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CMonsterNormalMelee::Retreat(_float fTimeDelta)
{
	if (m_bRetreat == false)
		return;

	_vec3 vPos = m_pTransform->Get_State(CTransform::STATE_POS);
	m_eCurState = FSM_MOVE;
	vPos += m_vRetreatDir * fTimeDelta * 2.5f;
	m_pTransform->SetUp_State(CTransform::STATE_POS, vPos);

	if (m_tRetreatCount.Check_Time())
	{
		m_bStateLock = false;
		m_bRetreat = false;
		m_tRetreatCount.fTime = 0.f;
	}
}

void CMonsterNormalMelee::Set_RetreatPos()
{
	_mat matMyWorld = *m_pTransform->Get_WorldMatrixPtr();
	_mat matPlayerWorld = *m_pTransformPlayer->Get_WorldMatrixPtr();

	_vec3 vPlayerPos = m_pTransformPlayer->Get_State(CTransform::STATE_POS);
	_vec3 vMyPos = m_pTransform->Get_State(CTransform::STATE_POS);

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

_bool CMonsterNormalMelee::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, CTransform * pOut)
{
	_vec3 vPos;
	if (m_pVIBuffer->Picking(vMouseRay, vMousePivot, &vPos))
	{
		pOut = dynamic_cast<CTransform*>(Get_Component(pTagComTransform));
		return true;
	}
	return false;
}

CMonsterNormalMelee * CMonsterNormalMelee::Create(_pDevice pDevice)
{
	CMonsterNormalMelee* pInstance = new CMonsterNormalMelee(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CMonsterNormalMelee Create Failed");
	}

	return pInstance;
}

void CMonsterNormalMelee::Free()
{
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);

	for (auto& pTexture : m_pTextureArr)
		Safe_Release(pTexture);

	__super::Free();
}

CGameObject * CMonsterNormalMelee::Clone(void * pArg)
{
	CGameObject* pInstance = new CMonsterNormalMelee(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CMonsterNormalMelee Clone Failed");
	}

	return pInstance;
}

_vec3 CMonsterNormalMelee::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxCollider->Get_MinPos();

	return vMinPos;
}

_vec3 CMonsterNormalMelee::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxCollider->Get_MaxPos();

	return vMaxPos;
}

_vec3 CMonsterNormalMelee::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

Engine::_vec3 CMonsterNormalMelee::Get_Length()
{
	return m_pTransform->Get_Scale();
}

const _vec3 & CMonsterNormalMelee::Get_BoxColliderCenter() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pBoxCollider->Get_ColliderCenter();
}
