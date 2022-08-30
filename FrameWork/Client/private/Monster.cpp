#include "stdafx.h"
#include "..\public\Monster.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "InputDevice.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "PoisonCubeHit.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CObserver(pGraphicDevice)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CObserver(rhs), m_tFrame(rhs.m_tFrame)
{
}

HRESULT CMonster::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CMonster NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CMonster::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CMonster NativeConstruct - __super");

	if (pArg == nullptr)
		MSG_FAIL("CMonster::NativeConstruct - MonsterStat is null");
	m_tStat = *(MONSTER_STAT*)(pArg);

	return S_OK;
}

_int CMonster::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CMonster Tick - __super");

	//텍스쳐 프레임 갱신
	Tick_Frame(fTimeDelta);

	Tick_Poison(fTimeDelta);

	return 0;
}

_int CMonster::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CMonster LateTick - __super");

	return _int();
}

HRESULT CMonster::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CMonster Render - __super");

	return S_OK;
}

_bool CMonster::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, CTransform * pOut)
{
	return false;
}

void CMonster::Set_Frame(_int iEnd, wstring wstrObjectKey, wstring wstrStateKey, wstring wstrDir, _int iStart /*= 0*/
	, _float fTime /*= 0.f*/, _float fDelay /*= 0.1f*/)
{
	m_tFrame.iStart = iStart;
	m_tFrame.iEnd = iEnd;
	m_tFrame.fDelay = fDelay;
	m_tFrame.fTime = fTime;
}

HRESULT CMonster::Sub_Hp(_uint iDmg)
{
	m_tStat.iHp -= iDmg;
	if (m_tStat.iHp <= 0)
		m_tStat.iHp = 0;

	return S_OK;
}

void CMonster::Set_Poison(TIME tTimePoisonTick, TIME tTimePoisonEnd, TIME tTimePoisonCreateHitParticle, _int iPoisonDmg)
{
	m_bPoison = true;
	m_tTimePoisonTick = tTimePoisonTick;
	m_tTimePoisonEnd = tTimePoisonEnd;
	m_tTimePoisonCreateHitParticle = tTimePoisonCreateHitParticle;
	m_iPoisonDmg = iPoisonDmg;
}

HRESULT CMonster::SetUp_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CMonster SetUp_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CMonster::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CMonster Release_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

void CMonster::Tick_Frame(_float fTimeDelta)
{
	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fDelay <= m_tFrame.fTime)
	{
		m_tFrame.fTime = 0.f;
		m_tFrame.iStart++;
		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;
		}
	}
}

void CMonster::Tick_Poison(_float fTimeDelta)
{
	if (!m_bPoison)
		return;

	m_tTimePoisonTick.Tick_Time(fTimeDelta);
	m_tTimePoisonEnd.Tick_Time(fTimeDelta);
	m_tTimePoisonCreateHitParticle.Tick_Time(fTimeDelta);

	if (m_tTimePoisonTick.Check_Time())
	{
		Sub_Hp(m_iPoisonDmg);
		m_tTimePoisonTick.fTime = 0.f;
	}

	if (m_tTimePoisonEnd.Check_Time())
	{
		m_tTimePoisonEnd.fTime = 0.f;
		m_bPoison = false;
		m_iPoisonDmg = 0;
		m_tTimePoisonTick.fTime = 0.f;
	}

	if (m_tTimePoisonCreateHitParticle.Check_Time())
	{
		m_tTimePoisonCreateHitParticle.fTime = 0.f;
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CPoisonCubeHit::DESC tDesc;
		Set_PoisonHitTransform(&tDesc);

		// 0나오는 거 방지용 곱하기 10
		_int iX = _int(tDesc.tObjInfo.Get_Scale().x * 10) / 2;
		if (iX == 0)
			MSGBOX("CMonster::Tick_Poison - Monster의 Set_PoisonHitTransform 구현 안됬음");

		_float fX = rand() % iX / 10.f;

		_vec3 vPos = tDesc.tObjInfo.Get_State(OBJ_INFO::STATE_POS);
		//오른쪽 생성
		if (m_bCreateRight)
			vPos.x += fX;
		//왼쪽 생성
		else
			vPos.x -= fX;

		//다들 기본 트랜스폼이 밑에 있는 경우가 많아서 위치 수정해줘야 할 가능성 있음
		_vec3 vBoxColliderCenter = Get_BoxColliderCenter();
		vPos += _vec3(vBoxColliderCenter.x * tDesc.tObjInfo.Get_Scale().x,
			vBoxColliderCenter.y * tDesc.tObjInfo.Get_Scale().y, 
			vBoxColliderCenter.z * tDesc.tObjInfo.Get_Scale().z);
		tDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, vPos);

		_vec3 vScale = tDesc.tObjInfo.Get_Scale();
		vScale /= 5.f;
		tDesc.tObjInfo.SetUp_Scale(vScale.x, vScale.y, vScale.z);

		lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTexturePoisonCube);
		tDesc.tObjInfo.iTextureIndex = 1;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypePoisonCubeHit, pTagLayerNonCollision, &tDesc)))
			MSGBOX("CMonster::Tick_Poison - pTagPrototypePoisonCubeHit");

		RELEASE_INSTANCE(CGameInstance);
	}
}

_bool CMonster::Check_Dead()
{
	if (m_tStat.iHp <= 0)
	{
		m_tStat.iHp = 0;
		return true;
	}

	return false;
}

void CMonster::Collision_Terrain(_float fTimeDelta, CGameInstance* pInstance, CTransform* pTransform)
{
	//터레인 여러개 되면 어떻게 할 것인가
	CTerrain* pTerrain = (CTerrain*)pInstance->Get_GameObjPtr(pInstance->Get_CurrentLevel(), L"LayerTerrain", 0);

	if (pTerrain == nullptr)
		return;

	_vec3 vTerrainPos = pTerrain->Compute_Height(pTransform);
	pTransform->SetUp_State(CTransform::STATE_POS, vTerrainPos);
}

HRESULT CMonster::BillBoarding(CTransform * pTransform)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	_mat matCamWorld = pGameInstance->Get_CamWorld();
	_vec3 vEffectScale = pTransform->Get_Scale();

	pTransform->SetUp_State(CTransform::STATE_RIGHT, (*(_vec3*)(&matCamWorld.m[0][0])) * vEffectScale.x);
	pTransform->SetUp_State(CTransform::STATE_LOOK, (*(_vec3*)(&matCamWorld.m[2][0])) * vEffectScale.z);
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CMonster::Free()
{
	__super::Free();
}

void CMonster::Collision(CGameObject * pColObj)
{
}

void CMonster::Notify(void * pMessage)
{
}
